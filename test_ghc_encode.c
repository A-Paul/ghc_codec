#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#include "ghc.h"
#include "ghc_codec.h"

#include "tests/rfc7400_examples_testsuite.h"

#define RFC_EXAMPLES_PAYLOAD_MAX (0x60U)
#define RFC_EXAMPLES_COMPRESSED_MAX (RFC_EXAMPLES_PAYLOAD_MAX + \
            RFC_EXAMPLES_PAYLOAD_MAX % GHC_COPY_CNT_MAX + 1)
#define RFC7400_EXAMPLES_FIRST (8U)

#define PRINT_ALL (1)

/*! Copy content of seq to the compressed of ghcc.
 * \param[inout] ghcc struct with member compressed as target.
 * \param[in]    seq array which is copied
 */
static inline int32_t ghcc_set_seq_compressed(struct ghc_coder* ghcc, uint8_t* seq) {
    uint32_t i;
    for (i = 0; i < ghcc->size_comp; ++i) {
            ghcc->compressed[i] = seq[i];
    }
    return i;
}

/*! Copy content of seq to the payload of ghcc.
 * \param[inout] ghcc struct with member uncompressed as target.
 * \param[in]    seq array which is copied
 */
static inline int32_t ghcc_set_seq_uncompressed(struct ghc_coder* ghcc, uint8_t* seq) {
    uint32_t i = 0;
    uint32_t j = 0;
    for (i = GHC_DICT_PRE_LEN, j = 0; i < ghcc->size_unco; ++i, ++j) {
            ghcc->uncompressed[i] = seq[j];
    }
    return i;
}

/*! Copy 48 bytes from seq to the predefined dictionary of ghcc.
 * \param[inout] ghcc struct with member uncompressed as target.
 * \param[in]    seq array which is copied
 */
static inline int32_t ghcc_set_seq_dict_pre(struct ghc_coder* ghcc, uint8_t* seq) {
    uint32_t i;
    for (i = 0; i < GHC_DICT_PRE_LEN; ++i) {
            ghcc->uncompressed[i] = seq[i];
    }
    return i;
}

/*! Set all elements of ghcc's compressed to value of val.
 * \param[inout] ghcc struct with member uncompressed as target.
 * \param[in]    val value to be set
 */
static inline int32_t ghcc_set_val_compressed(struct ghc_coder* ghcc, uint8_t val) {
    uint32_t i;
    for (i = 0; i < ghcc->size_comp; ++i) {
            ghcc->compressed[i] = val;
    }
    return i;
}

/*! Set all elements of ghcc's payload to value of val.
 * \param[inout] ghcc struct with member uncompressed as target.
 * \param[in]    val value to be set
 */
static inline int32_t ghcc_set_val_uncompressed(struct ghc_coder* ghcc, uint8_t val) {
    uint32_t i;
    for (i = GHC_DICT_PRE_LEN; i < ghcc->size_unco; ++i) {
            ghcc->uncompressed[i] = val;
    }
    return i;
}

int main (void)
{

    int32_t result;
    uint32_t i;
    uint8_t num_cases = (sizeof ghc_suite_case_refs)/
                         (sizeof ghc_suite_case_refs[0]);

    uint8_t min_case = 0;
    //num_cases = min_case + 1;
    uint8_t suite_case = 0;
    uint8_t comp_val1 = 0;
    uint8_t comp_val2 = 0;


    uint8_t test_plod[GHC_DICT_PRE_LEN + RFC_EXAMPLES_PAYLOAD_MAX];
    uint8_t test_comp[RFC_EXAMPLES_COMPRESSED_MAX];

    struct ghc_coder test_decoder;

    test_decoder.uncompressed = test_plod;
    test_decoder.compressed = test_comp;

    test_decoder.na = 0;        /*!< Extension value for @n. */
    test_decoder.sa = 0;        /*!< Extension value for @s. */

    for ( suite_case = min_case; suite_case < num_cases; ++suite_case) {

        /* Set metadata.
         * size_comp is sized to carry the "copy only" worst case.
         */
        i = ghc_suite_case_refs[suite_case].payload_len;
        test_decoder.size_comp = i + i / GHC_BREF_CNT_VALUE_MAX + 1;
        test_decoder.size_unco = i + ghc_suite_case_refs[suite_case].dictionary_len;

        test_decoder.pos_comp = 0;
        test_decoder.pos_unco = GHC_DICT_PRE_LEN;

        /* Set dictionary */
        ghcc_set_seq_dict_pre(&test_decoder, ghc_suite_case_refs[suite_case].dictionary);

        /* Prepare compressor */
        ghcc_set_seq_uncompressed(&test_decoder, ghc_suite_case_refs[suite_case].payload);
        ghcc_set_val_compressed(&test_decoder, 0);

        printf("Check compressiing of RFC7400 example %02"PRIu8":\n"
               "ghc_encode(ghc_suite_case_refs[%02"PRIu8"])\n"
               "[size_comp:%03u],[size_unco:%03u]\n",
               RFC7400_EXAMPLES_FIRST + suite_case, suite_case,
               test_decoder.size_comp, test_decoder.size_unco - GHC_DICT_PRE_LEN);
        result = ghc_encode(&test_decoder);
        printf("\nreturned with %03"PRId32"\n", result);
        //SHOW_GHC_CODER((&test_decoder));

        printf("!comp_size_diff: %u\n", ghc_suite_case_refs[suite_case].compressed_len - test_decoder.pos_comp);
        for (i=0; i < test_decoder.pos_comp; ++i) {
            comp_val1 = ghc_suite_case_refs[suite_case].compressed[i];
            comp_val2 = test_decoder.compressed[i];
            if (comp_val1 != comp_val2) {
                printf("%03"PRId32":0x%02X:0x%02X,%c", i,
                     comp_val1, comp_val2,
                    (((i+1) & 0x7) ? ' ' : '\n'));
            }
            else if (PRINT_ALL) {
                printf("%03"PRIu32":0x%02X,     %c", i,
                     comp_val1,
                    (((i+1) & 0x7) ? ' ' : '\n'));
            }
        }
        puts("\n");

        /* Prepare decompressor */
        test_decoder.pos_comp = 0;
        test_decoder.pos_unco = GHC_DICT_PRE_LEN;
        ghcc_set_val_uncompressed(&test_decoder, 0);

        printf("Check decompressiing of RFC7400 example %02i:\n"
               "ghc_decode(ghc_suite_case_refs[%02i])\n",
               RFC7400_EXAMPLES_FIRST + suite_case, suite_case);
        result = ghc_decode(&test_decoder);
        printf("\nreturned with %03"PRId32"\n", result);
        //SHOW_GHC_CODER((&test_decoder));

        for (i=0; i < ghc_suite_case_refs[suite_case].payload_len; ++i) {
            comp_val1 = ghc_suite_case_refs[suite_case].payload[i];
            comp_val2 = test_decoder.uncompressed[GHC_DICT_PRE_LEN + i];
            if (comp_val1 != comp_val2) {
                printf("%03"PRIu32":0x%02X:0x%02X,%c", i,
                     comp_val1, comp_val2,
                    (((i+1) & 0x7) ? ' ' : '\n'));
            }
            else if (PRINT_ALL) {
                printf("%03"PRIu32":0x%02X,     %c", i,
                     comp_val1,
                    (((i+1) & 0x7) ? ' ' : '\n'));
            }
        }
        puts("\n");
    }
    return EXIT_SUCCESS;
}
