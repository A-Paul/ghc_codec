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
