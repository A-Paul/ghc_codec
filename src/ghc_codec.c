#if DEBUG == 1
#include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "ghc.h"
#include "ghc_codec.h"
#include "ghc_codec_intern.h"

int ghc_compress(uint8_t* source, size_t sn, uint8_t* target, size_t tn)
{
    int retval = -1;
    (void)source;
    (void)sn;
    (void)target;
    (void)tn;

    return retval;
}


int ghc_decompress ( uint8_t* source, size_t sn, uint8_t* target, size_t tn)
{
    int retval = -1;
    int clean  = 0;

    /* Backreference and extension values. Naming according to rfc7400
     * section 2
     */
    uint8_t na = 0;        /*!< Extension value for @n. */
    uint8_t sa = 0;        /*!< Extension value for @s. */
    uint8_t n  = 0;        /*!< Number of backreference bytes.*/
    uint8_t s  = 0;        /*!< Index of backreference start. */

    uint16_t pos_source = 0;
    uint16_t pos_target = GHC_DICT_PRE_LEN;

    /*
     * Goes into ghc.h later
     */
#define GHC_COPY_CNT_MASK (0x7FU)
#define GHC_ZEROS_CNT_MASK (0x0FU)

    /*
     * Goes into ghc_codec_intern.h later
     */



    if ( NULL != source || NULL != target) {
        clean = 1;
    }

    while (clean) {

        if ( GHC_COPY_BC == (source[pos_source] & GHC_COPY_MASK)) {
            /*
             * Assertions: preconditions
             * sn >= pos_source + n + 1 (command byte)
             * tn >= pos_target + n
             * n < 96
             */
            n = source[pos_source] & GHC_COPY_CNT_MASK;
            if (( sn >= pos_source + n + 1) &&
                ( tn >= pos_target + n) && n < 96) {
                //                printf("X:%02i\n",source[pos_source]);
                copy_literal(target, &pos_target, &pos_source, source, n);
            } else {
                clean = 0;
                retval = -30;
            }

            /*
             * postconditions
             * pos_source += n + 1
             * pos_target += n
             */
        }
        else if ( GHC_ZEROS_BC == (source[pos_source] & GHC_ZEROS_MASK)) {
            /*
             * Assertions: preconditions
             * sn >= pos_source + 1 (command byte)
             * tn >= pos_target + n
             */
            n = (source[pos_source] & GHC_ZEROS_CNT_MASK) + 2;
            if (pos_target <= tn - n) {
                append_zeros(target, &pos_target, n);
            } else {
                clean = 0;
                retval = -31;
            }
            /*
             * postconditions
             * pos_target += n
             */
        }
        else if ( GHC_BREF_BC == (source[pos_source] & GHC_BREF_MASK)) {
            set_backrefs(&n, &s, &na, &sa, source[pos_source]);
            /*
             * Assertions: preconditions
             * tn >= pos_target + n (number of copied bytes inside re-
             * maining free buffer)
             * s  <= pos_tartget (backref index inside dictionary)
             */
            printf("pt:%03u:tn:%03u:s:%03u:n:%03u\n", pos_target, tn,  s, n);
            if ((pos_target <= tn - n) && (s <= pos_target)) {
                append_backreference(target, &pos_target, n, s);
            } else {
                clean = 0;
                retval = -32;
            }
            /* postconditions
             * na := sa := 0
             */
            na = 0;
            sa = 0;
        }
        else if ( GHC_EXT_BC == (source[pos_source] & GHC_EXT_MASK)) {
            set_extensions(&na, &sa, source[pos_source]);
        }
        else if ( GHC_STOP_BC == source[pos_source]) {
            /*
             * Assertions: NN
             */
            clean = 0;
        } else {
            clean = 0;
            retval = -34;
        }
        /*
         * Assertions: 0 < sn - pos_source
         * pos_source
         */
#if DEBUG == 1
        printf("ps:%03d:pt:%03u:tn:%03u:s:%03u:n:%03u\n", pos_source, pos_target, tn, s, n);
#endif
        if ( clean && (1 < (sn - pos_source))) {
            retval = pos_source;
            ++pos_source;
        } else {
            clean = 0;
        }
    }

    return retval;
}
