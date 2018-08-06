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


static inline int8_t comp_not_full(struct ghc_coder* encoder)
{
    return (encoder->pos_comp < encoder->size_comp);
}

static inline int8_t payload_not_empty(struct ghc_coder* encoder)
{
    return (encoder->pos_unco < encoder->size_unco);
}


/*! Return the count of zero byte prefix in actual payload.
 */
static inline uint32_t get_zeros_len(
    struct ghc_coder* encoder, uint8_t* zeros, uint16_t pos_plod)
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
static inline uint32_t get_match_len( struct ghc_coder* encoder,
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
static inline uint32_t get_longest_match(
    struct ghc_coder* encoder, uint16_t pload_idx,
    uint16_t* dict_idx, uint16_t* max_len)
{
    *dict_idx = 0;
    *max_len = 0;
    uint16_t dict_pivot;
    uint16_t match_len;
    uint16_t temp_dict_suffix_len;
    uint16_t match_max_len = encoder->size_unco - pload_idx;

    for( dict_pivot = 0; dict_pivot < (pload_idx - 2); ++dict_pivot) {

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
#if DEBUG == 5
        printf("gt_lo_ma pload_idx: %03d dict_pivot: %03d "
            "match_max_len: %03d match_len: %03d max_len: %03d\n",
            pload_idx, dict_pivot, match_max_len, match_len, *max_len);
#endif
        if(match_len >= *max_len) { /* Prefer *same* lengths with smaller indices */
            *max_len = match_len;
            *dict_idx = dict_pivot;
        }
    }
    return 1;
}


static inline void write_bref_code_0(struct ghc_coder* encoder,
    uint16_t longest_match_len, uint16_t longest_match_offs)
{
    uint16_t match_len = longest_match_len - GHC_BREF_CNT_ADD;
    uint16_t match_offs = longest_match_offs - match_len;
    uint8_t bref_bc = //GHC_BREF_BC |
        ((match_len & BREF_BC_LEN_MASK) << GHC_BREF_CNT_RSHIFT) |
        ((match_offs & GHC_BREF_OFFS_MASK));
    printf("A: %03d, B: %03d, C: %03d\n", match_len, match_offs, bref_bc);
    /* Mask out used bits */
    match_len &= ~BREF_BC_LEN_MASK;
    match_offs &= ~GHC_BREF_OFFS_MASK;

    uint8_t ext_len_repeat_count = match_len / EXT_BC_LEN_MASK;
    uint8_t ext_offs_repeat_count = match_offs / EXT_BC_OFFS_MASK;
    uint8_t ext_len_remain = match_len & EXT_BC_LEN_MASK;
    uint8_t ext_offs_remain = match_offs & EXT_BC_OFFS_MASK;

    printf("lo_ma match_len: 0x%03x match_offs: 0x%03x bref_bc: 0x%03x\n"
            "ext_len_repeat_count: 0x%02x ext_offs_repeat_count: 0x%02x\n"
            "ext_len_remain: 0x%02x ext_offs_remain: 0x%02x\n",
            match_len, match_offs, bref_bc, ext_len_repeat_count, ext_offs_repeat_count,
            ext_len_remain, ext_offs_remain);
}

static inline void write_bref_code_1(struct ghc_coder* encoder,
    uint16_t longest_match_len, uint16_t longest_match_offs)
{
    uint16_t match_len = longest_match_len - GHC_BREF_CNT_ADD;
    uint16_t match_offs = longest_match_offs - longest_match_len;
    uint8_t  bref_bc = 0;

    printf("Func: %s, line: %d, match_len: 0x%02X match_offs: 0x%02X longest_match_offs: 0x%02X\n",
            __func__, __LINE__, match_len, match_offs, longest_match_offs);

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

    printf("Func: %s, line: %d, bref_bc: 0x%02X\n", __func__, __LINE__, bref_bc);

    bref_bc = GHC_EXT_BC |
              ((match_len & EXT_BC_LEN_MASK) << GHC_EXT_CNT_RSHIFT) |
              ((match_offs & EXT_BC_OFFS_MASK) >> GHC_EXT_OFFS_LSHIFT);

    if (bref_bc != GHC_EXT_BC) {
        encoder->compressed[encoder->pos_comp] = bref_bc;
        ++encoder->pos_comp;
    }

    printf("Func: %s, line: %d, bref_bc: 0x%02X\n", __func__, __LINE__, bref_bc);

    bref_bc = GHC_BREF_BC |
              ((match_len & BREF_BC_LEN_MASK) << GHC_BREF_CNT_RSHIFT) |
              ((match_offs & BREF_BC_OFFS_MASK) >> GHC_BREF_OFFS_LSHIFT);

    if (bref_bc != GHC_BREF_BC) {
        encoder->compressed[encoder->pos_comp] = bref_bc;
        ++encoder->pos_comp;
    }

    printf("Func: %s, line: %d, bref_bc: 0x%02X\n", __func__, __LINE__, bref_bc);
}

/*
 * encoder.pos_unco points at the first uncompressed entry
 * encoder.pos_comp points at the first uncompressed entry
 */
int ghc_compress(struct ghc_coder* encoder)
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
    SHOW_GHC_CODER(encoder);

    while (clean && comp_not_full(encoder) && payload_not_empty(encoder))
    {
        longest_match_idx = pload_provision_idx;
        longest_match_len = 0;

        /* Look for sequences */
        get_zeros_len(encoder, &zero_seq_len, pload_provision_idx);
        get_longest_match(encoder, pload_provision_idx,
            &longest_match_idx, &longest_match_len);

#if DEBUG == 3
        printf("lo_ma longest_match_idx: 0x%03x longest_match_len: 0x%03x zero_len: 0x%03x\n",
            longest_match_idx, longest_match_len, zero_seq_len);
#endif

        if (1U < zero_seq_len || 1U < longest_match_len) {
            /* copy_provision(encoder, pload_provision_idx); */
            printf("Func: %s, line: %d, zero_seq_len: 0x%02X longest_match_len: 0x%02X\n",
                    __func__, __LINE__, zero_seq_len, longest_match_len);
            copy_seq_len = pload_provision_idx - encoder->pos_unco;
            if (copy_seq_len) {
                encoder->compressed[encoder->pos_comp] = GHC_COPY_BC |
                    (copy_seq_len & GHC_COPY_CNT_MASK);
                ++(encoder->pos_comp);
                while (copy_seq_len) {
                    encoder->compressed[encoder->pos_comp] =
                        encoder->uncompressed[encoder->pos_unco];
                    ++(encoder->pos_comp);
                    ++(encoder->pos_unco);
                    --copy_seq_len;
                }
            }

            if(longest_match_len > zero_seq_len) {

                write_bref_code_1(encoder, longest_match_len,
                    pload_provision_idx - longest_match_idx);

                //break;

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
        }
        puts("...\n");
        SHOW_GHC_CODER(encoder);
        clean = 1;
    }
    return retval;
}
