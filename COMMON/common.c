


/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : common.c
 * Description : conmmon source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include  "common.h"

#include <errno.h>

#define FROMHEX_MAXLEN 512

#ifdef DEBUG
/************************************************************************
 * function   : printf_buff_byte
 * Description: printf data block by byte
 * input :
 *         UINT8* buff: buff
 *         UINT32 length: byte length
 * return: none
 ************************************************************************/
void printf_buff_byte(const UINT8* buff, const UINT32 length)
{
	UINT32 i;

	for(i=0;i<length;i++)
	{
		printf("%.2x",buff[i]);	
	}
}


/************************************************************************
 * function   : printf_buff_word
 * Description: printf data block by word
 * input :
 *         UINT8* buff: buff
 *         UINT32 length: word length
 * return: none
 ************************************************************************/
void printf_buff_word(UINT32* buff, UINT32 length)
{
	UINT32 i;

	for(i=0;i<length;i++)
	{
		printf("%.8x",buff[i]);	
	}
}
#endif

void delay(UINT32 count)
{
    while(count--);
}

//一个字内大小端转换
void reverse_DWORD(UINT32 *var)
{
    UINT8 *P = (UINT8 *)var;
    UINT8 tmp;

    tmp = P[0];
    P[0] = P[3];
    P[3] = tmp;
    tmp = P[1];
    P[1] = P[2];
    P[2] = tmp;
}

//整批数据前后大小端转换
void reverse_memory(UINT8 *buff, UINT32 length)
{
    UINT8 temp;
    UINT8 *buff_start = buff;
    UINT8 *buff_end = buff + length - 1;

    while(buff_end > buff_start)
    {
        temp = *buff_start;
        *buff_start++ = *buff_end;
        *buff_end-- = temp;
    }
}


const uint8_t *fromhex(const char *str)
{
	static uint8_t buf[FROMHEX_MAXLEN];
	size_t len = strlen(str) / 2;
	if (len > FROMHEX_MAXLEN)
		len = FROMHEX_MAXLEN;
	for (size_t i = 0; i < len; i++)
	{
		uint8_t c = 0;
		if (str[i * 2] >= '0' && str[i * 2] <= '9')
			c += (str[i * 2] - '0') << 4;
		if ((str[i * 2] & ~0x20) >= 'A' && (str[i * 2] & ~0x20) <= 'F')
			c += (10 + (str[i * 2] & ~0x20) - 'A') << 4;
		if (str[i * 2 + 1] >= '0' && str[i * 2 + 1] <= '9')
			c += (str[i * 2 + 1] - '0');
		if ((str[i * 2 + 1] & ~0x20) >= 'A' && (str[i * 2 + 1] & ~0x20) <= 'F')
			c += (10 + (str[i * 2 + 1] & ~0x20) - 'A');
		buf[i] = c;
	}
	return buf;
}

size_t buf2hex(const uint8_t *buf,const size_t buf_len,uint8_t *out,size_t out_len){
  if (out_len < buf_len *2 || buf == NULL || out == NULL) return 0;
  size_t out_index = 0;
  uint8_t left = 0;
  uint8_t right = 0;

  for (size_t i = 0; i < buf_len; i++,out_index+=2){
    left = (buf[i] >> 4);
    right = (buf[i] & 0x0f);
    out[out_index] = left >= 0x0a && left <= 0x0f ? left + 0x57 : left + 0x30;
    out[out_index+1] = right >= 0x0a && right <= 0x0f ? right + 0x57 : right + 0x30;
  }
  
  return out_index;
}

int print_hex(const int8_t* file_name, const int line_number, const int8_t* time, const int8_t* func_name, const uint8_t* flag, const uint8_t* in, const uint32_t in_size) {
    if (in == NULL || in_size == 0)
        return 0;
    printf("%s,%d,%s,%s,%s:", file_name, line_number, time, func_name, flag);
    int i = 0;
    for (; i < in_size; i++)
        printf("%.2x", *(in + i));
    printf("\n");
    return i;
}

int print_str(const uint8_t *file_name, const int line_number, const uint8_t *time, const uint8_t *func_name, const uint8_t *flag, const uint8_t *in)
{
	return printf("%s,%d,%s,%s,%s:%s\n", file_name, line_number, time, func_name, flag, in);
}

int imemset(void *in, const uint8_t ch, const uint32_t in_size)
{
	uint8_t *buf = (void *)in;
	size_t i = 0;
	for (; i < in_size; i++) *buf++ = ch;
	return i;
}

int imemcpy(void* dest, const void* src, const uint32_t len) {
    if (dest == NULL || src == NULL || len == 0) return 0;
    uint8_t* d = (void*)dest;
    uint8_t* s = (void*)src;
    size_t i = 0;
    for (; i < len; i++) *(d + i) = *(s + i);
    return i;
}

uint32_t print_int32s_hex_inv(const uint8_t *file_name, const int line_number, const uint8_t *time, const uint8_t *func_name,const uint8_t *prefix, const uint32_t*nums,const uint32_t len){
  if (nums ==NULL||len==0) return 0; 
  printf("%s,%d,%s,%s,%s:", file_name, line_number, time, func_name, prefix);
  int i = len - 1;
  for (; i >= 0; i--)
    printf("%.8x",*(nums+i));
  printf("\n");
  return i;
}

uint32_t print_int32s_hex(const uint8_t *file_name, const int line_number, const uint8_t *time, const uint8_t *func_name,const uint8_t *prefix, const uint32_t*nums,const uint32_t len){
  if (nums ==NULL||len==0) return 0; 
  printf("%s,%d,%s,%s,%s:", file_name, line_number, time, func_name, prefix);
  int i = 0;
  for (; i < len; i++)
    printf("%.8x",*(nums+i));
  printf("\n");
  return i;
}

/**
 * @brief  
 * @note   
 *      1. 从后往前遍历in
 *      1. 每4B的数据存放在out前面
 *      1. 直到完成遍历
 * @param  in: 
 * @param  in_len: 
 * @param  out: 
 * @param  out_cap: 
 * @retval 
 */
uint32_t uint8_to_ecdsa_uint32(const uint8_t* in, const uint8_t in_len, uint32_t* out, uint32_t out_cap) {
    if (in == NULL || out == NULL || out_cap * 4 < in_len)
        return 0;
    uint32_t buf = 0;
    uint32_t out_index = 0;
    for (int i = in_len; i > 0;) {
        i -= 4;
        buf = in[i];
        buf <<= 8;

        buf |= in[i + 1];
        buf <<= 8;

        buf |= in[i + 2];
        buf <<= 8;

        buf |= in[i + 3];

        out[out_index++] = buf;
        buf = 0;
    }
    return out_index;
}


/**
 * @brief  硬件的ecdsa的数据转换为一般的数据存储情况
 * @note   1. 从后往前遍历in
 *         1. in中的32bit 数据份8bit存out中的4个cell
 *         1. 直到遍历完in
 * @param  in: 
 * @param  in_len: 
 * @param  out: 
 * @param  out_cap: 
 * @retval 
 */
uint32_t ecdsa_hd_uint32_to_uint8(const uint32_t* in, const uint32_t in_len, uint8_t* out, uint32_t out_cap) {
    if (in == NULL || out == NULL || out_cap < in_len * 4)
        return 0;
    uint32_t out_index = 0;
    uint32_t buf = 0;
    for (int i = in_len - 1; i >= 0; i--) {
        buf = in[i];
        out[out_index++] = (buf | 0x00ffffff) >> 24;
        out[out_index++] = (buf | 0xff00ffff) >> 16;
        out[out_index++] = (buf | 0xffff00ff) >> 8;
        out[out_index++] = buf | 0xffffff00;
    }
    return out_index;
}

int32_t buffer_read_uint16_le(const uint8_t * buffer, uint16_t * out) {
    ((uint8_t*)out)[0] = buffer[0];
    ((uint8_t*)out)[1] = buffer[1];
    return 2;
}

int32_t buffer_read_uint32_le(const uint8_t * buffer, uint32_t * out) {
    ((uint8_t*)out)[0] = buffer[0];
    ((uint8_t*)out)[1] = buffer[1];
    ((uint8_t*)out)[2] = buffer[2];
    ((uint8_t*)out)[3] = buffer[3];
    return 4;
}

int32_t buffer_read_uint64_le(const uint8_t * buffer, uint64_t * out) {
    ((uint8_t*)out)[0] = buffer[0];
    ((uint8_t*)out)[1] = buffer[1];
    ((uint8_t*)out)[2] = buffer[2];
    ((uint8_t*)out)[3] = buffer[3];
    ((uint8_t*)out)[4] = buffer[4];
    ((uint8_t*)out)[5] = buffer[5];
    ((uint8_t*)out)[6] = buffer[6];
    ((uint8_t*)out)[7] = buffer[7];
    return 8;
}

int32_t buffer_write_uint16_le(uint8_t * buffer, uint16_t value) {
    buffer[0] = ((uint8_t*)&value)[0];
    buffer[1] = ((uint8_t*)&value)[1];
    return 2;
}

int32_t buffer_write_uint32_le(uint8_t * buffer, uint32_t value) {
    buffer[0] = ((uint8_t*)&value)[0];
    buffer[1] = ((uint8_t*)&value)[1];
    buffer[2] = ((uint8_t*)&value)[2];
    buffer[3] = ((uint8_t*)&value)[3];
    return 4;
}

int32_t buffer_write_uint64_le(uint8_t * buffer, uint64_t value) {
    buffer[0] = ((uint8_t*)&value)[0];
    buffer[1] = ((uint8_t*)&value)[1];
    buffer[2] = ((uint8_t*)&value)[2];
    buffer[3] = ((uint8_t*)&value)[3];
    buffer[4] = ((uint8_t*)&value)[4];
    buffer[5] = ((uint8_t*)&value)[5];
    buffer[6] = ((uint8_t*)&value)[6];
    buffer[7] = ((uint8_t*)&value)[7];
    return 8;
}

//void delay_ms(unsigned int ms)
//{
//	unsigned int dd = ms * 30000;
//	for( ; dd != 0; dd --);
//}

void print_msg(uint8_t *msg,int len) {
    printfS("\n");
    for (size_t i = 0; i < len; i++) {
        printfS("%d", msg[i]);
    }
    printfS("\n");
}

void show_errno(void)
{
    if(errno==EDOM)   printfS("domain error");
    if(errno==EILSEQ) printfS("illegal sequence");    
    if(errno==ERANGE) printfS("pole or range error");
    if(errno==0)      printfS("no error");
    printfS(" occurred\n");
}

uint8_t uint8_extension(const uint8_t *src, const uint8_t src_len, uint8_t *out, uint8_t out_capcity)
{
    if (out_capcity < src_len * 2) return 1;
    uint8_t buffer_index = 0;

    uint8_t index = 0;
    while(src[index] == 0x00) index ++;
    for (; index < src_len; index++) {
        out[buffer_index++] = src[index] >> 4;
        out[buffer_index++] = src[index] & 0x0f;
    }
    return buffer_index;
}

int base_convert(const uint8_t *buffer, const uint8_t buffer_len, uint8_t *out, const uint8_t out_capcity, const uint8_t src_base, const uint8_t dst_base){
#define BUFFER_SIZE 50
  if (BUFFER_SIZE < buffer_len)
    return -1;

  uint8_t quotients[BUFFER_SIZE] = {0};
  uint8_t next_quotients[BUFFER_SIZE] = {0};
  uint8_t last_remainders[BUFFER_SIZE] = {0};

  memcpy(quotients, buffer, buffer_len);

  size_t out_len = 0;
  int div_index = 0;
  int remainder = 0;
  uint8_t tmp1, tmp2 = 0;

  for (int quotient_digits = buffer_len; quotient_digits > 0;) {
    for (div_index = 0, remainder = 0; div_index < quotient_digits; div_index++) {
      remainder = remainder * src_base + ((quotients[div_index] & 0xf0) >> 4);
      tmp1 = remainder / dst_base;
      remainder %= dst_base;

      remainder = remainder * src_base + (quotients[div_index] & 0x0f);
      tmp2 = remainder / dst_base;
      remainder %= dst_base;

      next_quotients[div_index] = (tmp1 << 4 | tmp2);
    }

    quotient_digits = div_index;
    last_remainders[out_len++] = remainder;

    int zero_cnt = 0;
    for (int i = 0; i < quotient_digits; i++) {
      if (next_quotients[i] == 0)
        zero_cnt++;
      else
        break;
    }

    quotient_digits -= zero_cnt;
    memset(quotients, 0, sizeof(quotients));
    memcpy(quotients, next_quotients + zero_cnt, quotient_digits);
    memset(next_quotients, 0, quotient_digits);
  }

  if (out_capcity < out_len + 1) return -1;

  int index = 0;
  for (; index < out_len;) {
    out[index++] = last_remainders[out_len - index - 1] + '0';
  }
  out[index] = 0;
  return index;
}

bool num2str(uint8_t *src, uint8_t src_len) {
    for (uint8_t i = 0; i < src_len; i++) {
        src[i] += 48;
    }
    src[src_len] = '\0';
    return true;
}

uint8_t count_words_by_separator(const uint8_t *in, const uint8_t len, const uint8_t separator) {
  if (len <= 0 || in == NULL) return 0;

  uint8_t cnt = 0;
  for (uint8_t i = 0; i < len; i++)
    if (in[i] == separator) cnt++;
  return cnt + 1;
}

int str_arr_flat(char str[][10], const uint8_t str_max_index, char *out, const uint8_t out_cap) {
  if (str == NULL || out == NULL) return 0;
  char  tmp[10] = {0};
  int cnt = 0;

  for (size_t i = 0; i <= str_max_index; i++) {
    if (i == str_max_index)
      sprintf(tmp, "%s", str[i]);
    else
      sprintf(tmp, "%s ", str[i]);

    if (i > out_cap) return 0;
    strcat(out, tmp);
    cnt += strlen(tmp);
  }
  return cnt;
}

bool amount_adjust(char* num_str, const int cap, const int8_t decimals, const int8_t fixed, const int8_t clear_ending_zero) {
    int len = strlen(num_str);
    int8_t offset = 1;  // 1 for dot
    if (len <= decimals) {
        offset = decimals - len + 2;  // addition 2 bytes for '0.'
    }

    const int new_len = len + offset + (fixed - decimals);
    if (cap < new_len) {
        return false;
    }

    num_str[new_len] = 0;

    int index = new_len - 1;
    int8_t width = 0;
    while (index >= 0) {
        int real_index = index - offset;
        num_str[index] = (real_index >= 0 && real_index < len) ? num_str[real_index] : '0';

        if (++width == fixed) {
            num_str[--index] = '.';
            if (--offset == 0) {
                break;
            }
        }
        index--;
    }

    if (clear_ending_zero) {
        index = new_len - 1;
        width = new_len - fixed;
        while (index > width) {
            if (num_str[index] != '0') {
                break;
            }
            num_str[index] = 0;
            index--;
        }
    }

    return true;
}

int xor(const uint8_t *src, const int src_len, const uint8_t *cipher, const int cipher_len, uint8_t *out) {
    for (int i = 0; i < src_len; i++) {
        out[i] = src[i] ^ cipher[i % cipher_len];
    }
    return 0;
}

void fill_sub_title(char *out, const int out_cap, const char *str) {
#define AVAIABLE_WIDTH 8
  int len = strlen(str);
  if (len >= AVAIABLE_WIDTH && AVAIABLE_WIDTH < out_cap) {
    int offset = 0;
    out[offset++] = '(';
    out[offset++] = str[0];
    out[offset++] = str[1];
    out[offset++] = str[2];
    out[offset++] = '.';
    out[offset++] = '.';
    out[offset++] = str[len - 2];
    out[offset++] = str[len - 1];
    out[offset++] = ')';
  } else {
    snprintf(out, out_cap, "(%s)", str);
  }
}

