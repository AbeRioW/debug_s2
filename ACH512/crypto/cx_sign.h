#ifndef __CX_SIGN_H__
#define __CX_SIGN_H__

#include "types.h"

int cx_sign(ecc_curve_t curve, sign_flag_t flag, const uint8_t* pri_key, uint32_t pri_key_len, const uint8_t* hash, uint32_t hash_len, uint8_t* sig, uint32_t sig_cap, uint32_t* sig_len);

#endif
