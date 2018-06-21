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