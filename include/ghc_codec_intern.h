#ifndef GHC_CODEC_INTERN
#define GHC_CODEC_INTERN

#include <inttypes.h>
#include <ghc.h>


/*!
 * Extract extension values.
 *
 * \param[out] na counter extension
 * \param[out] sa offset extension
 * \param[in]  extarg raw bytecode for set extension action.
 */
inline void set_extensions(
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
inline void set_backrefs(
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
 * 
 */
inline uint8_t compose_count(
    uint8_t backref,
    uint8_t extarg)
{
    return ((backref & GHC_BREF_CNT_MASK) >> GHC_BREF_CNT_RSHIFT) +
           ((extarg  & GHC_EXT_CNT_MASK)  >> GHC_EXT_CNT_RSHIFT) +
             GHC_BREF_CNT_ADD;
}


inline uint8_t compose_offset(
    uint8_t backref,
    uint8_t extarg,
    uint8_t count)
{
    return (backref & GHC_BREF_OFFS_MASK) +
           ((extarg & GHC_EXT_OFFS_MASK)  << GHC_EXT_OFFS_LSHIFT) +
             count;
}

#endif /* GHC_CODEC_INTERN */
