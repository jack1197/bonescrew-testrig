/*
 * FirCoeffs.cpp
 *
 *  Created on: 16 Nov 2020
 *      Author: Jack
 */



#include "FirCoeffs.hpp"
#include <stdint.h>

//First half of symetric coefficients
const int firCoeffsN = 314;

const uint16_t firCoeffs[314] = {
         7764,    767,    805,    844,    884,    925,    968,   1012,   1057,
         1103,   1151,   1199,   1250,   1300,   1352,   1405,   1460,   1515,
         1573,   1631,   1693,   1754,   1820,   1885,   1954,   2020,   2090,
         2157,   2230,   2301,   2388,   2465,   2533,   2623,   2701,   2788,
         2872,   2961,   3048,   3140,   3232,   3327,   3422,   3521,   3621,
         3723,   3827,   3933,   4040,   4149,   4260,   4373,   4488,   4605,
         4724,   4847,   4969,   5095,   5220,   5350,   5482,   5618,   5749,
         5888,   6028,   6169,   6313,   6459,   6607,   6757,   6910,   7064,
         7220,   7379,   7540,   7703,   7868,   8036,   8206,   8378,   8552,
         8728,   8906,   9087,   9270,   9456,   9643,   9833,  10025,  10219,
        10415,  10615,  10816,  11018,  11225,  11432,  11642,  11854,  12069,
        12285,  12504,  12725,  12949,  13174,  13402,  13632,  13864,  14098,
        14334,  14573,  14814,  15056,  15301,  15548,  15797,  16049,  16302,
        16558,  16815,  17074,  17336,  17600,  17865,  18132,  18403,  18674,
        18947,  19223,  19500,  19779,  20060,  20343,  20628,  20915,  21203,
        21493,  21784,  22078,  22373,  22670,  22968,  23269,  23570,  23873,
        24178,  24484,  24792,  25101,  25412,  25723,  26037,  26351,  26667,
        26984,  27302,  27623,  27943,  28265,  28588,  28912,  29237,  29563,
        29890,  30218,  30547,  30877,  31207,  31538,  31870,  32203,  32536,
        32871,  33205,  33540,  33876,  34212,  34548,  34886,  35223,  35560,
        35898,  36236,  36574,  36913,  37251,  37590,  37928,  38266,  38605,
        38943,  39281,  39619,  39956,  40294,  40631,  40967,  41303,  41639,
        41973,  42308,  42642,  42975,  43307,  43639,  43969,  44299,  44628,
        44956,  45283,  45609,  45934,  46257,  46580,  46901,  47221,  47539,
        47857,  48172,  48487,  48799,  49110,  49420,  49728,  50034,  50338,
        50641,  50941,  51240,  51537,  51832,  52124,  52415,  52704,  52990,
        53274,  53556,  53836,  54113,  54388,  54660,  54930,  55197,  55462,
        55724,  55984,  56241,  56495,  56746,  56994,  57240,  57483,  57723,
        57959,  58193,  58424,  58651,  58876,  59097,  59315,  59530,  59741,
        59950,  60154,  60356,  60554,  60749,  60940,  61127,  61311,  61492,
        61669,  61842,  62012,  62178,  62340,  62498,  62653,  62804,  62951,
        63094,  63233,  63369,  63500,  63628,  63751,  63871,  63986,  64098,
        64205,  64309,  64408,  64503,  64595,  64682,  64764,  64843,  64918,
        64988,  65054,  65116,  65174,  65227,  65276,  65321,  65362,  65399,
        65431,  65459,  65482,  65502,  65517,  65527,  65534,  65535
};

/*
const uint16_t firCoeffs[314] = {
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,65535

};

*/
