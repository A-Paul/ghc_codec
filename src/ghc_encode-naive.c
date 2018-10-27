#if DEBUG > 0
#include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "ghc.h"
#include "ghc_codec.h"
#include "ghc_codec_intern.h"

/*
 * Goes into ghc.h later
 */
#define GHC_ZEROS_CNT_MAX    (0x11U)   /*! Maximum zero sequence lenth */
#define GHC_ZEROS_CNT_MIN    (0x02U)   /*! Minimum zero sequence lenth */

#define GHC_BREF_OFFS_LSHIFT (0x00U)   /*! Placeholder */

/*
 * Goes into ghc_codec_intern.h later
 */
#define BREF_BC_LEN_MASK    (GHC_BREF_CNT_MASK >> GHC_BREF_CNT_RSHIFT)
#define BREF_BC_OFFS_MASK   (GHC_BREF_OFFS_MASK)
#define EXT_BC_LEN_MASK     (GHC_EXT_CNT_MASK >> GHC_EXT_CNT_RSHIFT)
#define EXT_BC_OFFS_MASK    (GHC_EXT_OFFS_MASK << GHC_EXT_OFFS_LSHIFT)

#define EXT_FULL_LEN_MASK   (EXT_BC_LEN_MASK | BREF_BC_LEN_MASK)
#define EXT_FULL_OFFS_MASK  (EXT_BC_OFFS_MASK | BREF_BC_OFFS_MASK)


static inline int8_t comp_not_full(struct ghc_codec* encoder)
{
    return (encoder->pos_comp < encoder->size_comp);
}

static inline int8_t payload_not_empty(struct ghc_codec* encoder)
{
    return (encoder->pos_unco < encoder->size_unco);
}


/*! Return the count of zero byte prefix in actual payload.
 */
static inline uint32_t get_zeros_len(
    struct ghc_codec* encoder, uint8_t* zeros, uint16_t pos_plod)
{
    uint16_t max_len = encoder->size_unco - pos_plod;
    uint8_t i = 0;
    *zeros = 0;

    if (GHC_ZEROS_CNT_MAX < max_len) {
        max_len = GHC_ZEROS_CNT_MAX;
    }

    for (i = 0; i < max_len; ++i) {
        if(0 != encoder->uncompressed[pos_plod + i]) {
            break;
        }
        else {
            ++*zeros;
        }
    }
    return *zeros;
}


/*! Return the count of matching bytes between two sequences.
 * \param[in]  encoder
 * \param[out] match_len
 * \param[in]  max_len upper bound for comparison
 * \param[in]  pos_dict dictionary start position
 * \param[in]  pos_plod payload start position
 */
static inline uint32_t get_match_len( struct ghc_codec* encoder,
    uint16_t* match_len, uint16_t max_len, uint16_t pos_dict, uint16_t pos_plod)
{
    uint32_t i = 0;
    *match_len = 0;
    for (i = 0; i < max_len; ++i) {
        if(encoder->uncompressed[pos_dict + i] !=
           encoder->uncompressed[pos_plod + i]) {
               break;
        }
        else {
            ++*match_len;
        }
    }
    return i;
}


/* Return position and lenghth for longest subsequence of #encoder.unco
 * tha matches subsequence of #encoder.unco starting at dict_idx.
 *
 * \param[out] dict_idx
 * \param[out] max_len
 * \param[in]  encoder
 * \param[in]  pload_idx start for temporary pload start
 */
static inline void get_longest_match(
    struct ghc_codec* encoder, uint16_t pload_idx,
    uint16_t* dict_idx, uint16_t* max_len)
{
    *dict_idx = 0;
    *max_len = 0;
    uint16_t dict_pivot;
    uint16_t match_len;
    uint16_t temp_dict_suffix_len;
    uint16_t match_max_len = encoder->size_unco - pload_idx;

    for( dict_pivot = 0; dict_pivot <= (pload_idx - GHC_BREF_CNT_VALUE_MIN); ++dict_pivot) {

        temp_dict_suffix_len = pload_idx - dict_pivot;
        /* compare length is bound by either actual dict or payload suffix. */
        if (temp_dict_suffix_len < match_max_len) {
            match_max_len = temp_dict_suffix_len;
        }
        /* Can't get longer match */
        /* if( temp_dict_suffix_len < *max_len) {
         *  break;
         */
        get_match_len(
            encoder, &match_len, match_max_len, dict_pivot, pload_idx);
#if DEBUG >= 2
        printf("%s:%d: pload_idx: %03d dict_pivot: %03d "
            "match_max_len: %03d match_len: %03d max_len: %03d byte: 0x%02x\n",
            __func__, __LINE__, pload_idx - 48, dict_pivot, match_max_len,
            match_len, *max_len, encoder->uncompressed[pload_idx]);
#endif
        if(match_len >= *max_len) { /* Prefer *same* lengths with smaller indices */
            *max_len = match_len;
            *dict_idx = dict_pivot;
        }
    }
}


static inline void write_bref_code(struct ghc_codec* encoder,
    uint16_t longest_match_len, uint16_t longest_match_offs)
{
    uint16_t match_len = longest_match_len - GHC_BREF_CNT_ADD;
    uint16_t match_offs = longest_match_offs - longest_match_len;
    uint8_t  bref_bc = 0;

#if DEBUG >= 3
    printf("%s:%d: match_len: 0x%02X match_offs: 0x%02X longest_match_offs: 0x%02X\n",
            __func__, __LINE__, match_len, match_offs, longest_match_offs);
#endif

    while ((match_len & ~EXT_FULL_LEN_MASK) ||
           (match_offs & ~EXT_FULL_OFFS_MASK)) {

        bref_bc = GHC_EXT_BC; /* 0b10100000 */

        if (match_len & ~EXT_FULL_LEN_MASK) {
            match_len -= EXT_FULL_LEN_MASK + 1;
            bref_bc |= GHC_EXT_CNT_MASK; /* 0b1011ssss */
        }
        if (match_offs & ~EXT_FULL_OFFS_MASK) {
            match_offs -= EXT_FULL_OFFS_MASK + 1;
            bref_bc |= GHC_EXT_OFFS_MASK; /* 0b101n1111 */
        }
        encoder->compressed[encoder->pos_comp] = bref_bc;
        ++encoder->pos_comp;
    }

    bref_bc = GHC_EXT_BC |  /* 0b101nssss */
              ((match_len & EXT_BC_LEN_MASK) << GHC_EXT_CNT_RSHIFT) |
              ((match_offs & EXT_BC_OFFS_MASK) >> GHC_EXT_OFFS_LSHIFT);

    if (bref_bc != GHC_EXT_BC) {
        encoder->compressed[encoder->pos_comp] = bref_bc;
        ++encoder->pos_comp;
    }

    bref_bc = GHC_BREF_BC |  /* 0b11nnnkkk */
              ((match_len & BREF_BC_LEN_MASK) << GHC_BREF_CNT_RSHIFT) |
              ((match_offs & BREF_BC_OFFS_MASK) >> GHC_BREF_OFFS_LSHIFT);

    encoder->compressed[encoder->pos_comp] = bref_bc;
    ++encoder->pos_comp;
}

/*
 * encoder.pos_unco points at the first uncompressed entry
 * encoder.pos_comp points at the first uncompressed entry
 */
int ghc_encode(struct ghc_codec* encoder)
{
    int32_t retval = -1;
    int32_t clean  = 0;
    /*!
     * Remeber the start position of longest matching substring.
     * Initialized with encoder state.
     */
    uint16_t longest_match_idx = encoder->pos_unco;
    /*! remeber the length of longest matching substring */
    uint16_t longest_match_len = 0;
    uint8_t  zero_seq_len = 0;
    uint8_t  copy_seq_len = 0;
    uint16_t pload_provision_idx = encoder->pos_unco;

    clean = 1;

    while (clean && comp_not_full(encoder) && payload_not_empty(encoder))
    {
        longest_match_idx = pload_provision_idx;
        longest_match_len = 0;

        /* Look for sequences */
        get_zeros_len(encoder, &zero_seq_len, pload_provision_idx);
        get_longest_match(encoder, pload_provision_idx,
            &longest_match_idx, &longest_match_len);

#if DEBUG >= 2
        printf("%s:%d: longest_match |_idx:0x%03x|_len:0x%03x|zero_len:0x%03x|\n",
            __func__, __LINE__, longest_match_idx, longest_match_len, zero_seq_len);
        printf("%s:%d: longest_match |comp:0x%03x|plod:0x%03x|provision:0x%03x|\n",
            __func__, __LINE__, encoder->pos_comp, encoder->pos_unco, pload_provision_idx);
#endif

        if (1U < zero_seq_len || 1U < longest_match_len ||
            copy_seq_len == GHC_COPY_CNT_MAX || pload_provision_idx == encoder->size_unco) {
            /* copy_provision(encoder, pload_provision_idx); */
            if (copy_seq_len) {
                encoder->compressed[encoder->pos_comp] =
                    GHC_COPY_BC | (copy_seq_len & GHC_COPY_CNT_MASK);
                ++(encoder->pos_comp);

                while (copy_seq_len) {
                    encoder->compressed[encoder->pos_comp] =
                        encoder->uncompressed[encoder->pos_unco];
                    ++(encoder->pos_comp);
                    ++(encoder->pos_unco);
                    --copy_seq_len;
                }
            }
        }

        if (1U < zero_seq_len || 1U < longest_match_len) {
            if(longest_match_len > zero_seq_len) {

                /* Compose and set bytecodes */
                write_bref_code(encoder, longest_match_len,
                    pload_provision_idx - longest_match_idx);

                /* Update encoder "state". */
                encoder->pos_unco += longest_match_len;
            }
            else {
                /* Compose and set bytecode */
                encoder->compressed[encoder->pos_comp] =
                (GHC_ZEROS_BC | (zero_seq_len - GHC_ZEROS_CNT_MIN));
                /* Update encoder "state". */
                encoder->pos_comp += 1;
                encoder->pos_unco += zero_seq_len;
            }
            pload_provision_idx = encoder->pos_unco;
        }
        else {
            ++pload_provision_idx;
            copy_seq_len = pload_provision_idx - encoder->pos_unco;
        }
    }
    return retval;
}
