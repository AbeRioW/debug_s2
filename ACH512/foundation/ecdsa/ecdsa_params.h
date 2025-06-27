#ifndef __ECDSA_PARAMS_H__
#define __ECDSA_PARAMS_H__

#include "common.h"

typedef struct{
    uint32_t CurveLength;
    uint32_t P_Array[8];
    uint32_t a_Array[8];
    uint32_t b_Array[8];
    uint32_t N_Array[8];
    uint32_t BaseX_Array[8];
    uint32_t BaseY_Array[8];
} ecc256_parameters;

extern const ecc256_parameters secp256k1_hd;
extern const ecc256_parameters secp256r1_hd;

#endif
