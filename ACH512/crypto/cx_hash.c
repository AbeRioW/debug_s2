#include "cx_hash.h"
#include "hasher.h"
#include "common.h"

static Hasher hasher;

uint32_t get_return_length(const hash_type_t type) {
    if (type == SHA2_RIPEMD || type == BLAKE_RIPEMD) {
        return 16;
    }
    else if (type == SHA3_512 || type == SHA2_512) {
        return 64;
    }
    else if (type == SHA3_384) {
        return 48;
    }
    else if (type == SHA3_224) {
        return 28;
    }
    else if (type == SHA1) {
        return 20;
    }
    else {
        return 32;
    }
}

int cx_hash(const hash_type_t type, const hash_flag_t flag, const UINT8 * message, const UINT16 message_len, UINT8 * out, UINT32 * out_len) {
    HasherType hasher_type;
    if (type == SHA1)                    hasher_type = HASHER_SHA1;
    else if (type == SHA2)               hasher_type = HASHER_SHA2;
    else if (type == SHA2_512)           hasher_type = HASHER_SHA2_512;
    else if (type == SHA2D)              hasher_type = HASHER_SHA2D;
    else if (type == SHA2_RIPEMD)        hasher_type = HASHER_SHA2_RIPEMD;
    else if (type == SHA3)               hasher_type = HASHER_SHA3;
    else if (type == SHA3_KECCAK)        hasher_type = HASHER_SHA3K;
    else if (type == SHA3_224)           hasher_type = HASHER_SHA3_224;
    else if (type == SHA3_384)           hasher_type = HASHER_SHA3_384;
    else if (type == SHA3_512)           hasher_type = HASHER_SHA3_512;
    else if (type == BLAKE)              hasher_type = HASHER_BLAKE;
    else if (type == BLAKED)             hasher_type = HASHER_BLAKED;
    else if (type == BLAKE_RIPEMD)       hasher_type = HASHER_BLAKE_RIPEMD;
    else if (type == GROESTLD_TRUNC)     hasher_type = HASHER_GROESTLD_TRUNC;
    else if (type == BLAKE2B)            hasher_type = HASHER_BLAKE2B;
    else if (type == BLAKE2B_PERSONAL)   hasher_type = HASHER_BLAKE2B_PERSONAL;
    else return ERROR_CODE_INVALID_PARAMS;

    *out_len = 0;

    if (flag & FLAG_HASH_INIT) {
        hasher_Init(&hasher, hasher_type);
    }

    if (flag & FLAG_HASH_RESET) {
        hasher_Reset(&hasher);
    }

    if (flag & FLAG_HASH_UPDATE && message && message_len > 0) {
        hasher_Update(&hasher, message, message_len);
    }

    if (flag & FLAG_HASH_FINAL && out) {
        hasher_Final(&hasher, out);
        *out_len = get_return_length(type);
    }

    if (flag & FLAG_HASH_RAW) {
        hasher_Raw(hasher_type, message, message_len, out);
        *out_len = get_return_length(type);
    }

    return 0;
}
