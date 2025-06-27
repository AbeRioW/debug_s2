#include "bip39.h"
#include "common.h"
#include "eflash.h"
#include "hrng.h"
#include "hmac.h"
#include "seed.h"

#define EFLASH_ADDR_SEED (EFlashMainBaseAddr + EFLASH_SEED_PAGE_INDEX * PAGE_SIZE)


#define SN_BASE_ADDR (EFlashNVR2BaseAddr + 0x08)  //SN base addr
#define SN_CRC_ADDR (EFlashNVR2BaseAddr + 0x18)   //SN CRC value
#define CRC_INIT 0xffff

bool get_device_sequence(uint8_t* out, const uint8_t out_cap)
{
    #ifdef DEBUG
    printf("in func: %s\n", __func__);
    #endif

    if (out == NULL || out_cap < 16)
        return false;

    uint8_t* sn = (uint8_t*)SN_BASE_ADDR;

    for (uint8_t i = 0; i < 16; i++)
    {
        out[i] = sn[i];
    }

    return true;
}



int generate_mnemonic(char * out, int* out_len, uint8_t request_mnemonic_len)
{
    const char* mnemonic = NULL;

    if (request_mnemonic_len == 12)
    {
        mnemonic = mnemonic_generate(128);
    }
    else if (request_mnemonic_len == 24)
    {
        mnemonic = mnemonic_generate(256);
    }
    else
    {
        #ifdef DEBUG
        printf("this mnemonic length:%d is not support\n", request_mnemonic_len);
        #endif
        return -1;
    }

    #ifdef DEBUG
    printf("mnemonic:%s,len:%d\n", mnemonic, strlen(mnemonic));
    #endif
    strcpy(out, mnemonic);
    *out_len = strlen(mnemonic);
    return 0;  // NOTE this is important
}

static int calc_cipher(uint8_t cipher[32])
{
    uint8_t ds[16];

    if (!get_device_sequence(ds, sizeof(ds)))
    {
        return 1;
    }

    const uint8_t key[] = "WalletIO Seed Cipher";
    hmac_sha256(key, strlen((const char *)key), ds, sizeof(ds), cipher);

    #ifdef DEBUG
    printf("sequence: ");
    printfB8(ds, 16);
    printf("\n");

    printf("cipher: ");
    printfB8(cipher, 32);
    printf("\n");
    #endif

    return 0;
}

static int xor_seed(const uint8_t* src, const int src_len, uint8_t* out)
{
    uint8_t cipher[32];
    int ret = calc_cipher(cipher);
    ret += xor(src, src_len, cipher, sizeof(cipher), out);
    return ret;
}

int get_seed(uint8_t * seed, uint8_t * seed_len, const uint8_t capacity)
{
//    if (!is_authenticated())
//    {
//        #ifdef DEBUG
//        printf("permission denied for getting seed\n");
//        #endif
//        return ERROR_CODE_PERMISSION_DENIED;
//    }

    if (capacity < 64)
    {
        #ifdef DEBUG
        printf("capacity not enough\n");
        #endif
        return ERROR_CODE_INTERNAL_ERROR;
    }

    uint32_t eflash_data[2 + 32] = {0};
    eflash_read_arr(EFLASH_ADDR_SEED, eflash_data, sizeof(eflash_data) / sizeof(uint32_t));

    uint32_t type = eflash_data[0];

    if (type == 0)
    {
        *seed_len = 64;

        if (ecdsa_hd_uint32_to_uint8(&eflash_data[1], 16, seed, *seed_len) == 0)
        {
            #ifdef DEBUG
            printf("ecdsa_hd_uint32_to_uint8 error\n");
            #endif
            return ERROR_CODE_INTERNAL_ERROR;
        }
    }
    else if (type == 1)
    {
        *seed_len = 64;

        if (xor_seed((const uint8_t*)&eflash_data[1], *seed_len, seed) != 0)
        {
            #ifdef DEBUG
            printf("decrypt seed error\n");
            #endif
            return ERROR_CODE_INTERNAL_ERROR;
        }
    }
    else if (type == 2)
    {
        *seed_len = eflash_data[1];

        if (xor_seed((const uint8_t*)&eflash_data[2], *seed_len, seed) != 0)
        {
            #ifdef DEBUG
            printf("decrypt seed error\n");
            #endif
            return ERROR_CODE_INTERNAL_ERROR;
        }

    }
    else
    {
        #ifdef DEBUG
        printf("unknown seed data\n");
        #endif
        return ERROR_CODE_INTERNAL_ERROR;
    }

    return 0;
}

int save_seed(uint8_t* seed, const uint16_t seed_len)
{
//    if (!is_authenticated())
//    {
//        #ifdef DEBUG
//        printf("permission denied for saving seed\n");
//        #endif
//        return ERROR_CODE_PERMISSION_DENIED;
//    }

    uint32_t seed_u32[17] = {0};  // first 32bit for header
    seed_u32[0] = 2;
    seed_u32[1] = seed_len;

    if (xor_seed(seed, seed_len, (uint8_t*)&seed_u32[2]) != 0)
    {
        #ifdef DEBUG
        printf("encrypt seed error\n");
        #endif
        return ERROR_CODE_INTERNAL_ERROR;
    }

    #ifdef DEBUG
    printf("encrypt: ");
    printfB8(seed, 64);
    printf(" -> ");
    printfB8((uint8_t*)&seed_u32[2], 64);
    printf("\n");
    #endif

    eflash_erase_page(EFLASH_ADDR_SEED);
    eflash_write_arr(EFLASH_ADDR_SEED, seed_u32, sizeof(seed_u32));

    memset(seed, 0, seed_len);
    memset(seed_u32, 0, sizeof(seed_u32));

    return 0;
}

int seed_check_mnemonic(const char* mnemonic, uint8_t* out, int* tx)
{
    #ifdef DEBUG
    printf("mnemonic: %s\n", mnemonic);
    printf("mnemonic len: %d\n", strlen(mnemonic));
    #endif

    if (mnemonic_check(mnemonic) == 0)
    {
        #ifdef DEBUG
        printf("ERROR_CODE_INVALID_PARAMS\n");
        #endif
        return ERROR_CODE_INVALID_PARAMS;
    }

    mnemonic_to_seed(mnemonic, "", out, NULL);
    *tx = 64;
    return 0;
}

int seed_reset(void)
{
    eflash_erase_page(EFLASH_ADDR_SEED);
    return 0;
}
