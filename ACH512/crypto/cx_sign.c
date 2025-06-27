#include "cx_sign.h"
#include "ecdsa.h"
#include "ecdsa_hd.h"
#include "ecdsa_params.h"
#include "secp256k1.h"
#include "nist256p1.h"
#include "ed25519.h"

static int sign_ecdsa_hd(const ecc256_parameters * ecc_params, const uint8_t* pri_key, uint32_t pri_key_len, const uint8_t* hash, uint32_t hash_len, uint8_t* sig, uint32_t sig_cap, uint32_t* sig_len) {
    if (pri_key == NULL || hash == NULL || sig == NULL || pri_key_len < 32 || hash_len < 32 || sig_cap < 65)
        return 1;

    uint32_t pri[8] = {0};
    uint32_t digest[8] = {0};
    uint32_t sig_r[8] = {0};
    uint32_t sig_s[8] = {0};

    uint8_to_ecdsa_uint32(pri_key, pri_key_len, pri, 8);
    uint8_to_ecdsa_uint32(hash, hash_len, digest, 8);

    ECC_G_STR ecc_glb_str;
    MATH_G_STR math_glb_str;

    ECC_para_initial((ECC_G_STR*)(&ecc_glb_str), ecc_params->CurveLength, (UINT32*)ecc_params->P_Array, (UINT32*)ecc_params->a_Array, (UINT32*)ecc_params->b_Array, (UINT32*)ecc_params->N_Array, (UINT32*)ecc_params->BaseX_Array, (UINT32*)ecc_params->BaseY_Array);
    int res = ECDSA_sign(&ecc_glb_str, &math_glb_str, digest, pri, sig_r, sig_s);
    *sig = 0x00;
    ecdsa_hd_uint32_to_uint8(sig_r, 8, sig + 1, 32);
    ecdsa_hd_uint32_to_uint8(sig_s, 8, sig + 1 + 32, 32);
    *sig_len = 65;
    return res;
}

static int is_canonical(uint8_t by, uint8_t sig[64]) {
    return !(sig[0] & 0x80) && !(sig[0] == 0 && !(sig[1] & 0x80));
}

static int is_canonical_strictly(uint8_t by, uint8_t sig[64]) {
    return !(sig[0] & 0x80) && !(sig[0] == 0 && !(sig[1] & 0x80)) && !(sig[32] & 0x80) && !(sig[32] == 0 && !(sig[33] & 0x80)); 
}

int cx_sign(ecc_curve_t curve, sign_flag_t flag, const uint8_t* pri_key, uint32_t pri_key_len, const uint8_t* hash, uint32_t hash_len, uint8_t* sig, uint32_t sig_cap, uint32_t* sig_len) {
    int res = 1;
    if (curve == SECP256K1) {
        if (flag & FLAG_HARDWARE) {
            res = sign_ecdsa_hd(&secp256k1_hd, pri_key, pri_key_len, hash, hash_len, sig, sig_cap, sig_len);
        }
        else {
            res = ecdsa_sign_digest(&secp256k1, pri_key, hash, sig + 1, sig, (flag & FLAG_CHECK_CANONICAL_STRICTLY) ? is_canonical_strictly : ((flag & FLAG_CHECK_CANONICAL) ? is_canonical : 0));
            sig[0] += 27;
            *sig_len = 65;
        }
    }
    else if (curve == SECP256R1) {
        if (flag & FLAG_HARDWARE) {
            res = sign_ecdsa_hd(&secp256r1_hd, pri_key, pri_key_len, hash, hash_len, sig, sig_cap, sig_len);
        }
        else {
            res = ecdsa_sign_digest(&nist256p1, pri_key, hash, sig + 1, sig, (flag & FLAG_CHECK_CANONICAL_STRICTLY) ? is_canonical_strictly : ((flag & FLAG_CHECK_CANONICAL) ? is_canonical : 0));
            sig[0] += 27;
            *sig_len = 65;
        }
    }
    else if (curve == ED25519 || curve == ED25519_CARDANO) {
        ed25519_sign_ext(hash, hash_len, pri_key, pri_key + 32, pri_key + 64, sig);
        *sig_len = 64;
        res = 0;
    }

    return res;
}
