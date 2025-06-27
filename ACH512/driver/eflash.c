/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : eflash.c
 * Description : eflash driver source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#include "eflash.h"
#include "uart.h"

#ifndef ROM_DRIVER_FLASH
/************************************************************************
 * function   : eflash_write_word
 * Description: eflash write word
 * input : 
 *         UINT32 addr: address
 *         UINT32 value: value
 * return: 0--success   1--fail 
 ************************************************************************/
UINT8 eflash_write_word(UINT32 addr, UINT32 value) {
    UINT8 vf;
    REG_EFC_CTRL |= EFC_WRITE_MODE;
#    ifdef EFLASH_VERIFY_EN
    REG_EFC_CTRL |= EFC_PROGRAM_VRI_EN;
#    endif
    REG_EFC_SEC = 0x55AAAA55;
    *((volatile UINT32*)(addr)) = value;
    while (!(REG_EFC_STATUS & 0x01))
        ;
    REG_EFC_CTRL &= ~EFC_WRITE_MODE;
    vf = 0;

#    ifdef EFLASH_VERIFY_EN
    while (!(REG_EFC_INTSTATUS & (0x01 << 4)))
        ;
    REG_EFC_INTSTATUS = (0x01 << 4);
    if (REG_EFC_INTSTATUS & (0x01 << 6))  //vf error
    {
        REG_EFC_INTSTATUS = (0x01 << 6);
        vf = 1;
    }
    REG_EFC_CTRL &= ~EFC_PROGRAM_VRI_EN;
#    endif

    return vf;
}

/************************************************************************
 * function   : eflash_erase_page
 * Description: eflash erase page
 * input : 
 *         UINT32 page_addr: page address
 * return: 0--success   1--fail
 ************************************************************************/
UINT8 eflash_erase_page(UINT32 page_addr) {
    UINT8 vf;

    REG_EFC_CTRL |= EFC_PAGE_ERASE_MODE;
    REG_EFC_SEC = 0x55AAAA55;
    *((volatile UINT32*)(page_addr)) = 0;
    while (!(REG_EFC_STATUS & 0x01))
        ;
    REG_EFC_CTRL &= ~EFC_PAGE_ERASE_MODE;
    vf = 0;

#    ifdef EFLASH_VERIFY_EN
    REG_EFC_ADCT = (page_addr) >> 2;
    REG_EFC_CTRL |= EFC_ERASE_VRI_EN;
    while (!(REG_EFC_INTSTATUS & (0x01 << 4)))
        ;
    REG_EFC_INTSTATUS = (0x01 << 4);
    if (REG_EFC_INTSTATUS & (0x01 << 3))  //vf error
    {
        REG_EFC_INTSTATUS = (0x01 << 3);
        vf = 1;
    }
#    endif

    return vf;
}
#endif

void eflash_page_erase_test(UINT32 base_addr) {
    UINT32 page, start_page, end_page, addr, i, result;

    start_page = 100;
    end_page = 110;

    for (page = start_page; page < end_page; page++) {
        addr = base_addr + page * PAGE_SIZE;

#ifdef ROM_DRIVER_FLASH
        eflash_erase_page(addr);  //rom里面的页擦除没有返回值
#else
        if (eflash_erase_page(addr))  //如果返回1，则擦除校验错误
        {
            printfS("page Erase Verify Error\n");
            return;
        }
#endif

        for (i = 0; i < (PAGE_SIZE / 4); i++) {
            result = eflash_read_word(addr);
            if (result != (*(volatile UINT32*)(SM_FLASH_FF_VALUE_ADDR)))
            //if(result != 0xffffffff)
            {
                printfS("page Erase Error: addr= 0x%x, result= 0x%x \r\n", addr, result);
                return;
            }

            addr += 4;
        }
        printfS("page Erase Test Pass, page = %d\n", page);
    }
}

int eflash_write_bytes(UINT32 addr, UINT8* pin_hash, UINT8 size) {
    int ret = 0;
    UINT32 value = 0;
    UINT32 result = 0;
    UINT8 i;
    for (i = 0; i < size; i += 4) {
        value = *(pin_hash + i) << 24 | *(pin_hash + i + 1) << 16 | *(pin_hash + i + 2) << 8 | *(pin_hash + i + 3);
        eflash_write_word(addr, value);
        result = eflash_read_word(addr);
        printfS("result = 0x%x\n", result);
        if (result != value) {
            printfS("write/word read error: addr= 0x%x, result= 0x%x \n", addr, result);
            return ret;
        }
        addr += 4;
    }

    ret = 1;
    return ret;
}

void eflash_write_read_test(UINT32 base_addr, UINT32 value) {
    UINT32 page, start_page, end_page, addr, i, result;

    printfS("write/read test value= 0x%x \n", value);

    start_page = 100;
    end_page = 110;

    for (page = start_page; page < end_page; page++) {
        addr = base_addr + page * PAGE_SIZE;
        eflash_erase_page(addr);
        for (i = 0; i < (PAGE_SIZE / 4); i++) {
#ifdef ROM_DRIVER_FLASH
            eflash_write_word(addr, value);
#else
            if (eflash_write_word(addr, value)) {
                printfS("write word verify Error\n");
                return;
            }
#endif

            result = eflash_read_word(addr);
            printfS("result = 0x%x\n", result);
            printfS("i = %d\n", i);
            if (result != value) {
                printfS("write/word read error: addr= 0x%x, result= 0x%x \n", addr, result);
                return;
            }
            result = eflash_read_halfword(addr);
            if (result != (value & 0xffff)) {
                printfS("write/halfword read error: addr= 0x%x, result= 0x%x \n", addr, result);
                return;
            }
            result = eflash_read_byte(addr);
            if (result != (value & 0xff)) {
                printfS("write/byte read error: addr= 0x%x, result= 0x%x \n", addr, result);
                return;
            }
            addr += 4;
        }
        printfS("Write/Read Test Pass, page = %d\n", page);
    }
}

void eflash_test(void) {
    printfS("-----start eflash test-------\n");

    printfS("------main area: word write->word|halfword|byte read test------\n");
    eflash_write_read_test(EFlashMainBaseAddr, 0x13223344);

    printfS("-----eflash test over-------\n");
}

uint8_t eflash_write_arr(uint32_t start_addr, uint32_t* arr, uint8_t arr_size) {
    size_t i = 0;
    for (; i < arr_size; i++) {
        eflash_write_word(start_addr + i * 4, arr[i]);
    }
    return i;
}

uint8_t eflash_read_arr(uint32_t start_addr, uint32_t* arr, uint8_t arr_size) {
    size_t i = 0;
    for (; i < arr_size; i++) {
        arr[i] = eflash_read_word(start_addr + i * 4);
    }
    return i;
}

