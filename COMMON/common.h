/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : common.h
 * Description : common header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include  "stdio.h"	   //printf .....
#include  "string.h"   //strlen ,memset,strcmp,memcmp,strcpy .....
#include  "types.h"
#include  "config.h"
#include  "ach512.h"
#include  "uart.h"

#define SWAP(x)             ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))
#define max(a, b)		    (((a) > (b)) ? (a) : (b))
#define min(a, b)		    (((a) < (b)) ? (a) : (b))

typedef enum {
    CODE_SUCCESS = 0,
    CODE_FAILURE,
    CODE_RESET_DEVICE
} return_code_t;

#ifdef DEBUG
#define printfS     printf
#define printfB8    printf_buff_byte
#define printfB32   printf_buff_word
#define THROW(STR) {uint8_t lcd_msg[16] = {0};clear_screen();snprintf(lcd_msg, sizeof(lcd_msg), "err:%x", STR);wait_for_confirm();}
#else
#define	printfS(format, ...)	     ((void)0)
#define	printfB8(buff, byte_len)	 ((void)0)
#define	printfB32(buff, word_len)	 ((void)0)
#define THROW(STR)
#endif

/************************************************************************
 * function   : printf_buff_byte
 * Description: printf data block by byte
 * input :
 *         UINT8* buff: buff
 *         UINT32 length: byte length
 * return: none
 ************************************************************************/
void printf_buff_byte(const UINT8* buff, const UINT32 length);

/************************************************************************
 * function   : printf_buff_word
 * Description: printf data block by word
 * input :
 *         UINT8* buff: buff
 *         UINT32 length: word length
 * return: none
 ************************************************************************/
void printf_buff_word(UINT32* buff, UINT32 length);

void delay(UINT32 count);
void reverse_DWORD(UINT32 *var);
void reverse_memory(UINT8 *buff, UINT32 length);

const uint8_t *fromhex(const char *str);
size_t buf2hex(const uint8_t *buf,const size_t buf_len,uint8_t *out,size_t out_len);
int print_hex(const int8_t *file_name, const int line_number, const int8_t *time, const int8_t *func_name,const uint8_t *flag, const uint8_t *in, const uint32_t in_size);
int print_str(const uint8_t *file_name, const int line_number, const uint8_t *time, const uint8_t *func_name,const uint8_t *flag, const uint8_t *in);
int imemset(void *in, const uint8_t ch, const uint32_t in_size);
uint32_t print_int32s_hex_inv(const uint8_t *file_name, const int line_number, const uint8_t *time, const uint8_t *func_name,const uint8_t *prefix, const uint32_t*nums,const uint32_t len);
uint32_t print_int32s_hex(const uint8_t *file_name, const int line_number, const uint8_t *time, const uint8_t *func_name,const uint8_t *prefix, const uint32_t*nums,const uint32_t len);
int imemcpy(void* dest, const void* src, const uint32_t len);
uint32_t uint8_to_ecdsa_uint32(const uint8_t* in, const uint8_t in_len, uint32_t* out, uint32_t out_cap);
uint32_t ecdsa_hd_uint32_to_uint8(const uint32_t* in, const uint32_t in_len, uint8_t* out, uint32_t out_cap);


int32_t buffer_read_uint16_le(const uint8_t * buffer, uint16_t * out);
int32_t buffer_read_uint32_le(const uint8_t * buffer, uint32_t * out);
int32_t buffer_read_uint64_le(const uint8_t * buffer, uint64_t * out);

int32_t buffer_write_uint16_le(uint8_t * buffer, uint16_t value);
int32_t buffer_write_uint32_le(uint8_t * buffer, uint32_t value);
int32_t buffer_write_uint64_le(uint8_t * buffer, uint64_t value);
//void delay_ms(unsigned int ms);
void print_msg(uint8_t *msg,int len);
void show_errno(void);

uint8_t uint8_extension(const uint8_t *src, const uint8_t src_len, uint8_t *out, uint8_t out_capcity);
int base_convert(const uint8_t *buffer, const uint8_t buffer_len, uint8_t *out, const uint8_t out_capcity, const uint8_t src_base, const uint8_t dst_base);
bool num2str(uint8_t *src, uint8_t src_len);
uint8_t count_words_by_separator(const uint8_t *in, const uint8_t len, const uint8_t separator);
int str_arr_flat(char str[][10], const uint8_t str_max_index, char *out, const uint8_t out_cap);
bool amount_adjust(char* num_str, const int cap, const int8_t decimals, const int8_t fixed, const int8_t clear_ending_zero);

int xor(const uint8_t *src, const int src_len, const uint8_t *cipher, const int cipher_len, uint8_t *out);
void fill_sub_title(char *out, const int out_cap, const char *str);
inline bool is_zero(const void* src, const uint8_t size) {
    uint8_t *tmp = (uint8_t*)src;
    for (size_t i = 0; i < size; i++) {
        if (tmp[i] != 0)
            return false;
    }
    return true;
}


#endif

