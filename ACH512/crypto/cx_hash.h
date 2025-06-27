#ifndef __CX_HASH_H__
#define __CX_HASH_H__

#include "types.h"

int cx_hash(const hash_type_t type, const hash_flag_t flag, const UINT8 * message, const UINT16 message_len, UINT8 * out, UINT32 * out_len);

#endif
