/*
    COPY:
    > appending bytes to the reconstituted payload that are literally
    > given with the instruction in the compressed data

    ZEROS:
    > appending a given number of zero bytes to the reconstituted
    > payload

    EXT:
    > appending bytes to the reconstituted payload by copying a
    > contiguous sequence from the payload being reconstituted
    > ("backreferencing")

    BREF:
    > an ancillary instruction for setting up parameters for the
    > backreferencing instruction in "decompression variables"

    STOP:
    > a stop code (optional; see Section 3.2)
 */

#include <inttypes.h>

/*! MSB as mask, 0xxxxxxx */
#define GHC_COPY_BC             (0x00U)
#define GHC_COPY_MASK           (0x80U)
/*! Upper nibble as mask, 1001nnnn */
#define GHC_ZEROS_MASK          (0xF0U)
#define GHC_ZEROS_BC            (0x80U)

/*! Three MSBs as mask, 101nssss */
#define GHC_EXT_MASK            (0xE0U)
#define GHC_EXT_BC              (0xA0U)

#define GHC_EXT_CNT_MASK        (0x10U)
#define GHC_EXT_CNT_RSHIFT      (0x01U)
#define GHC_EXT_OFFS_MASK       (0x0FU)
#define GHC_EXT_OFFS_LSHIFT     (0x03U)

/*! Two MSBs as mask, 11nnnkkk */
#define GHC_BREF_MASK           (0xC0U)
#define GHC_BREF_BC             (0xC0U)

#define GHC_BREF_CNT_MASK       (0x38U)
#define GHC_BREF_CNT_RSHIFT     (0x03U)
#define GHC_BREF_CNT_ADD        (0x02U)
#define GHC_BREF_OFFS_MASK      (0x07U)

/*! 10010000 */

#define GHC_STOP_BC             (0x90U)

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