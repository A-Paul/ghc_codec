#include "tests/rfc7400_examples.h"

/* Why is this failing for */
struct ghc_case_ref const ghc_case_08 = {
    /* 8, */
    (sizeof ghc_expl_08_plod)/(sizeof ghc_expl_08_plod[0]),
    DICT_LEN,
    6,
    ghc_expl_08_plod,
    ghc_expl_08_dict,
    ghc_expl_08_byco
};


struct ghc_case_ref ghc_case_09 = {
    /* 92, */
    (sizeof ghc_expl_09_plod)/(sizeof ghc_expl_09_plod[0]),
    DICT_LEN,
    52,
    ghc_expl_09_plod,
    ghc_expl_09_dict,
    ghc_expl_09_byco
};

struct ghc_case_ref ghc_case_10 = {
    /* 50, */
    (sizeof ghc_expl_10_plod)/(sizeof ghc_expl_10_plod[0]),
    DICT_LEN,
    27,
    ghc_expl_10_plod,
    ghc_expl_10_dict,
    ghc_expl_10_byco
};

struct ghc_case_ref ghc_case_11 = {
    /* 48, */
    (sizeof ghc_expl_11_plod)/(sizeof ghc_expl_11_plod[0]),
    DICT_LEN,
    26,
    ghc_expl_11_plod,
    ghc_expl_11_dict,
    ghc_expl_11_byco
};

struct ghc_case_ref ghc_case_12 = {
    /* 48, */
    (sizeof ghc_expl_12_plod)/(sizeof ghc_expl_12_plod[0]),
    DICT_LEN,
    27,
    ghc_expl_12_plod,
    ghc_expl_12_dict,
    ghc_expl_12_byco
};

struct ghc_case_ref ghc_case_13 = {
    /* 24, */
    (sizeof ghc_expl_13_plod)/(sizeof ghc_expl_13_plod[0]),
    DICT_LEN,
    12,
    ghc_expl_13_plod,
    ghc_expl_13_dict,
    ghc_expl_13_byco
};

struct ghc_case_ref ghc_case_14 = {
    /* 96, */
    (sizeof ghc_expl_14_plod)/(sizeof ghc_expl_14_plod[0]),
    DICT_LEN,
    58,
    ghc_expl_14_plod,
    ghc_expl_14_dict,
    ghc_expl_14_byco
};

struct ghc_case_ref ghc_case_15 = {
    /* 42, */
    (sizeof ghc_expl_15_plod)/(sizeof ghc_expl_15_plod[0]),
    DICT_LEN,
    27,
    ghc_expl_15_plod,
    ghc_expl_15_dict,
    ghc_expl_15_byco
};

struct ghc_case_ref ghc_case_16 = {
    /* 35, */
    (sizeof ghc_expl_16_plod)/(sizeof ghc_expl_16_plod[0]),
    DICT_LEN,
    22,
    ghc_expl_16_plod,
    ghc_expl_16_dict,
    ghc_expl_16_byco
};

struct ghc_case_ref ghc_case_17 = {
    /* 67, */
    (sizeof ghc_expl_17_plod)/(sizeof ghc_expl_17_plod[0]),
    DICT_LEN,
    53,
    ghc_expl_17_plod,
    ghc_expl_17_dict,
    ghc_expl_17_byco
};

/*
 * Failing with:
 * examples.h:107:5: error: initializer element is not constant
 *      ghc_case_08,
 *      ^~~~~~~~~~~
 * examples.h:107:5: note: (near initialization for ‘ghc_suite_case_refs[0]’)
 * [...]
 * examples.h:116:5: note: (near initialization for ‘ghc_suite_case_refs[9]’)
 */

struct ghc_case_ref const ghc_suite_case_refs_fail[] = {
    ghc_case_08,
    ghc_case_09,
    ghc_case_10,
    ghc_case_11,
    ghc_case_12,
    ghc_case_13,
    ghc_case_14,
    ghc_case_15,
    ghc_case_16,
    ghc_case_17
};
