#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "ghc.h"
#include "ghc_codec.h"

#include "tests/rfc7400_examples_testsuite.h"

#define RFC_EXAMPLES_PAYLOAD_MAX (0x60U)
#define RFC_EXAMPLES_COMPRESSED_MAX (0x3AU)

int main (void)
{

    int32_t result;
    uint8_t i;
    uint8_t suite_case = 2;

    uint8_t test_plod[GHC_DICT_PRE_LEN + RFC_EXAMPLES_PAYLOAD_MAX] = { 0 };
    uint8_t test_comp[RFC_EXAMPLES_COMPRESSED_MAX] = { 0 };
    uint16_t sn;
    uint16_t tn;

    for (i=0; i < ghc_suite_case_refs[suite_case].dictionary_len; ++i) {
        test_plod[i] = ghc_suite_case_refs[suite_case].dictionary[i];
    }

    for (i=0; i < ghc_suite_case_refs[suite_case].compressed_len; ++i) {
        test_comp[i] = ghc_suite_case_refs[suite_case].compressed[i];
    }


    sn = ghc_suite_case_refs[suite_case].compressed_len;
    tn = ghc_suite_case_refs[suite_case].dictionary_len +
         ghc_suite_case_refs[suite_case].payload_len;

    printf("calling ghc_decompress(0x%p,%d,0x%p,%d)\n\n",
           test_comp, sn, test_plod, tn);
    result = ghc_decompress(test_comp, sn, test_plod, tn);
    printf("\nreturned with %03i\n", result);

    for (i=0; i < ghc_suite_case_refs[suite_case].payload_len; ++i) {
        printf("%03d:0x%02X:0x%02X,%c", i,
               ghc_suite_case_refs[suite_case].payload[i],
               test_plod[DICT_LEN+i],
               (((i+1) & 0x7) ? ' ' : '\n'));
    }

    puts("\n");

    return EXIT_SUCCESS;
}
