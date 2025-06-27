/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.h
 * Description : application example header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#ifndef __APP_H__
#define __APP_H__

#include "common.h"

#define DATABUF             ((UINT8 *)g_data_buf )

//memory config

///*#define USB_FIFO_EP1_IN  	   DATABUF
//#define USB_FIFO_EP1_OUT       DATABUF
//#define USB_FIFO_EP2_IN  	  (DATABUF+ 0x1000)  //4KB
//#define USB_FIFO_EP2_OUT      (DATABUF+ 0x1000)
//#define USB_FIFO_EP3_IN  	  (DATABUF+ 0x2000)
//#define USB_FIFO_EP3_OUT      (DATABUF+ 0x2000)
//#define USB_FIFO_EP4_IN  	  (DATABUF+ 0x3000)
//#define USB_FIFO_EP4_OUT      (DATABUF+ 0x3000)	*/

////sram 64KB�����2KB�����USB EP fifo�� ÿ��EPռ��512B
//#define USB_FIFO_EP1_IN  	  (0x10000-0x200)
//#define USB_FIFO_EP1_OUT      (0x10000-0x200)
//#define USB_FIFO_EP2_IN  	  (0x10000-0x400)
//#define USB_FIFO_EP2_OUT      (0x10000-0x400)

//#define USB_FIFO_EP3_IN  	  (0x10000-0x600)
//#define USB_FIFO_EP3_OUT      (0x10000-0x600)
//#define USB_FIFO_EP4_IN  	  (0x10000-0x800)
//#define USB_FIFO_EP4_OUT      (0x10000-0x800)

////#define USB_OUT_BUF   	DATABUF
////#define USB_IN_BUF   	    DATABUF[512]

////17KB��g_data_buf�����5·Ӧ�õ�����buff�� EP1~4 ÿ·Ӧ�ø�ռ��4KB��EP0Ӧ��ռ��1KB
//#define DATABUF_UMS1   		DATABUF
//#define DATABUF_UMS2   		(DATABUF+0x1000)  
//#define DATABUF_CCID   		(DATABUF+0x2000)
//#define DATABUF_HID_EP4		(DATABUF+0x3000)
//#define DATABUF_HID_EP0		(DATABUF+0x4000)  

extern UINT32 g_data_buf[];

void USB_CMD_proc(void);

#endif

