#include "usb.h"
#include "common.h"

static UINT16 ep1_stall[2] = {0};  // EP1 stall??
static UINT16 ep2_stall[2] = {0};  // EP2 stall??

volatile UINT8 flag_clear_stall = 0;

UINT16 usb_get_fifo_length(UINT8 ep_index) {
    REG_USBC_Eindex = ep_index;
    if (ep_index == 0) {
        return REG_USBC_E0COUNTR;
    }
    else {
        return REG_USBC_RXCOUNT;
    }
}

void usb_ep0_send_stall(void) {
    REG_USBC_Eindex = 0;
    REG_USBC_E0CSR |= 1 << 5;
    while (REG_USBC_E0CSR & 0x20)
        ;
    REG_USBC_E0CSR &= ~(1 << 2);  //clear stall status
}

UINT16 get_stall_status(UINT8 ep_index, UINT8 ep_dir) {
    switch (ep_index) {
        case USB_EP1: {
            if (ep_dir == EP_DIR_IN)
                return ep1_stall[0];  //in
            else
                return ep1_stall[1];  //out
        }
        case USB_EP2: {
            if (ep_dir == EP_DIR_IN)
                return ep2_stall[0];  //in
            else
                return ep2_stall[1];  //out
        }
        default: return 0xff;
    }
}

void usb_clear_stall(UINT8 ep_index, UINT8 ep_dir) {
    if (ep_index == 0)
        return;

    REG_USBC_Eindex = ep_index;
    if (ep_dir == EP_DIR_IN) {
        ep1_stall[0] = 0x0000;        //in
        REG_USBC_TxCSR |= 1 << 6;     //clear toggle
        REG_USBC_TxCSR &= ~(1 << 4);  //clear stall
    }
    else {
        ep1_stall[1] = 0x0000;        //out
        REG_USBC_RxCSR |= 1 << 7;     //clear toggle
        REG_USBC_RxCSR &= ~(1 << 5);  //clear stall
    }

    flag_clear_stall = 0;
}

void usb_send_stall(UINT8 ep_index, UINT8 ep_dir) {
    REG_USBC_Eindex = ep_index;

    if (ep_dir == EP_DIR_IN) {
        ep1_stall[0] = 0x0001;     //in
        REG_USBC_TxCSR |= 1 << 4;  //set stall
    }
    else {
        ep1_stall[1] = 0x0001;     //out
        REG_USBC_RxCSR |= 1 << 5;  //set stall
    }
}

#ifndef ROM_DRIVER_USB


void usb_read_ep_fifo(UINT8* dst, UINT32 length, UINT8 ep_index) {
    UINT32 i;

    for (i = 0; i < length; i++) {
        *dst++ = REG_USBC_FIFO_ENTRY(ep_index);
    }
}


void usb_write_ep_fifo(UINT8* src, UINT32 length, UINT8 ep_index) {
    UINT32 i;

    for (i = 0; i < length; i++) {
        REG_USBC_FIFO_ENTRY(ep_index) = *src++;
    }
}

void start_ep_transfer(UINT32 length, UINT8 ep_index) {
    REG_USBC_Eindex = ep_index;
    if (ep_index == USB_EP0) {
        REG_USBC_E0CSR |= 0x02;  //set the TxPktRdy
        while (REG_USBC_E0CSR & 0x02)
            ;
    }
    else {
        REG_USBC_TX_PTR = length;
        REG_USBC_TxCSR |= 0x01;  //set the TxPktRdy
        while (REG_USBC_TxCSR & 0x01)
            ;
    }
}

/*******************************************************************************
* Description    : Usb send data
* Input          : *buffer??Data to be sent
*										length??Length of data sent	
*										ep_index??The endpoint that sends the data
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void usb_send_data(UINT8* buffer, UINT32 length, UINT8 ep_index) {
    UINT16 max_packet_size;

    if (ep_index == USB_EP0) {
        max_packet_size = EP0_MAX_PACKET_SIZE;
    }
    else {
        max_packet_size = EPX_MAX_PACKET_SIZE_FS;
    }

    while (length >= max_packet_size) {
        usb_write_ep_fifo(buffer, max_packet_size, ep_index);
        start_ep_transfer(max_packet_size, ep_index);
        length -= max_packet_size;
        buffer += max_packet_size;
    }
    if (length > 0) {
        usb_write_ep_fifo(buffer, length, ep_index);
        start_ep_transfer(length, ep_index);
    }
}
/*******************************************************************************
* Description    : Usb  received data
* Input          : *buffer??Data to be receive
*										length??Length of data receive	
*										ep_index??The endpoint that received the data
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void usb_receive_data(UINT8* buffer, UINT32 length, UINT8 ep_index) {
    UINT32 len;

    REG_USBC_Eindex = ep_index;

    if (ep_index == USB_EP0) {
        while (length > 0) {
            while (!(REG_USBC_E0CSR & 0x01))
                ;

            len = REG_USBC_E0COUNTR;
            usb_read_ep_fifo(buffer, len, ep_index);

            REG_USBC_E0CSR |= 0x40;  //clear RxReady

            length -= len;
            buffer += len;
        }
        return;
    }

    while (length > 0) {
        REG_USBC_RxCSR &= (~0x01);
        while (!(REG_USBC_RxCSR & 0x01))
            ;

        len = REG_USBC_RXCOUNT;
        usb_read_ep_fifo(buffer, len, ep_index);

        length -= len;
        buffer += len;
    }

    REG_USBC_RxCSR &= (~0x01);  //?????????????????????
}

#endif
