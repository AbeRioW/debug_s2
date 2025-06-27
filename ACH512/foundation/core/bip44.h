#ifndef __BIP44_H__
#define __BIP44_H__

#include "common.h"
#include "bip32.h"
#include <string.h>


bool hdnode_private_ckd_from_path(HDNode *inout, uint8_t *path,uint32_t path_len);

#endif