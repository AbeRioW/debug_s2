/***********************************************************************
 * Copyright (c)  2008 - 2016, Xiamen Toyonway Intellectual Co.,Ltd .
 * All rights reserved.
 * Filename    : divider.h
 * Description : divider header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __DIVIDER_H__
#define __DIVIDER_H__

#include "common.h"

/******************************************************************************
* Function Name  : hardwareNN_Div_q32
* Description    : Computes q = b div c and a = b mod c
                   cDigits must be 1, and *c < 0xffff
* Input          : - *b          : dividend
                 : bDigits       : word length of *b
                 : *c            : divisor
				         : cDigits       : word length of *c
* Output         : - *q          : quotient
				         : - *a          : remainder
* Return         : NONE
******************************************************************************/
extern void hardwareNN_Div_q32(UINT32 *q,UINT32 *a,UINT32 *b,UINT32 bDigits,UINT32 *c,UINT32 cDigits);

#endif
