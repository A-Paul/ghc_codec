#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "ghc.h"
#include "ghc_codec.h"

#include "tests/rfc7400_examples_testsuite.h"

#define RFC_EXAMPLES_PAYLOAD_MAX (0x60U)
#define RFC_EXAMPLES_COMPRESSED_MAX (0x3AU)
#define RFC7400_EXAMPLES_FIRST (8U)

#define PRINT_ALL (0)

int main (void)
{

    int32_t result;
    uint32_t i;
    uint32_t num_cases = (sizeof ghc_suite_case_refs)/
                         (sizeof ghc_suite_case_refs[0]);
    uint8_t suite_case = 0;
    uint8_t comp_val1 = 0;
    uint8_t comp_val2 = 0;

    uint8_t test_plod[GHC_DICT_PRE_LEN + RFC_EXAMPLES_PAYLOAD_MAX] = { 0 };
    uint8_t test_enco[RFC_EXAMPLES_COMPRESSED_MAX] = { 0 };

    struct ghc_codec test_decoder;

    for ( suite_case = 0; suite_case < num_cases; ++suite_case) {

        for (i=0; i < ghc_suite_case_refs[suite_case].dictionary_len; ++i) {
            test_plod[i] = ghc_suite_case_refs[suite_case].dictionary[i];
        }

        for (i=0; i < ghc_suite_case_refs[suite_case].encoded_len; ++i) {
            test_enco[i] = ghc_suite_case_refs[suite_case].encoded[i];
        }

        test_decoder.decoded = test_plod;
        test_decoder.encoded = test_enco;

        test_decoder.size_enco = ghc_suite_case_refs[suite_case].encoded_len;
        test_decoder.size_deco = ghc_suite_case_refs[suite_case].dictionary_len +
            ghc_suite_case_refs[suite_case].payload_len;

        printf("Check decoding of RFC7400 example %02i:\n"
               "ghc_decode(ghc_suite_case_refs[%02i])\n",
               RFC7400_EXAMPLES_FIRST + suite_case, suite_case);
        /*
        printf("calling ghc_decode(0x%p,%d,0x%p,%d)\n\n",
            test_decoder.encoded, test_decoder.size_enco,
            test_decoder.decoded, test_decoder.size_deco);
        */
        result = ghc_decode(&test_decoder);
        printf("\nreturned with %03i\n", result);

        for (i=0; i < ghc_suite_case_refs[suite_case].payload_len; ++i) {
            comp_val1 = ghc_suite_case_refs[suite_case].payload[i];
            comp_val2 = test_decoder.decoded[DICT_LEN+i];
            if (PRINT_ALL || (comp_val1 != comp_val2)) {
                printf("%03d:0x%02X:0x%02X,%c", i, comp_val1, comp_val2,
                    (((i+1) & 0x7) ? ' ' : '\n'));
            }
        }

        puts("\n");
    }

    return EXIT_SUCCESS;
}
