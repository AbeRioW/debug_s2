/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : config.h
 * Description : config header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#define DEBUG    //printf调试接口使能

#define DEBUG_UART    UARTA  //调试打印的端口
 
#ifdef DEBUG
#define printfS     printf
#define printfB8    printf_buff_byte
#define printfB32   printf_buff_word	 
#else
#define	printfS(format, ...)	     ((void)0)
#define	printfB8(buff, byte_len)	 ((void)0)
#define	printfB32(buff, word_len)	 ((void)0)
#endif

/*--------------- 时钟设置 ----------------------- */
#define FCLK 110  //配置core时钟/HCLK ,可配置为：6 12 30 48 50 60 70 80 90 100 110 等 (uint:MHz)
  
/*--------------- uart设置 ----------------------- */
//通信格式采用：8位数据位，1位停止位，无校验位

#define UART_BAUD_RATE	115200
//#define UART_Tx_INT_MODE   // Tx采用中断方式  （程序中Rx始终采用中断方式）
//#define UARTB_USE_CTSMODE    //CTS mode
//#define UARTB_USE_RTSMODE	 //RTS mode


#define MAX_IN_BUFFER_LEN 255
#define MAX_OUT_BUFFER_LEN 255
#define MAX_PATH_LEN 5

#define DRIVER_SYNC 0x3c

#define APDU_DEFAULT_CLA 0xff

#define SYS_INS_VERSION     0
#define SYS_INS_HASH        1
#define SYS_INS_SIGN        2
#define SYS_INS_PUB_KEY     3
#define SYS_INS_SEED        10 // 0x0A
#define SYS_INS_AUTH        11 //0x0B 系统权限
#define SYS_INS_TEST_USSD   12 // 未使用
#define SYS_INS_DEVICE      13 // 0x0D
#define SYS_INS_UNAUTH      20 // 0x14
#define SYS_INS_PIN         31 //0x1F 创建PIN码
#define SYS_INS_FP          32 // 0x20 采集指纹

#define SYS_INS_DEBUG_LOG  100

#define IO_ASYNCH_REPLY 0x01
#define IO_IGNORE 0x01 << 1

#define ERROR_CODE_INTERNAL_ERROR                  1
#define ERROR_CODE_UNKNOWN_INSTRUCTION             2
#define ERROR_CODE_INVALID_PARAMS                  3
#define ERROR_CODE_PATH_DEPTH_EXCEEDED             4
#define ERROR_CODE_TIMEOUT                         5
#define ERROR_CODE_PERMISSION_DENIED               6

#define PIN_CODE_LENGTH 8
#endif
