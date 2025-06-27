#ifndef __SEED_H__
#define __SEED_H__

#include "types.h"

#define EFLASH_SLAVE_DATA_START_PAGE_INDEX               1018
#define EFLASH_IS_DEVICE_INIT_PAGE_INDEX                 (EFLASH_SLAVE_DATA_START_PAGE_INDEX + 0)
#define EFLASH_SEED_PAGE_INDEX                           (EFLASH_SLAVE_DATA_START_PAGE_INDEX + 1)
#define EFLASH_PIN_PAGE_INDEX                            (EFLASH_SLAVE_DATA_START_PAGE_INDEX + 2)

int generate_mnemonic(char * out, int* mnemonic_len, uint8_t request_mnemonic_len);
int get_seed(uint8_t * seed, uint8_t * seed_len, const uint8_t capacity);
int seed_check_mnemonic(const char* mnemonic, uint8_t* out, int* tx);
int save_seed(uint8_t* seed, const uint16_t seed_len);
int seed_reset(void);

#endif
