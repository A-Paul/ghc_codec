#ifndef GHC_CODEC_H
#define GHC_CODEC_H

#include "stdint.h"

/*
 * Draft for one/two struct(s) carrying the buffers and meta data
 * of (de)compressor.
 *
 */
struct ghc_coder {
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
 * \param[in]    source uncompressed byte array.
 * \param[in]    sn number of elements in source.
 * \param[inout] target compressed byte array.
 * \param[inout] tn number of elements in target.
 */
int ghc_compress (struct ghc_coder* encoder);


/*!
 * \param[in]    source compressed bytecode.
 * \param[in]    sn number of elements in source.
 * \param[inout] target uncompressed byte array.
 * \param[inout] tn number of elements in target.
 */
int ghc_decompress (struct ghc_coder* decoder);

#endif /* GHC_CODEC_H */
