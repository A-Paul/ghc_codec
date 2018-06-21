/*!
 * \file
 *
 * - COPY:
 *   > appending bytes to the reconstituted payload that are literally
 *   > given with the instruction in the compressed data
 * 
 * - ZEROS:
 *   > appending a given number of zero bytes to the reconstituted
 *   > payload
 * 
 * - EXT:
 *   > appending bytes to the reconstituted payload by copying a
 *   > contiguous sequence from the payload being reconstituted
 *   > ("backreferencing")
 * 
 * - BREF:
 *   > an ancillary instruction for setting up parameters for the
 *   > backreferencing instruction in "decompression variables"
 * 
 * - STOP:
 *   > a stop code (optional; see Section 3.2)
 */

#include <inttypes.h>

/*! MSB as mask, 0xxxxxxx */
#define GHC_COPY_MASK           (0x80U)    /*!< Prefix mask for copy instruction */
#define GHC_COPY_BC             (0x00U)    /*!< Token prefix for copy instruction */

/*! Upper nibble as mask, 1001nnnn */
#define GHC_ZEROS_MASK          (0xF0U)    /*!< Prefix mask for append zero instruction */
#define GHC_ZEROS_BC            (0x80U)    /*!< Token prefix for append zero instruction */

/*! Three MSBs as mask, 101nssss */
#define GHC_EXT_MASK            (0xE0U)    /*!< Prefix mask for extension instruction */
#define GHC_EXT_BC              (0xA0U)    /*!< Token prefix for extension instruction */

#define GHC_EXT_CNT_MASK        (0x10U)    /*!< Mask for extension counter */
#define GHC_EXT_CNT_RSHIFT      (0x01U)    /*!< Shift for extension counter */
#define GHC_EXT_OFFS_MASK       (0x0FU)    /*!< Mask for extension offset */
#define GHC_EXT_OFFS_LSHIFT     (0x03U)    /*!< Shift for extension offset */

/*! Two MSBs as mask, 11nnnkkk */
#define GHC_BREF_MASK           (0xC0U)    /*!< Prefix mask for copy instruction */
#define GHC_BREF_BC             (0xC0U)    /*!< Token prefix for copy instruction */

#define GHC_BREF_CNT_MASK       (0x38U)    /*!< Mask for back reference counter */
#define GHC_BREF_CNT_RSHIFT     (0x03U)    /*!< Shift for back reference counter */
#define GHC_BREF_CNT_ADD        (0x02U)    /*!< Base value for back reference counter */
#define GHC_BREF_OFFS_MASK      (0x07U)    /*!< Mask for back reference offset */

/*! 10010000 */

#define GHC_STOP_BC             (0x90U)
