#include <stdint.h>
#include <stdlib.h>

#include "ghc.h"
#include "ghc_codec.h"

static inline void copy_literal( uint8_t* source, uint8_t* target) {

}

int ghc_compress ( uint8_t* source, size_t sn, uint8_t* target, size_t tn)
{
    int retval = -1;

    return retval;
}


int ghc_decompress ( uint8_t* source, size_t sn, uint8_t* target, size_t tn)
{
    int retval = -1;
    int clean = 0;

    uint8_t na = 0;
    uint8_t sa = 0;
    uint8_t n  = 0;
    uint8_t s  = 0;

    uint16_t pos_source = GHC_PRE_DICT_SIZE;
    uint16_t pos_target = GHC_PRE_DICT_SIZE;

    if ( NULL != source || NULL != target) {
        clean = 1;
    }

    while (clean) {

        if ( GHC_COPY_BC == (source[pos_source] & GHC_COPY_MASK)) {
            copy_literal( source, target, n);
        }
        else if ( GHC_ZEROS_BC == (source[pos_source] & GHC_ZEROS_MASK)) {
            append_zeros;
        }
        else if ( GHC_BREF_BC == (source[pos_source] & GHC_BREF_MASK)) {
            copy_backref;
        }
        else if ( GHC_EXT_BC == (source[pos_source] & GHC_EXT_MASK)) {
            set_extended;
        }
    }

    return retval;
}
