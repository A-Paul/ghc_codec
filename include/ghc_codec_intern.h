#ifndef GHC_CODEC_INTERN_H
#define GHC_CODEC_INTERN_H

#include <inttypes.h>
#include <ghc.h>


/*!
 * Extract extension values.
 *
 * \param[out] na counter extension
 * \param[out] sa offset extension
 * \param[in]  extarg raw bytecode for set extension action.
 */
static inline void set_extensions(
    uint8_t* const na,
    uint8_t* const sa,
    uint8_t extarg)
{
           *na = (extarg & GHC_EXT_CNT_MASK)  >> GHC_EXT_CNT_RSHIFT;
           *sa = (extarg & GHC_EXT_OFFS_MASK) << GHC_EXT_OFFS_LSHIFT;
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
 * \param[inout]  na counter extension. Reset to zero after return.
 * \param[inout]  sa offset extension. Reset to zero after return.
 * \param[in]     extarg raw bytecode for set extension action.
 */
static inline void set_backrefs(
    uint8_t* const n,
    uint8_t* const s,
    uint8_t* const na,
    uint8_t* const sa,
    uint8_t backref)
{
           *n = ((backref & GHC_BREF_CNT_MASK) >> GHC_BREF_CNT_RSHIFT) +
                *na + GHC_BREF_CNT_ADD;
           *s = (backref & GHC_BREF_OFFS_MASK) + *sa + *n;
           *na = 0;
           *sa = 0;
}


/*!
 * Copy literal data bytes.
 * 
 * \param[inout]  target buffer.
 * \param[inout]  pos_target position index.
 * \param[inout]  pos_source position index.
 * \param[in]     source buffer.
 * \param[in]     n number of copied bytes.
 */
static inline void copy_literal(
    uint8_t*  const target,
    uint16_t* const pos_target,
    uint16_t* const pos_source,
    const uint8_t*  const source,
    uint8_t n)
{
    while(n) {
        ++(*pos_source);
        target[*pos_target] = source[*pos_source];
        ++(*pos_target);
        --n;
    }
}


/*!
 * Copy data bytes from dictionary.
 *
 * \param[inout]  target buffer.
 * \param[inout]  pos_target position index.
 * \param[in]     n number of copied bytes.
 * \param[in]     s backreference index.
 */
static inline void append_backreference(
    uint8_t*  const target,
    uint16_t* const pos_target,
    uint8_t n,
    uint8_t s)
{
    while(n) {
        target[*pos_target] = target[*pos_target - s];
        ++(*pos_target);
        --n;
    }
}


/*!
 * Append zero bytes.
 *
 * \param[inout]  target buffer.
 * \param[inout]  pos_target position index.
 * \param[in]     n number of copied bytes.
 */
static inline void append_zeros(
    uint8_t*  const target,
    uint16_t* const pos_target,
    uint8_t n)
{
    while(n) {
        target[*pos_target] = 0;
        ++(*pos_target);
        --n;
    }
}

#endif /* GHC_CODEC_INTERN_H */
