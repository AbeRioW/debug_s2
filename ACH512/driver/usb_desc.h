/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : usb_desc.h
 * Description : usb driver header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/

#ifndef __USBDESC_H__
#define __USBDESC_H__

#include "common.h"
/**********************************************************
*	definitions
**********************************************************/

/**********************************************************
*	include files
**********************************************************/

/**********************************************************
*	structure
**********************************************************/

typedef struct _USB_DEVICE_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT16 bcdUSB;
    UINT8 bDeviceClass;
    UINT8 bDeviceSubClass;
    UINT8 bDeviceProtocol;
    UINT8 bMaxPacketSize0;
    UINT16 idVendor;
    UINT16 idProduct;
    UINT16 bcdDevice;
    UINT8 iManufacturer;
    UINT8 iProduct;
    UINT8 iSerialNumber;
    UINT8 bNumConfigurations;
} USB_DEVICE_DESCRIPTOR;

typedef struct USB_DEVICE_QUALIFIER_DESCRIPTOR {
    BYTE bLength;
    BYTE bDescriptorType;
    WORD bcdUSB;
    BYTE bDeviceClass;
    BYTE bDeviceSubClass;
    BYTE bDeviceProtocol;
    BYTE bMaxPacketSize0;
    BYTE bNumConfigurations;
    BYTE bReserved;
} USB_DEVICE_QUALIFIER_DESCRIPTOR, *PUSB_DEVICE_QUALIFIER_DESCRIPTOR;

extern const UINT8 usb_descriptor_config[82];

/**********************************************************
*	extern variables
**********************************************************/
extern USB_DEVICE_DESCRIPTOR device_descr;
extern USB_DEVICE_QUALIFIER_DESCRIPTOR device_qualifier_descr;
extern UINT8 StrDesc_LanguageID[4];
extern UINT8 StrDesc_Manufacturer[16];
extern UINT8 StrDesc_Product[20];
extern UINT8 StrDesc_SerialNumber[10];
extern const UINT8 Vendor_HID_ReportDescriptor[34];
extern const UINT8 Fido_HID_ReportDescriptor[34];
extern uint8_t USBD_HID_Desc[9];
extern UINT8 UsbStrDescSerialNumber[34];

extern UINT8 StrDesc_WebusbNumber[14];
#endif

