#include "bip44.h"

bool hdnode_private_ckd_from_path(HDNode *inout, uint8_t *path, uint32_t path_len)
{
    int8_t strs[path_len + 1];
    int8_t *delim = "/";
    uint32_t index = 0;                     

    memset(strs, 0, sizeof(strs));
    strcpy(strs, path);

    int8_t *token = strtok(strs, delim);
    while (token){
        index = atoi(token);
        if (token != NULL && strchr(token,'\'')) {// hardened derivation
            hdnode_private_ckd_prime(inout,index);
        }else if (token != NULL){
            hdnode_private_ckd(inout,index);
        }
        
        token = strtok(NULL, delim);
    }
    return true;
}