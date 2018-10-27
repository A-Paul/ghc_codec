#ifndef GHC_CODEC_H
#define GHC_CODEC_H

#include "stdint.h"


#define SHOW_GHC_CODER(x) printf( "size_unco:%03lu,size_comp:%03lu,pos_unco:%03u,"\
    "pos_comp:%03u, ns:%03u, sa:%03u\n",\
    x->size_unco, x->size_comp, x->pos_unco, x->pos_comp, x->na, x->sa )

/*
 * Draft for one/two struct(s) carrying the buffers and meta data
 * of (de)compressor.
 *
 */
struct ghc_codec {
  /* uint32_t status; */       /*  */
  uint8_t* uncompressed;       /* source */
  uint8_t* compressed;         /* target */
  size_t   size_unco;	       /*  */
  size_t   size_comp;	       /*  */
  uint16_t pos_unco;	       /*  */
  uint16_t pos_comp;	       /*  */
  uint8_t  na;		       /*  */
  uint8_t  sa;		       /*  */
};

/*!
 * \param[inout] encoder struct holding the buffers and state and meta data.
 */
int ghc_encode (struct ghc_codec* encoder);


/*!
 * \param[inout] decoder struct holding the buffers and state and meta data.
 */
int ghc_decode (struct ghc_codec* decoder);

#endif /* GHC_CODEC_H */
