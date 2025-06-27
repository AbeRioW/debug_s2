#include "cx_derive.h"


static int derive_hdnode(const ecc_curve_t curve, HDNode * node, const UINT8 depth, const UINT32 * path) {
    const char * curve_name;
    if (curve == SECP256K1) {
        curve_name = SECP256K1_NAME;
    }
    else if (curve == SECP256R1) {
        curve_name = NIST256P1_NAME;
    }
    else if (curve == ED25519 || curve == ED25519_CARDANO) {
        curve_name = ED25519_CARDANO_NAME;
    }
    else {
        return 1;
    }

    UINT8 seed[64] = {0};
    UINT8 seed_len = 0;
    if (get_seed(seed, &seed_len, sizeof(seed)) != 0 || seed_len <= 0) {
        return 1;
    }

    if (curve == ED25519 || curve == ED25519_CARDANO) {
        hdnode_from_seed_cardano(seed, seed_len, node);
    }
    else if (!hdnode_from_seed(seed, seed_len, curve_name, node)) {
        return 1;
    }

    for (int i = 0; i < depth; i++) {
        if (curve == ED25519 || curve == ED25519_CARDANO) {
            hdnode_private_ckd_cardano(node, path[i]);
        }
        else {
            hdnode_private_ckd(node, path[i]);
        }
    }

    return 0;
}

int cx_derive_private_key(const ecc_curve_t curve, const UINT8 depth, const UINT32 * path, UINT8 * child_key, UINT8 * chain_code) {
    HDNode node;
    int ret = derive_hdnode(curve, &node, depth, path);
    if (ret != 0) {
        return ret;
    }

    memmove(child_key, node.private_key, sizeof(node.private_key));
    if (curve == ED25519 || curve == ED25519_CARDANO) {
        hdnode_fill_public_key(&node);
        memmove(child_key + sizeof(node.private_key), node.private_key_extension, sizeof(node.private_key_extension));
        memmove(child_key + sizeof(node.private_key) + sizeof(node.private_key_extension), node.public_key + 1, sizeof(node.public_key) - 1);
    }
    if (chain_code) {
        memmove(chain_code, node.chain_code, sizeof(node.chain_code));
    }
    return 0;
}

int cx_derive_public_key(const ecc_curve_t curve, const UINT8 depth, const UINT32 * path, UINT8 * child_key, UINT8 * chain_code) {
    HDNode node;
    int ret = derive_hdnode(curve, &node, depth, path);
    if (ret != 0) {
        return ret;
    }

    hdnode_fill_public_key(&node);

    memmove(child_key, node.public_key, sizeof(node.public_key));
    if (chain_code) {
        memmove(chain_code, node.chain_code, sizeof(node.chain_code));
    }
    return 0;
}
