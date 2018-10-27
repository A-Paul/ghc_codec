#if DEBUG > 0
#include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "ghc.h"
#include "ghc_codec.h"
#include "ghc_codec_intern.h"


int ghc_decode ( struct ghc_codec* decoder)
{
    int retval = -1;
    int clean  = 0;

    /* Backreference and extension values. Naming according to rfc7400
     * section 2
     */
    decoder->na = 0;        /*!< Extension value for @n. */
    decoder->sa = 0;        /*!< Extension value for @s. */
    uint8_t n  = 0;         /*!< Number of (backreference) bytes.*/
    uint8_t s  = 0;         /*!< Index of backreference start. */

    decoder->pos_enco = 0;
    decoder->pos_deco = GHC_DICT_PRE_LEN;

    /*
     * Goes into ghc.h later
     */

    /*
     * Goes into ghc_codec_intern.h later
     */



    if ( NULL != decoder->encoded || NULL != decoder->decoded) {
        clean = 1;
    }

    while (clean) {

        if ( GHC_COPY_BC == (decoder->encoded[decoder->pos_enco] & GHC_COPY_MASK)) {
            /*
             * Assertions: preconditions
             * decoder->size_enco >= decoder->pos_enco + n + 1 (command byte)
             * decoder->size_deco >= decoder->pos_deco + n
             * n < 96
             */
            n = decoder->encoded[decoder->pos_enco] & GHC_COPY_CNT_MASK;
            /* Instead of checking array boundary override it can be done in
             *advance. */
            if (( decoder->size_enco >= decoder->pos_enco + n + 1U) &&
                ( decoder->size_deco >= decoder->pos_deco + n) &&
                ( n < (GHC_COPY_CNT_MAX + 1))) {
#if DEBUG >= 2
                printf("X:%02i\n",decoder->encoded[decoder->pos_enco]);
#endif
                copy_literal(decoder, n);
            } else {
                clean = 0;
                retval = -30;
            }

            /*
             * postconditions
             * decoder->pos_enco += n + 1
             * decoder->pos_deco += n
             */
        }
        else if ( GHC_ZEROS_BC == (decoder->encoded[decoder->pos_enco] & GHC_ZEROS_MASK)) {
            /*
             * Assertions: preconditions
             * decoder->size_enco >= decoder->pos_enco + 1 (command byte)
             * decoder->size_deco >= decoder->pos_deco + n
             */
            n = (decoder->encoded[decoder->pos_enco] & GHC_ZEROS_CNT_MASK) + 2;
            if (decoder->pos_deco <= decoder->size_deco - n) {
                append_zeros(decoder, n);
            } else {
                clean = 0;
                retval = -31;
            }
            /*
             * postconditions
             * decoder->pos_deco += n
             */
        }
        else if ( GHC_BREF_BC == (decoder->encoded[decoder->pos_enco] & GHC_BREF_MASK)) {
            set_backrefs(&n, &s, decoder);
            /*
             * Assertions: preconditions
             * decoder->size_deco >= decoder->pos_deco + n (number of copied bytes inside re-
             * maining free buffer)
             * s  <= pos_tartget (backref index inside dictionary)
             */
#if DEBUG >= 1
            printf("pt:%05d:decoder->size_deco:%05lu:s:%05u:n:%05u\n",
                   decoder->pos_deco, decoder->size_deco, s, n);
#endif
            if ((decoder->pos_deco <= decoder->size_deco - n) && (s <= decoder->pos_deco)) {
                append_backreference(decoder, n, s);
            } else {
                clean = 0;
                retval = -32;
            }
            /* postconditions
             * na := sa := 0
             */
        }
        else if ( GHC_EXT_BC == (decoder->encoded[decoder->pos_enco] & GHC_EXT_MASK)) {
            set_extensions(decoder);
        }
        else if ( GHC_STOP_BC == decoder->encoded[decoder->pos_enco]) {
            /*
             * Assertions: NN
             */
            clean = 0;
        } else {
            clean = 0;
            retval = -34;
        }
        /*
         * Assertions: 0 < decoder->size_enco - decoder->pos_enco
         * decoder->pos_enco
         */
#if DEBUG >= 1
        printf("ps:%05d:pt:%05d:decoder->size_deco:%03lu:s:%03u:n:%03u\n",
               decoder->pos_enco, decoder->pos_deco, decoder->size_deco, s, n);
#endif
        if ( clean && (1 < (decoder->size_enco - decoder->pos_enco))) {
            retval = decoder->pos_enco;
            ++decoder->pos_enco;
        } else {
            clean = 0;
        }
    }

    return retval;
}
