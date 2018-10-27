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

/*! Copy content of seq to the encoded of ghcc.
 * \param[inout] ghcc struct with member encoded as target.
 * \param[in]    seq array which is copied
 */
static inline int32_t ghcc_set_seq_encoressed(struct ghc_codec* ghcc, uint8_t* seq) {
    uint32_t i;
    for (i = 0; i < ghcc->size_enco; ++i) {
            ghcc->encoded[i] = seq[i];
    }
    return i;
}

/*! Copy content of seq to the payload of ghcc.
 * \param[inout] ghcc struct with member decoded as target.
 * \param[in]    seq array which is copied
 */
static inline int32_t ghcc_set_seq_decoded(struct ghc_codec* ghcc, uint8_t* seq) {
    uint32_t i = 0;
    uint32_t j = 0;
    for (i = GHC_DICT_PRE_LEN, j = 0; i < ghcc->size_deco; ++i, ++j) {
            ghcc->decoded[i] = seq[j];
    }
    return i;
}

/*! Copy 48 bytes from seq to the predefined dictionary of ghcc.
 * \param[inout] ghcc struct with member decoded as target.
 * \param[in]    seq array which is copied
 */
static inline int32_t ghcc_set_seq_dict_pre(struct ghc_codec* ghcc, uint8_t* seq) {
    uint32_t i;
    for (i = 0; i < GHC_DICT_PRE_LEN; ++i) {
            ghcc->decoded[i] = seq[i];
    }
    return i;
}

/*! Set all elements of ghcc's encoded to value of val.
 * \param[inout] ghcc struct with member decoded as target.
 * \param[in]    val value to be set
 */
static inline int32_t ghcc_set_val_encoressed(struct ghc_codec* ghcc, uint8_t val) {
    uint32_t i;
    for (i = 0; i < ghcc->size_enco; ++i) {
            ghcc->encoded[i] = val;
    }
    return i;
}

/*! Set all elements of ghcc's payload to value of val.
 * \param[inout] ghcc struct with member decoded as target.
 * \param[in]    val value to be set
 */
static inline int32_t ghcc_set_val_decoded(struct ghc_codec* ghcc, uint8_t val) {
    uint32_t i;
    for (i = GHC_DICT_PRE_LEN; i < ghcc->size_deco; ++i) {
            ghcc->decoded[i] = val;
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
    uint8_t test_enco[RFC_EXAMPLES_COMPRESSED_MAX];

    struct ghc_codec test_decoder;

    test_decoder.decoded = test_plod;
    test_decoder.encoded = test_enco;

    test_decoder.na = 0;        /*!< Extension value for @n. */
    test_decoder.sa = 0;        /*!< Extension value for @s. */

    for ( suite_case = min_case; suite_case < num_cases; ++suite_case) {

        /* Set metadata.
         * size_enco is sized to carry the "copy only" worst case.
         */
        i = ghc_suite_case_refs[suite_case].payload_len;
        test_decoder.size_enco = i + i / GHC_BREF_CNT_VALUE_MAX + 1;
        test_decoder.size_deco = i + ghc_suite_case_refs[suite_case].dictionary_len;

        test_decoder.pos_enco = 0;
        test_decoder.pos_deco = GHC_DICT_PRE_LEN;

        /* Set dictionary */
        ghcc_set_seq_dict_pre(&test_decoder, ghc_suite_case_refs[suite_case].dictionary);

        /* Prepare encoder */
        ghcc_set_seq_decoded(&test_decoder, ghc_suite_case_refs[suite_case].payload);
        ghcc_set_val_encoressed(&test_decoder, 0);

        printf("Check encoding of RFC7400 example %02"PRIu8":\n"
               "ghc_encode(ghc_suite_case_refs[%02"PRIu8"])\n"
               "[size_enco:%03u],[size_deco:%03u]\n",
               RFC7400_EXAMPLES_FIRST + suite_case, suite_case,
               test_decoder.size_enco, test_decoder.size_deco - GHC_DICT_PRE_LEN);
        result = ghc_encode(&test_decoder);
        printf("\nreturned with %03"PRId32"\n", result);
        //SHOW_GHC_CODER((&test_decoder));

        printf("!comp_size_diff: %u\n", ghc_suite_case_refs[suite_case].encoded_len - test_decoder.pos_enco);
        for (i=0; i < test_decoder.pos_enco; ++i) {
            comp_val1 = ghc_suite_case_refs[suite_case].encoded[i];
            comp_val2 = test_decoder.encoded[i];
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

        /* Prepare decoder */
        test_decoder.pos_enco = 0;
        test_decoder.pos_deco = GHC_DICT_PRE_LEN;
        ghcc_set_val_decoded(&test_decoder, 0);

        printf("Check decoding of RFC7400 example %02i:\n"
               "ghc_decode(ghc_suite_case_refs[%02i])\n",
               RFC7400_EXAMPLES_FIRST + suite_case, suite_case);
        result = ghc_decode(&test_decoder);
        printf("\nreturned with %03"PRId32"\n", result);
        //SHOW_GHC_CODER((&test_decoder));

        for (i=0; i < ghc_suite_case_refs[suite_case].payload_len; ++i) {
            comp_val1 = ghc_suite_case_refs[suite_case].payload[i];
            comp_val2 = test_decoder.decoded[GHC_DICT_PRE_LEN + i];
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
