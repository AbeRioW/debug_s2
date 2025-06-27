
#ifndef __USB_H__
#define __USB_H__

#include  "common.h"

#define     HIGH_SPEED     1 
#define     FULL_SPEED     0 

#define     USB_EP0        0
#define     USB_EP1        1        
#define     USB_EP2        2      
#define     USB_EP3        3        
#define     USB_EP4        4   

#define     USB_EP_IN      USB_EP1
#define     USB_EP_OUT     USB_EP1 

#define     EP_DIR_IN      0x80  
#define     EP_DIR_OUT     0x00   

#define 	HID_REPORT_SIZE         1024

#define     EP0_MAX_PACKET_SIZE        64
#define     EPX_MAX_PACKET_SIZE_HS     512
#define     EPX_MAX_PACKET_SIZE_FS     64

#define 	USB_PACKET_NUM_HS   8  		//max value:8
#define 	USB_PACKET_NUM_FS   32   	//max value:32
#define     USB_FIFO_SIZE	9  //4096 bytes	

//#define 	USB_PACKET_NUM_HS   1   
//#define 	USB_PACKET_NUM_FS   1   	
//#define     USB_FIFO_SIZE	6  //512 bytes	
//#define     USB_FIFO_SIZE	3  //64 bytes	


typedef __packed struct _device_request
{
	UINT8  bmRequestType;
	UINT8  bRequest;
	UINT16 wValue;
	UINT16 wIndex;
	UINT16 wLength;
} DEVICE_REQUEST;



#define USB_BUS_RESET         0x01
#define USB_SUSPEND           0x02
#define USB_RESUME            0x04
#define USB_EP0_SETUP_PACKET  0x08
#define USB_EP1_OUT_PACKET    0x10
#define USB_EP2_OUT_PACKET    0x20
#define USB_EP3_OUT_PACKET    0x40
#define USB_EP4_OUT_PACKET    0x80


void usb_read_ep_fifo(UINT8 *dst,UINT32 length,UINT8 ep_index);
void usb_write_ep_fifo(UINT8 * src,UINT32 length, UINT8 ep_index);
void start_ep_transfer(UINT32 length,UINT8 ep_index);
void usb_send_data(UINT8 *buffer,UINT32 length,UINT8 ep_index);
void usb_receive_data(UINT8 *buffer,UINT32 length,UINT8 ep_index);
void usb_send_data_dynamic(UINT8 *buffer,UINT32 length,UINT8 ep_index);
void usb_receive_data_dynamic(UINT8 *buffer,UINT32 length,UINT8 ep_index);


void ep0_send_empty_packet(void);
void usb_clear_fifo(UINT8 ep_index, UINT8 ep_dir);
UINT16 usb_get_fifo_length(UINT8 ep_index);
void usb_ep0_send_stall(void);
void usb_clear_stall(UINT8 ep_index, UINT8 ep_dir);
void usb_send_stall(UINT8 ep_index, UINT8 ep_dir);
UINT16 get_stall_status(UINT8 ep_index, UINT8 ep_dir);
void usb_remote_wakeup(void);

extern volatile UINT8 flag_clear_stall;  
extern UINT8 speed_mode;
extern UINT8 out_ep_index;
extern UINT8 in_ep_index;


#endif

