#if DEBUG == 1
#include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "ghc.h"
#include "ghc_codec.h"
#include "ghc_codec_intern.h"

int ghc_compress(struct ghc_coder* encoder)
{
    int retval = -1;
    (void)encoder->compressed;
    (void)encoder->size_comp;
    (void)encoder->uncompressed;
    (void)encoder->size_unco;

    return retval;
}


int ghc_decompress ( struct ghc_coder* decoder)
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

    decoder->pos_comp = 0;
    decoder->pos_unco = GHC_DICT_PRE_LEN;

    /*
     * Goes into ghc.h later
     */
#define GHC_COPY_CNT_MAX (0x60U)

    /*
     * Goes into ghc_codec_intern.h later
     */



    if ( NULL != decoder->compressed || NULL != decoder->uncompressed) {
        clean = 1;
    }

    while (clean) {

        if ( GHC_COPY_BC == (decoder->compressed[decoder->pos_comp] & GHC_COPY_MASK)) {
            /*
             * Assertions: preconditions
             * decoder->size_comp >= decoder->pos_comp + n + 1 (command byte)
             * decoder->size_unco >= decoder->pos_unco + n
             * n < 96
             */
            n = decoder->compressed[decoder->pos_comp] & GHC_COPY_CNT_MASK;
            /* Instead of checking array boundary override it can be done in
             *advance. */
            if (( decoder->size_comp >= decoder->pos_comp + n + 1U) &&
                ( decoder->size_unco >= decoder->pos_unco + n) &&
                ( n < GHC_COPY_CNT_MAX)) {
#if DEBUG == 1
                printf("X:%02i\n",decoder->compressed[decoder->pos_comp]);
#endif
                copy_literal(decoder, n);
            } else {
                clean = 0;
                retval = -30;
            }

            /*
             * postconditions
             * decoder->pos_comp += n + 1
             * decoder->pos_unco += n
             */
        }
        else if ( GHC_ZEROS_BC == (decoder->compressed[decoder->pos_comp] & GHC_ZEROS_MASK)) {
            /*
             * Assertions: preconditions
             * decoder->size_comp >= decoder->pos_comp + 1 (command byte)
             * decoder->size_unco >= decoder->pos_unco + n
             */
            n = (decoder->compressed[decoder->pos_comp] & GHC_ZEROS_CNT_MASK) + 2;
            if (decoder->pos_unco <= decoder->size_unco - n) {
                append_zeros(decoder->uncompressed, &decoder->pos_unco, n);
            } else {
                clean = 0;
                retval = -31;
            }
            /*
             * postconditions
             * decoder->pos_unco += n
             */
        }
        else if ( GHC_BREF_BC == (decoder->compressed[decoder->pos_comp] & GHC_BREF_MASK)) {
            set_backrefs(&n, &s, &decoder->na, &decoder->sa, decoder->compressed[decoder->pos_comp]);
            /*
             * Assertions: preconditions
             * decoder->size_unco >= decoder->pos_unco + n (number of copied bytes inside re-
             * maining free buffer)
             * s  <= pos_tartget (backref index inside dictionary)
             */
#if DEBUG == 1
            printf("pt:%05d:decoder->size_unco:%05lu:s:%05u:n:%05u\n",
                   decoder->pos_unco, decoder->size_unco, s, n);
#endif
            if ((decoder->pos_unco <= decoder->size_unco - n) && (s <= decoder->pos_unco)) {
                append_backreference(decoder->uncompressed, &decoder->pos_unco, n, s);
            } else {
                clean = 0;
                retval = -32;
            }
            /* postconditions
             * na := sa := 0
             */
            decoder->na = 0;
            decoder->sa = 0;
        }
        else if ( GHC_EXT_BC == (decoder->compressed[decoder->pos_comp] & GHC_EXT_MASK)) {
            set_extensions(&decoder->na, &decoder->sa,
                           decoder->compressed[decoder->pos_comp]);
        }
        else if ( GHC_STOP_BC == decoder->compressed[decoder->pos_comp]) {
            /*
             * Assertions: NN
             */
            clean = 0;
        } else {
            clean = 0;
            retval = -34;
        }
        /*
         * Assertions: 0 < decoder->size_comp - decoder->pos_comp
         * decoder->pos_comp
         */
#if DEBUG == 1
        printf("ps:%05d:pt:%05d:decoder->size_unco:%03lu:s:%03u:n:%03u\n",
               decoder->pos_comp, decoder->pos_unco, decoder->size_unco, s, n);
#endif
        if ( clean && (1 < (decoder->size_comp - decoder->pos_comp))) {
            retval = decoder->pos_comp;
            ++decoder->pos_comp;
        } else {
            clean = 0;
        }
    }

    return retval;
}
