#ifndef GHC_CODEC_INTERN_H
#define GHC_CODEC_INTERN_H

#include <inttypes.h>
#include <ghc.h>


/*!
 * Extract extension values.
 *
 * \param[inout]  decoder.
 */
static inline void set_extensions(struct ghc_codec* decoder)
{
    decoder->na = (decoder->encoded[decoder->pos_enco] &
                  GHC_EXT_CNT_MASK)  >> GHC_EXT_CNT_RSHIFT;
    decoder->sa = (decoder->encoded[decoder->pos_enco] &
                  GHC_EXT_OFFS_MASK) << GHC_EXT_OFFS_LSHIFT;
}


/*!
 * Extract back reference values.
 *
 * Combines previous set extension variables na, sa and values
 * extracted from #backref byecode, according to <em>rfc7400, 2.
 * 6LoWPAN-GHC, Table 1</em>.
 * \em n and \em s are set to perform the backreference operation.
 *
 * \param[out]    n combined counter value
 * \param[out]    s combined offset value
 * \param[inout]  decoder.
 */
static inline void set_backrefs(
    uint8_t* const n, uint8_t* const s, struct ghc_codec* decoder)
{
    *n = ((decoder->encoded[decoder->pos_enco] & GHC_BREF_CNT_MASK) >>
         GHC_BREF_CNT_RSHIFT) + decoder->na + GHC_BREF_CNT_ADD;
    *s = (decoder->encoded[decoder->pos_enco] & GHC_BREF_OFFS_MASK) +
         decoder->sa + *n;
}


/*!
 * Copy literal data bytes.
 * Limits are not validated and have to be asserted before invocation.
 * 
 * \param[inout]  decoder.
 * \param[in]     n number of copied bytes.
 */
static inline void copy_literal( struct ghc_codec* decoder, uint8_t n)
{
    while(n) {
        ++(decoder->pos_enco);
        decoder->decoded[decoder->pos_deco] =
            decoder->encoded[decoder->pos_enco];
        ++(decoder->pos_deco);
        --n;
    }
}

/*!
 * Copy data bytes from dictionary.
 * Limits are not validated and have to be asserted before invocation.
 *
 * \param[inout]  decoder.
 * \param[in]     n number of copied bytes.
 * \param[in]     s backreference index.
 */
static inline void append_backreference(
    struct ghc_codec* decoder, uint8_t n, uint8_t s)
{
    while(n) {
        decoder->decoded[decoder->pos_deco] =
            decoder->decoded[decoder->pos_deco - s];
        ++(decoder->pos_deco);
        --n;
    }
    decoder->na = 0;
    decoder->sa = 0;
}


/*!
 * Append zero bytes.
 * Limits are not validated and have to be asserted before invocation.
 *
 * \param[inout]  decoder.
 * \param[in]     n number of copied zero bytes.
 */
static inline void append_zeros(struct ghc_codec* decoder, uint8_t n)
{
    while(n) {
        decoder->decoded[decoder->pos_deco] = 0x0U;
        ++(decoder->pos_deco);
        --n;
    }
}

#endif /* GHC_CODEC_INTERN_H */
