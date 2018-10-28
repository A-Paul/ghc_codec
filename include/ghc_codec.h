#ifndef GHC_CODEC_H
#define GHC_CODEC_H

#include "stdint.h"


#define SHOW_GHC_CODER(x) printf( "size_deco:%03lu,size_enco:%03lu,pos_deco:%03u,"\
    "pos_enco:%03u, ns:%03u, sa:%03u\n",\
    x->size_deco, x->size_enco, x->pos_deco, x->pos_enco, x->na, x->sa )

/*!
 * Carry the buffers and meta data of de/encoder.
 *
 */
struct ghc_codec{
    /* uint32_t status; */ /*  */
    uint8_t *decoded;      /*!< dictionary + payload buffer */
    uint8_t *encoded;      /*!< GHC bytecode buffer */
    size_t size_deco;      /*!< payload size */
    size_t size_enco;      /*!< bytecode size */
    uint16_t pos_deco;     /*!< payload  cursor */
    uint16_t pos_enco;     /*!< bytecode cursor */
    uint8_t na;            /*!< length extension */
    uint8_t sa;            /*!< index extension */
};

/*!
 * Encode a sequence of bytes with Generic Header Compression.
 *
 * \param[inout] encoder struct holding the buffers and state and meta data.
 */
int ghc_encode(struct ghc_codec* encoder);


/*!
 * Decode a Generic Header Compression bytecode to the original payload.
 *
 * \param[inout] decoder struct holding the buffers and state and meta data.
 */
int ghc_decode(struct ghc_codec* decoder);

#endif /* GHC_CODEC_H */
