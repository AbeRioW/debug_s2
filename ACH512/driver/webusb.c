#include "webusb.h"
#include "stdbool.h"

#define MS_OS_20_DESCRIPTOR_LENGTH (0xB2)

/**
 * @note  二进制设备对象存储 (BOS) 是 USB 3.0 中引入的概念，
 *        但也已作为 2.1 版的一部分向后移植到 USB 2.0 设备。
 *        声明对 WebUSB 的支持首先在 BOS 描述符中包含以下平台功能描述符
 *        https://web.dev/build-for-webusb/
 */
 
/**
 * @note  LANDING_PAGE 
 *  - 1 open
 *  - 0 close
 */
#define LANDING_PAGE 0x00 
const uint8_t USBD_BinaryObjectStoreDescriptor[]=
{
    // 二进制设备对象存储描述符
    0x05,                           // bLength          固长为5
    0x0F,                           // bDescriptorType  固定为15
    0x39, 0x00,                     // wTotalLength     BOS描述符的总大小
    0x02,                           // bNumDeviceCaps   BOS描述符中独立设备功能特性描述符的数量
    
    // WebUSB 平台能力描述符
    0x18,                           // Descriptor size (24 bytes)
    0x10,                           // Descriptor type (Device Capability) 设备功能描述符 
    0x05,                           // Capability type (Platform) 平台描述符
    0x00,                           // Reserved 

    // WebUSB Platform Capability ID (3408b638-09a9-47a0-8bfd-a0768815b665)
    // 平台功能 UUID 将此标识为WebUSB 平台功能描述符，它提供有关设备的基本信息
    0x38, 0xB6, 0x08, 0x34,
    0xA9, 0x09,
    0xA0, 0x47,
    0x8B, 0xFD,
    0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65,
    
    0x00, 0x01,                     // WebUSB version 1.0
    WL_REQUEST_WEBUSB,              // Vendor-assigned WebUSB request code
    LANDING_PAGE,                           // Landing page
    
    // Microsoft 操作系统兼容性描述符
    0x1C,                           // Descriptor size (28 bytes)
    0x10,                           // Descriptor type (Device Capability)
    0x05,                           // Capability type (Platform)
    0x00,                           // Reserved
    
    0xDF, 0x60, 0xDD, 0xD8,         /* PlatformCapabilityUUID */
	0x89, 0x45, 0xC7, 0x4C,
	0x9C, 0xD2, 0x65, 0x9D,
	0x9E, 0x64, 0x8A, 0x9F,
    
	0x00, 0x00, 0x03, 0x06,         /* >= Win 8.1 * dwWindowsVersion    最低兼容 Windows 版本 */

    MS_OS_20_DESCRIPTOR_LENGTH, 0X00,   /* wDescriptorSetTotalLength */

    WL_REQUEST_WINUSB,            /* bVendorCode */
	0X00                          /* bAltEnumCode */
};


const uint8_t MS_OS_20_DESCRIPTOR_SET[] = {
    // Microsoft OS 2.0 描述符集标头
    0x0A,0x00,                          // Descriptor size (10 bytes)
    0x00,0x00,                          // MS OS 2.0 descriptor set header
    0x00,0x00,0x03,0x06,                // Windows version (8.1) (0x06030000)
    MS_OS_20_DESCRIPTOR_LENGTH,0x00,    // Size, MS OS 2.0 descriptor set
   
    // Microsoft OS 2.0 配置子集标头
    0x08,0x00,                  // wLength
    0x01,0x00,                  // wDescriptorType
    0x00,                       // 适用于配置 1
    0x00,                       // bReserved
    0XA8,0X00,                  // Size, MS OS 2.0 configuration subset
    
    // Microsoft OS 2.0 函数子集头
    0x08, 0x00,  // Descriptor size (8 bytes)
    0x02, 0x00,  // MS OS 2.0 function subset header
    0x02,        // 第2个接口
    0x00,        // 必须设置为 0
    0xA0, 0x00,
    
    // Microsoft OS 2.0 兼容 ID 描述符
    // 兼容 ID 描述符告诉 Windows 此设备与 WinUSB 驱动程序兼容
    0x14, 0x00,             // wLength  20
    0x03, 0x00,             // MS_OS_20_FEATURE_COMPATIBLE_ID
    'W',  'I',  'N',  'U',  'S',  'B',  0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    // Microsoft OS 2.0 注册表属性描述符
    // 注册表属性分配设备接口 GUID
    0x84, 0x00,             //wLength: 132
    0x04, 0x00,             // wDescriptorType: MS_OS_20_FEATURE_REG_PROPERTY: 0x04 (Table 9)
    0x07, 0x00,             //wPropertyDataType: REG_MULTI_SZ (Table 15)
    0x2a, 0x00,             //wPropertyNameLength: 
    //bPropertyName: “DeviceInterfaceGUID”
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00, 
    'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 
    0x00, 0x00,
    0x50, 0x00,             // wPropertyDataLength
    //bPropertyData: “{975F44D9-0D08-43FD-8B3E-127CA8AFFF9D}”.
    '{', 0x00, '9', 0x00, 'd', 0x00, '7', 0x00, 'd', 0x00, 'e', 0x00, 'b', 0x00, 'b', 0x00, 'c', 0x00, '-', 0x00, 
    'c', 0x00, '8', 0x00, '5', 0x00, 'd', 0x00, '-', 0x00, '1', 0x00, '1', 0x00, 'd', 0x00, '1', 0x00, '-', 0x00, 
    '9', 0x00, 'e', 0x00, 'b', 0x00, '4', 0x00, '-', 0x00, '0', 0x00, '0', 0x00, '6', 0x00, '0', 0x00, '0', 0x00, 
    '8', 0x00, 'c', 0x00, '3', 0x00, 'a', 0x00, '1', 0x00, '9', 0x00, 'a', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00   
}; 

// 网址描述符
const uint8_t USBD_WebUSBURLDescriptor[]=
{
    0x1C,                       // Size of this descriptor
    WEBUSB_URL_TYPE,            // bDescriptorType
    WEBUSB_URL_SCHEME_HTTPS,    // URL scheme prefix
    WEBUSB_URL_STRINGS          // 链接地址 UTF-8 编码的 URL 内容
};


void USBD_EndPoint0_Setup_WinUSB_ReqToDevice(DEVICE_REQUEST req)
{
    uint8_t *pD=0;
    uint32_t len=0;

    bool sucess=false;
    
    if(req.wIndex == WINUSB_REQUEST_DESCRIPTOR)
    {
        usb_send_data((uint8_t *)(MS_OS_20_DESCRIPTOR_SET), sizeof(MS_OS_20_DESCRIPTOR_SET), USB_EP0);      
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
}

void USBD_EndPoint0_Setup_WebUSB_ReqToDevice(DEVICE_REQUEST req)
{
    if(req.wIndex == WEBUSB_REQUEST_GET_URL)
    {
                
        if(req.wValue != 0x00)
        {
            usb_send_data((uint8_t *)(USBD_WebUSBURLDescriptor), sizeof(USBD_WebUSBURLDescriptor), USB_EP0);
            REG_USBC_E0CSR |= 0x08;  //set DataEnd
        }
    }
}







