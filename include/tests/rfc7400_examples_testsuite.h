#ifndef GHC_EXAMPLES_TESTSUITE_H
#define GHC_EXAMPLES_TESTSUITE_H

#include "tests/rfc7400_examples.h"

struct ghc_case_ref ghc_suite_case_refs[] = {
{
    (sizeof ghc_expl_08_plod)/(sizeof ghc_expl_08_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_08_comp)/(sizeof ghc_expl_08_comp[0]),
    ghc_expl_08_plod,
    ghc_expl_08_dict,
    ghc_expl_08_comp
},

{
    (sizeof ghc_expl_09_plod)/(sizeof ghc_expl_09_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_09_comp)/(sizeof ghc_expl_09_comp[0]),
    ghc_expl_09_plod,
    ghc_expl_09_dict,
    ghc_expl_09_comp
},

{
    (sizeof ghc_expl_10_plod)/(sizeof ghc_expl_10_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_10_comp)/(sizeof ghc_expl_10_comp[0]),
    ghc_expl_10_plod,
    ghc_expl_10_dict,
    ghc_expl_10_comp
},

{
    (sizeof ghc_expl_11_plod)/(sizeof ghc_expl_11_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_11_comp)/(sizeof ghc_expl_11_comp[0]),
    ghc_expl_11_plod,
    ghc_expl_11_dict,
    ghc_expl_11_comp
},

{
    (sizeof ghc_expl_12_plod)/(sizeof ghc_expl_12_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_12_comp)/(sizeof ghc_expl_12_comp[0]),
    ghc_expl_12_plod,
    ghc_expl_12_dict,
    ghc_expl_12_comp
},

{
    (sizeof ghc_expl_13_plod)/(sizeof ghc_expl_13_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_13_comp)/(sizeof ghc_expl_13_comp[0]),
    ghc_expl_13_plod,
    ghc_expl_13_dict,
    ghc_expl_13_comp
},

{
    (sizeof ghc_expl_14_plod)/(sizeof ghc_expl_14_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_14_comp)/(sizeof ghc_expl_14_comp[0]),
    ghc_expl_14_plod,
    ghc_expl_14_dict,
    ghc_expl_14_comp
},

{
    (sizeof ghc_expl_15_plod)/(sizeof ghc_expl_15_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_15_comp)/(sizeof ghc_expl_15_comp[0]),
    ghc_expl_15_plod,
    ghc_expl_15_dict,
    ghc_expl_15_comp
},

{
    (sizeof ghc_expl_16_plod)/(sizeof ghc_expl_16_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_16_comp)/(sizeof ghc_expl_16_comp[0]),
    ghc_expl_16_plod,
    ghc_expl_16_dict,
    ghc_expl_16_comp
},

{
    (sizeof ghc_expl_17_plod)/(sizeof ghc_expl_17_plod[0]),
    DICT_LEN,
    (sizeof ghc_expl_17_comp)/(sizeof ghc_expl_17_comp[0]),
    ghc_expl_17_plod,
    ghc_expl_17_dict,
    ghc_expl_17_comp
}
};

#endif /* !GHC_EXAMPLES_TESTSUITE_H */
