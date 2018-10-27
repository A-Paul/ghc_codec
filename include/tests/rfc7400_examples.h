/*! \file */
#include <inttypes.h>
#include <stdio.h>

#define DICT_LEN (0x30U)

/*!
 * rfc7400 test/example case.
 *
 * Reference data collection contains pointers to byte arrays (uint8_t*)
 * with the data for:
 * - payload Reference for source of a encoding, target of a decoding.
 * - dictionary Pre filled lookup buffer.
 * - encoded Reference for target of a encoding, source of a decoding.
 */
struct ghc_case_ref {
    /*! @{ \name Array size data.
     *  Lengh fields for the correspondig arrays.
     */
    size_t   payload_len;       /*!< #payload array size */
    size_t   dictionary_len;    /*!< #dictionary array size */
    size_t   encoded_len;       /*!< #encoded array size */
    /*! @} */
    /*! @{ \name Test data array references. */
    uint8_t* payload;           /*!< Unencoded header payload */
    uint8_t* dictionary;        /*!< Predefined lookup buffer */
    uint8_t* encoded;           /*!< Compressed #payload. */
    /*! @} */
};

/*! \name rfc7400 test data
 * Byte arrays containing the data listed in the Example section of rfc7400.
 * For each example there is an array with the name template "ghc_expl_NN-SSSS"
 * with "NN" being the number of the figure describing the relatd case. And
 * "SSSS" stands for:
 * - "plod" Payload
 * - "dict" Dictionary
 * - "comp" Compressed
 * @{
 * \see https://tools.ietf.org/html/rfc7400#appendix-A
 */
/* RFC7400, Appendix A.  Examples, Figure 08 */
/* 8 bytes */
uint8_t ghc_expl_08_plod[] = {
    0x9b, 0x00, 0x6b, 0xde, 0x00, 0x00, 0x00, 0x00
};

/*  48 bytes */
uint8_t ghc_expl_08_dict[] = {
    0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x20, 0x24,
    0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/*  6 bytes */
uint8_t ghc_expl_08_byco[] = {
    0x04, 0x9b, 0x00, 0x6b, 0xde, 0x82
};


/* RFC7400, Appendix A.  Examples, Figure 09 */
/* 92 bytes */
uint8_t ghc_expl_09_plod[] = {
    0x9b, 0x01, 0x7a, 0x5f, 0x00, 0xf0, 0x01, 0x00, 0x88, 0x00, 0x00, 0x00, 0x20, 0x02, 0x0d, 0xb8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0xfa, 0xce, 0x04, 0x0e, 0x00, 0x14,
    0x09, 0xff, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1e, 0x80, 0x20,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x0d, 0xb8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0xfa, 0xce, 0x03, 0x0e, 0x40, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00
};

/* 48 bytes */
uint8_t ghc_expl_09_dict[] = {
    0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23,
    0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/* 52 bytes */
uint8_t ghc_expl_09_byco[] = {
    0x06, 0x9b, 0x01, 0x7a, 0x5f, 0x00, 0xf0, 0xc7, 0x01, 0x88, 0x81, 0x04, 0x20, 0x02, 0x0d, 0xb8,
    0x85, 0xa7, 0xc9, 0x08, 0xfa, 0xce, 0x04, 0x0e, 0x00, 0x14, 0x09, 0xff, 0xa4, 0xda, 0x83, 0x06,
    0x08, 0x1e, 0x80, 0x20, 0xff, 0xff, 0xc0, 0xd0, 0x82, 0xb4, 0xf0, 0x03, 0x03, 0x0e, 0x40, 0xc7,
    0xa3, 0xc9, 0xa2, 0xf0
};


/* RFC7400, Appendix A.  Examples, Figure 10 */
/* 50 bytes */
uint8_t ghc_expl_10_plod[] = {
    0x9b, 0x02, 0x58, 0x7d, 0x01, 0x80, 0x00, 0xf1, 0x05, 0x12, 0x00, 0x80, 0x20, 0x02, 0x0d, 0xb8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x33, 0x44, 0x06, 0x14, 0x00, 0x80,
    0xf1, 0x00, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00,
    0x11, 0x22
};

/* 48 bytes */
uint8_t ghc_expl_10_dict[] = {
    0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x33, 0x44,
    0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x11, 0x22,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/* 27 bytes */
uint8_t ghc_expl_10_byco[] = {
    0x0c, 0x9b, 0x02, 0x58, 0x7d, 0x01, 0x80, 0x00, 0xf1, 0x05, 0x12, 0x00, 0x80, 0xb5, 0xf4, 0x08,
    0x06, 0x14, 0x00, 0x80, 0xf1, 0x00, 0xfe, 0x80, 0x87, 0xa7, 0xdd
};


/* RFC7400, Appendix A.  Examples, Figure 11 */
/* 48 bytes */
uint8_t ghc_expl_11_plod[] = {
    0x87, 0x00, 0xa7, 0x68, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23, 0x01, 0x01, 0x3b, 0xd3, 0x00, 0x00, 0x00, 0x00,
    0x1f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x20, 0x24
};

/* 48 bytes */
uint8_t ghc_expl_11_dict[] = {
    0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x3b, 0xd3,
    0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/* 26 bytes */
uint8_t ghc_expl_11_byco[] = {
    0x04, 0x87, 0x00, 0xa7, 0x68, 0x82, 0xb3, 0xf0, 0x04, 0x01, 0x01, 0x3b, 0xd3, 0x82, 0x02, 0x1f,
    0x02, 0x83, 0x02, 0x06, 0x00, 0xa2, 0xdb, 0x02, 0x20, 0x24
};


/* RFC7400, Appendix A.  Examples, Figure 12 */
/* 48 bytes */
uint8_t ghc_expl_12_plod[] = {
    0x88, 0x00, 0x26, 0x6c, 0xc0, 0x00, 0x00, 0x00, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23, 0x02, 0x01, 0xfa, 0xce, 0x00, 0x00, 0x00, 0x00,
    0x1f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x20, 0x24
};

/* 48 bytes */
uint8_t ghc_expl_12_dict[] = {
    0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23,
    0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x3b, 0xd3,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/* 27 bytes */
uint8_t ghc_expl_12_byco[] = {
    0x05, 0x88, 0x00, 0x26, 0x6c, 0xc0, 0x81, 0xb5, 0xf0, 0x04, 0x02, 0x01, 0xfa, 0xce, 0x82, 0x02,
    0x1f, 0x02, 0x83, 0x02, 0x06, 0x00, 0xa2, 0xdb, 0x02, 0x20, 0x24
};


/* RFC7400, Appendix A.  Examples, Figure 13 */
/* 24 bytes */
uint8_t ghc_expl_13_plod[] = {
    0x85, 0x00, 0x90, 0x65, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xac, 0xde, 0x48, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* 48 bytes */
uint8_t ghc_expl_13_dict[] = {
    0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xae, 0xde, 0x48, 0x00, 0x00, 0x00, 0x00, 0x01,
    0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/* 12 bytes */
uint8_t ghc_expl_13_byco[] = {
   0x04, 0x85, 0x00, 0x90, 0x65, 0xde, 0x02, 0x02, 0xac, 0xa5, 0xeb, 0x84
};


/* RFC7400, Appendix A.  Examples, Figure 14 */
/* 96 bytes */
uint8_t ghc_expl_14_plod[] = {
    0x86, 0x00, 0x55, 0xc9, 0x40, 0x00, 0x0f, 0xa0, 0x1c, 0x5a, 0x38, 0x17, 0x00, 0x00, 0x07, 0xd0,
    0x01, 0x01, 0x11, 0x22, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x40, 0x40, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x40, 0x10, 0x00, 0x00, 0x03, 0xe8,
    0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x21, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x11, 0x22
};

/* 48 bytes */
uint8_t ghc_expl_14_dict[] = {
    0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x34, 0x00, 0xff, 0xfe, 0x00, 0x11, 0x22,
    0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xae, 0xde, 0x48, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/* 58 bytes */
uint8_t ghc_expl_14_byco[] = {
    0x0c, 0x86, 0x00, 0x55, 0xc9, 0x40, 0x00, 0x0f, 0xa0, 0x1c, 0x5a, 0x38, 0x17, 0x80, 0x06, 0x07,
    0xd0, 0x01, 0x01, 0x11, 0x22, 0x82, 0x06, 0x03, 0x04, 0x40, 0x40, 0xff, 0xff, 0xc0, 0xd0, 0x82,
    0x04, 0x20, 0x02, 0x0d, 0xb8, 0x8a, 0x04, 0x20, 0x02, 0x40, 0x10, 0xa4, 0xcb, 0x01, 0xe8, 0xa2,
    0xf0, 0x02, 0x21, 0x03, 0xa9, 0xe6, 0xb3, 0xcd, 0xaf, 0xdb
};


/* RFC7400, Appendix A.  Examples, Figure 15 */
/* 42 bytes */
uint8_t ghc_expl_15_plod[] = {
    0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1d, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x09, 0xb2, 0x0e, 0x82, 0xc1, 0x6e, 0xb6, 0x96, 0xc5, 0x1f, 0x36,
    0x8d, 0x17, 0x61, 0xe2, 0xb5, 0xd4, 0x22, 0xd4, 0xed, 0x2b
};

/* 48 bytes */
uint8_t ghc_expl_15_dict[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/* 27 bytes */
uint8_t ghc_expl_15_byco[] = {
    0xb0, 0xd1, 0x01, 0x1d, 0xf2, 0x15, 0x09, 0xb2, 0x0e, 0x82, 0xc1, 0x6e, 0xb6, 0x96, 0xc5, 0x1f,
    0x36, 0x8d, 0x17, 0x61, 0xe2, 0xb5, 0xd4, 0x22, 0xd4, 0xed, 0x2b
};


/* RFC7400, Appendix A.  Examples, Figure 16 */
/* 35 bytes */
uint8_t ghc_expl_16_plod[] = {
    0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x16, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0xae, 0xa0, 0x15, 0x56, 0x67, 0x92, 0x4d, 0xff, 0x8a, 0x24, 0xe4,
    0xcb, 0x35, 0xb9
};

/* 48 bytes */
uint8_t ghc_expl_16_dict[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/* 22 bytes */
uint8_t ghc_expl_16_byco[] = {
    0xb0, 0xc3, 0x03, 0x05, 0x00, 0x16, 0xf2, 0x0e, 0xae, 0xa0, 0x15, 0x56, 0x67, 0x92, 0x4d, 0xff,
    0x8a, 0x24, 0xe4, 0xcb, 0x35, 0xb9
};


/* RFC7400, Appendix A.  Examples, Figure 17 */
/* 67 bytes */
uint8_t ghc_expl_17_plod[] = {
    0x16, 0xfe, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x01, 0x00, 0x00,
    0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xfe, 0xfd, 0x51, 0x52, 0xed, 0x79, 0xa4,
    0x20, 0xc9, 0x62, 0x56, 0x11, 0x47, 0xc9, 0x39, 0xee, 0x6c, 0xc0, 0xa4, 0xfe, 0xc6, 0x89, 0x2f,
    0x32, 0x26, 0x9a, 0x16, 0x4e, 0x31, 0x7e, 0x9f, 0x20, 0x92, 0x92, 0x00, 0x00, 0x00, 0x02, 0xc0,
    0xa8, 0x01, 0x00
};

/* 48 bytes */
uint8_t ghc_expl_17_dict[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

/* 53 bytes */
uint8_t ghc_expl_17_byco[] = {
    0xa1, 0xcd, 0x87, 0x01, 0x36, 0xa1, 0xcd, 0x01, 0x2a, 0x85, 0x23, 0x2a, 0xfe, 0xfd, 0x51, 0x52,
    0xed, 0x79, 0xa4, 0x20, 0xc9, 0x62, 0x56, 0x11, 0x47, 0xc9, 0x39, 0xee, 0x6c, 0xc0, 0xa4, 0xfe,
    0xc6, 0x89, 0x2f, 0x32, 0x26, 0x9a, 0x16, 0x4e, 0x31, 0x7e, 0x9f, 0x20, 0x92, 0x92, 0x81, 0x05,
    0x02, 0xc0, 0xa8, 0x01, 0x00
};
/*! @} */ /* End rfc7400 test data */
