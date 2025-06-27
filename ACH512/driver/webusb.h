#ifndef __WEBUSB_H
#define __WEBUSB_H

#include "common.h"
#include "usb.h"


#define WL_REQUEST_WEBUSB    (0x22)  // WebUSB descriptors.
#define WL_REQUEST_WINUSB    (0x21)  // WinUSB descriptor.

#define WINUSB_REQUEST_DESCRIPTOR (0x07)

#define USB_BINARY_OBJECT_STORE_DESCRIPTOR_TYPE (15)


/* WebUSB Request Codes */
#define WEBUSB_REQUEST_GET_URL                  0x02

#define WEBUSB_URL_TYPE                         0x03

/* bScheme in URL descriptor */
#define WEBUSB_URL_SCHEME_HTTP                  0x00
#define WEBUSB_URL_SCHEME_HTTPS                 0x01

// …Ë±∏¡¥Ω”Õ¯÷∑
#define WEBUSB_URL_STRINGS  \
's','o','w','b','u','g','.','g','i','t','h','u','b','.','i','o','/','w', 'e', 'b', 'l', 'i', 'g', 'h', 't'

/* WebUSB Platform Capability Descriptor */
typedef __packed struct _WEBUSB_PLATFORM_CAPABILITY_DESCRIPTOR {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDevCapabilityType;
    uint8_t  bReserved;
    uint8_t  platformCapabilityUUID[16];
    uint16_t bcdVersion;
    uint8_t  bVendorCode;
    uint8_t  iLandingPage;
} WEBUSB_PLATFORM_CAPABILITY_DESCRIPTOR;

typedef __packed struct _WEBUSB_URL_DESCRIPTOR {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bScheme;
    char URL[];
} WEBUSB_URL_DESCRIPTOR;

/* USB Binary Object Store Descriptor */
typedef __packed struct _USB_BINARY_OBJECT_STORE_DESCRIPTOR {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wTotalLength;
    uint8_t  bNumDeviceCaps;
} USB_BINARY_OBJECT_STORE_DESCRIPTOR;

/* WinUSB Microsoft OS 2.0 descriptor set header */
typedef __packed struct _WINUSB_DESCRIPTOR_SET_HEADER {
    uint16_t wLength;
    uint16_t wDescriptorType;
    uint32_t dwWindowsVersion;
    uint16_t wTotalLength;
} WINUSB_DESCRIPTOR_SET_HEADER;



extern const uint8_t USBD_BinaryObjectStoreDescriptor[];

void USBD_EndPoint0_Setup_WinUSB_ReqToDevice(DEVICE_REQUEST req);
void USBD_EndPoint0_Setup_WebUSB_ReqToDevice(DEVICE_REQUEST req);

#endif /* __WEBUSB_H */

