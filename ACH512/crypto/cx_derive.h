#ifndef __CX_DERIVE_H__
#define __CX_DERIVE_H__

#include "types.h"

#include "bip32.h"
#include "curves.h"
#include "common.h"
#include "seed.h"

int cx_derive_private_key(const ecc_curve_t curve, const UINT8 depth, const UINT32 * path, UINT8 * child_key, UINT8 * chain_code);

int cx_derive_public_key(const ecc_curve_t curve, const UINT8 depth, const UINT32 * path, UINT8 * child_key, UINT8 * chain_code);

#endif
