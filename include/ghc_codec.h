#ifndef GHC_CODEC_H
#define GHC_CODEC_H

#include "stdint.h"


#define SHOW_GHC_CODER(x) printf( "size_deco:%03lu,size_enco:%03lu,pos_deco:%03u,"\
    "pos_enco:%03u, ns:%03u, sa:%03u\n",\
    x->size_deco, x->size_enco, x->pos_deco, x->pos_enco, x->na, x->sa )

/*
 * Draft for one/two struct(s) carrying the buffers and meta data
 * of de/encoder.
 *
 */
struct ghc_codec {
  /* uint32_t status; */       /*  */
  uint8_t* decoded;       /* source */
  uint8_t* encoded;         /* target */
  size_t   size_deco;	       /*  */
  size_t   size_enco;	       /*  */
  uint16_t pos_deco;	       /*  */
  uint16_t pos_enco;	       /*  */
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
