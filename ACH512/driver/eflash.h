/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : eflash.h
 * Description : eflash driver header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __EFLASH_H__
#define __EFLASH_H__

#include "common.h"
#define PASS_H 0x90
#define PASS_L 0x00
#define FAIL_H 0x69
#define FAIL_L 0x00

#define ROM_DRIVER_FLASH

#define EFLASH_VERIFY_EN

#define EFLASH_BASE_ADDR 0x00000000

#define EFlashMainBaseAddr (EFLASH_BASE_ADDR + 0x00000000)
#define EFlashNVR2BaseAddr (EFLASH_BASE_ADDR + 0x00080200)

#define SM_FLASH_FF_VALUE_ADDR (EFlashNVR2BaseAddr + 0x64)  //flash?????FF?????????

#define EFC_RD_TIME 52  //uint??ns

#define PIN_BASE_ADDR 0x0007c000
//#define EFC_RD_WAIT 		        (8<<8)

#define EFC_WRITE_MODE (1 << 0)
#define EFC_PAGE_ERASE_MODE (1 << 1)
#define EFC_CHIP_ERASE_MODE (1 << 2)
#define EFC_DOUBLE_READ_EN (1 << 3)
#define EFC_PROGRAM_VRI_EN (1 << 4)
#define EFC_ERASE_VRI_EN (1 << 5)
#define EFC_ARCT_EN (1 << 6)
#define EFC_TIME_OUT_EN (1 << 7)

#define EFC_SLEEP0_EN (1 << 13)
#define EFC_SLEEP1_EN (1 << 14)
#define EFC_ERA_WRI_EN (1 << 15)

#define PagePerChip 1024
#define PAGE_SIZE 512

#define eflash_read_word(addr) (*(volatile UINT32*)(addr))      //read by word
#define eflash_read_halfword(addr) (*(volatile UINT16*)(addr))  //read by half word
#define eflash_read_byte(addr) (*(volatile UINT8*)(addr))       //read by byte

#ifdef ROM_DRIVER_FLASH
/************************************************************************
     * function   : eflash_write_word
     * Description: eflash write word
     * input : 
     *         UINT32 addr: address
     *         UINT32 value: value
     * return: none
     ************************************************************************/
#define eflash_write_word ((void (*)(UINT32, UINT32))(ROM_BASE_ADDR + 0x00001c39))
/************************************************************************
    * function   : eflash_erase_page
    * Description: eflash erase page
    * input : 
    *         UINT32 page_addr: page address
    * return: none
    ************************************************************************/
#define eflash_erase_page ((void (*)(UINT32))(ROM_BASE_ADDR + 0x00001c6b))  //?????????????????????????SM_FLASH_FF_VALUE_ADDR?????????

#else
/************************************************************************
	 * function   : eflash_write_word
	 * Description: eflash write word
	 * input : 
	 *         UINT32 addr: address
	 *         UINT32 value: value
	 * return: 0--success   1--fail
	 ************************************************************************/
UINT8 eflash_write_word(UINT32 addr, UINT32 value);

/************************************************************************
     * function   : eflash_erase_page
     * Description: eflash erase page
     * input : 
     *         UINT32 page_addr: page address
     * return: 0--success   1--fail
     ************************************************************************/
UINT8 eflash_erase_page(UINT32 page_addr);  //?????????????????????????SM_FLASH_FF_VALUE_ADDR?????????
#endif

/************************************************************************
 * function   : return_to_boot
 * Description: return to boot
 * input : none
 * return: none
 ************************************************************************/
#define return_to_boot ((void (*)(void))(ROM_BASE_ADDR + 0x000010a1))

void eflash_test(void);
void eflash_write_read_test(UINT32 base_addr, UINT32 value);
void eflash_page_erase_test(UINT32 base_addr);
int eflash_write_bytes(UINT32 addr, UINT8* pin_hash, UINT8 size);

uint8_t eflash_write_arr(uint32_t start_addr, uint32_t* arr, uint8_t arr_size);
uint8_t eflash_read_arr(uint32_t start_addr, uint32_t* arr, uint8_t arr_size);

#endif

