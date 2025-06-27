/************************************************************************
 *Updated date:2019-12-25
 ************************************************************************/

#ifndef __USB_APP_H__
#define __USB_APP_H__

#include  "common.h"
extern UINT32 g_data_buf[];
#define DATABUF             											  ((UINT8 *)g_data_buf )
#define USBBUF_EP1_IN  	                            DATABUF
#define USBBUF_EP1_OUT                             (DATABUF+64)
#define USBBUF_EP2_IN  	                           (DATABUF+128)
#define USBBUF_EP2_OUT                             (DATABUF+192)
#define USB_OUT_BUF   	                           (DATABUF+256)
#define USB_IN_BUF   	                             (DATABUF+256)
#define DATABUF_HID_EP0														 (DATABUF+64)
/*--------------from usb1.1 protocal------------------*/
#define USB_REMOTE_WAKEUP                               0xA0

#define USB_REQUEST_MASK			                    			0x0F
#define USB_REQUEST_TYPE_MASK                           0x60
#define USB_STANDARD_REQUEST                            0x00	//???????
#define USB_CLASS_REQUEST                               0x20	//??????
#define USB_VENDOR_REQUEST                              0x40	//????????

#define USB_RECIPIENT_MASK                              0x1F // 接收
#define USB_RECIPIENT_DEVICE                            0x00 // 设备
#define USB_RECIPIENT_INTERFACE                         0x01 // 接口
#define USB_RECIPIENT_ENDPOINT                          0x02 // 端点

/* USB Feature selectors */
#define USB_FEATURE_ENDPOINT_STALL                      0
#define USB_FEATURE_REMOTE_WAKEUP                       1
#define USB_FEATURE_TEST_MODE                           2

/* USB GET_STATUS Bit Values */
#define USB_GETSTATUS_SELF_POWERED                      0x01
#define USB_GETSTATUS_REMOTE_WAKEUP                     0x02
#define USB_GETSTATUS_ENDPOINT_STALL                    0x01


//usb Descriptor type
#define USB_DEVICE_DESCRIPTOR_TYPE		                	0x01
#define	USB_CONFIGURATION_DESCRIPTOR_TYPE	            	0x02
#define	USB_STRING_DESCRIPTOR_TYPE		                	0x03
#define	USB_INTERFACE_DESCRIPTOR_TYPE	            		0x04
#define	USB_ENDPOINT_DESCRIPTOR_TYPE	            		0x05
#define	USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE	       	 	0x06
#define	USB_OTHER_SPEED_CONFIGURATION_DESCRIPTOR_TYPE		0x07
#define USB_INTERFACE_POWER_DESCRIPTOR_TYPE	                0x08

//#define USB_BINARY_OBJECT_STORE_DESCRIPTOR_TYPE             15

// defined USB device classes
#define USB_DEVICE_CLASS_RESERVED                       0x00
#define USB_DEVICE_CLASS_AUDIO                          0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS                 0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE                0x03
#define USB_DEVICE_CLASS_MONITOR                        0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE             0x05
#define USB_DEVICE_CLASS_POWER                          0x06
#define USB_DEVICE_CLASS_PRINTER                        0x07
#define USB_DEVICE_CLASS_STORAGE                        0x08
#define USB_DEVICE_CLASS_HUB                            0x09
#define USB_CLASS_CODE_TEST_CLASS_DEVICE                0xDC
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC                0xFF

#define USB_SUBCLASS_CODE_TEST_CLASS                    0xA0
#define USB_PROTOCOL_CODE_TEST_CLASS                    0xB0

#define INDEX_LANGUAGE_ID                               0x00 // ???Index????0??????????????????????豸??????????????ID??????????Table 9-15
#define INDEX_MANUFACTURER                              0x01
#define INDEX_PRODUCT                                   0x02
#define INDEX_SERIALNUMBER	                            0x03
#define INDEX_WEBUSBNUMBER	                            0x04

// Standard Request Codes
#define USB_REQUEST_GET_STATUS                          0x00
#define USB_REQUEST_CLEAR_FEATURE                       0x01
#define USB_REQUEST_SET_FEATURE                         0x03
#define USB_REQUEST_SET_ADDRESS                         0x05
#define USB_REQUEST_GET_DESCRIPTOR                      0x06
#define USB_REQUEST_SET_DESCRIPTOR                      0x07
#define USB_REQUEST_GET_CONFIGURATION                   0x08
#define USB_REQUEST_SET_CONFIGURATION                   0x09
#define USB_REQUEST_GET_INTERFACE                       0x0A
#define USB_REQUEST_SET_INTERFACE                       0x0B
#define USB_REQUEST_SYNC_FRAME                          0x0C

// Class Request Codes
#define GET_MAX_LUN                                     0xFE
#define Bulk_Only_Mass_Storage_Reset                    0xFF

// Scsi Command Operation Code List.
#define OPCODE_TEST_UNIT_READY                          0x00
#define OPCODE_REQUEST_SENSE                            0x03
#define OPCODE_MODE_SELECT6                             0x15
#define OPCODE_MODE_SENSE6                              0x1A
#define OPCODE_INQUIRY                                  0x12
#define OPCODE_START_STOP_UNIT                          0x1B
#define OPCODE_PREVENT_REMOVAL                          0x1E
#define OPCODE_Read_Format_Capacity                     0x23
#define OPCODE_READ_CAPACITY                            0x25
#define OPCODE_READ10                                   0x28
#define OPCODE_WRITE10                                  0x2A
#define OPCODE_VERIFY10                                 0x2F
#define OPCODE_MODE_SENSE10                             0x5A

#define BulkIoPhase_In		                        			0x80
#define BulkIoPhase_Out 	                        			0x00
#define BulkIoSignature		                        			0x43425355
#define BULK_IO_ACK			                        				0x53425355

typedef struct _bulk_io_request
{
    UINT32 dBulkIoSignature; 
    UINT8  BulkIoPhase;    
    UINT8  Length[3];         
} BULK_IO_REQUEST;


extern volatile UINT8 flag_usb_int;
extern volatile UINT8 flag_usb_ep1_int;
extern volatile UINT8 flag_usb_ep2_int;
extern volatile UINT8 flag_usb_enum_finish;
extern UINT8 out_ep_index;
extern UINT8 in_ep_index;

void usb_initial(void);
void usb_monitor(void);
void usb_transfer_monitor(void);
void usb_receive_data(UINT8 *buffer,UINT32 length,UINT8 ep_index);
void usb_send_data(UINT8 *buffer,UINT32 length,UINT8 ep_index);
void usb_hid_fuction(void);
void usb_u2f_fuction(void);

typedef enum {
    USB_CHANNEL_U2F,
    USB_CHANNEL_HID,
} usb_channel_t;

typedef void (*data_reveived_callback_t)(usb_channel_t channel, UINT8* buffer, UINT16 size);
void usb_set_data_reveived_callback(data_reveived_callback_t callback);

#endif

