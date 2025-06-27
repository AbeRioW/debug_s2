/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : app.c
 * Description : application example source file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#include  "app.h"
#include  "usb.h"
#include  "usb_app.h"

__align(8) UINT32 g_data_buf[1024];

static BULK_IO_REQUEST bulk_io_request;
static UINT32 g_bulk_ack = 0x53425355;


void USB_CMD_proc(void)
{
    UINT32 length;
    UINT8  g_io_phase;
	
    length = usb_get_fifo_length(out_ep_index);
    /*-----------cmd stage-- CMD with 8 bytes ------------*/
    if(length != 8)
    {
        printfS("fifo length==%d \n", length);

        //usb_read_ep_fifo( &DATABUF[256], length, ep_index );
        //usb_clear_fifo(ep_in_index, EP_DIR_IN);
        //usb_send_stall(ep_in_index, EP_DIR_IN);
        //usb_send_stall(ep_index, EP_DIR_OUT);
        return;
    }
    else
    {
        usb_read_ep_fifo((UINT8 *)(&bulk_io_request), 8, out_ep_index);
    }


    length = (bulk_io_request.Length[2] << 16) + (bulk_io_request.Length[1] << 8) + (bulk_io_request.Length[0]);
    g_io_phase = bulk_io_request.BulkIoPhase;

    /*-----------data stage--------------*/
    if(g_io_phase == BulkIoPhase_Out)
    {
        printfS("usb data out \n");

		//usb_receive_data(USB_OUT_BUF, length, out_ep_index);
        usb_receive_data_dynamic(USB_OUT_BUF, length, out_ep_index);

		printfB8(USB_OUT_BUF,(length/4)*4+4);

    }
    else if(g_io_phase == BulkIoPhase_In)
    {
        printfS("usb data in \n");

        REG_USBC_RxCSR &= (~0x01);     //释放之前的 out包
		//usb_send_data(USB_OUT_BUF, length, in_ep_index);
        usb_send_data_dynamic(USB_OUT_BUF, length, in_ep_index);
    }

    /*-----------ACK stage--------------*/

    printfS("return ACK \n");
	
    if(out_ep_index==USB_EP1)		flag_usb_ep1_int=0;
    else if(out_ep_index==USB_EP2)	flag_usb_ep2_int=0;
    else if(out_ep_index==USB_EP3)	flag_usb_ep3_int=0;
	else							flag_usb_ep4_int=0;

    usb_send_data((UINT8 *)&g_bulk_ack, 4, in_ep_index);

} 
