#include "webusb.h"
#include "stdbool.h"

#define MS_OS_20_DESCRIPTOR_LENGTH (0xB2)

/**
 * @note  �������豸����洢 (BOS) �� USB 3.0 ������ĸ��
 *        ��Ҳ����Ϊ 2.1 ���һ���������ֲ�� USB 2.0 �豸��
 *        ������ WebUSB ��֧�������� BOS �������а�������ƽ̨����������
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
    // �������豸����洢������
    0x05,                           // bLength          �̳�Ϊ5
    0x0F,                           // bDescriptorType  �̶�Ϊ15
    0x39, 0x00,                     // wTotalLength     BOS���������ܴ�С
    0x02,                           // bNumDeviceCaps   BOS�������ж����豸��������������������
    
    // WebUSB ƽ̨����������
    0x18,                           // Descriptor size (24 bytes)
    0x10,                           // Descriptor type (Device Capability) �豸���������� 
    0x05,                           // Capability type (Platform) ƽ̨������
    0x00,                           // Reserved 

    // WebUSB Platform Capability ID (3408b638-09a9-47a0-8bfd-a0768815b665)
    // ƽ̨���� UUID ���˱�ʶΪWebUSB ƽ̨���������������ṩ�й��豸�Ļ�����Ϣ
    0x38, 0xB6, 0x08, 0x34,
    0xA9, 0x09,
    0xA0, 0x47,
    0x8B, 0xFD,
    0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65,
    
    0x00, 0x01,                     // WebUSB version 1.0
    WL_REQUEST_WEBUSB,              // Vendor-assigned WebUSB request code
    LANDING_PAGE,                           // Landing page
    
    // Microsoft ����ϵͳ������������
    0x1C,                           // Descriptor size (28 bytes)
    0x10,                           // Descriptor type (Device Capability)
    0x05,                           // Capability type (Platform)
    0x00,                           // Reserved
    
    0xDF, 0x60, 0xDD, 0xD8,         /* PlatformCapabilityUUID */
	0x89, 0x45, 0xC7, 0x4C,
	0x9C, 0xD2, 0x65, 0x9D,
	0x9E, 0x64, 0x8A, 0x9F,
    
	0x00, 0x00, 0x03, 0x06,         /* >= Win 8.1 * dwWindowsVersion    ��ͼ��� Windows �汾 */

    MS_OS_20_DESCRIPTOR_LENGTH, 0X00,   /* wDescriptorSetTotalLength */

    WL_REQUEST_WINUSB,            /* bVendorCode */
	0X00                          /* bAltEnumCode */
};


const uint8_t MS_OS_20_DESCRIPTOR_SET[] = {
    // Microsoft OS 2.0 ����������ͷ
    0x0A,0x00,                          // Descriptor size (10 bytes)
    0x00,0x00,                          // MS OS 2.0 descriptor set header
    0x00,0x00,0x03,0x06,                // Windows version (8.1) (0x06030000)
    MS_OS_20_DESCRIPTOR_LENGTH,0x00,    // Size, MS OS 2.0 descriptor set
   
    // Microsoft OS 2.0 �����Ӽ���ͷ
    0x08,0x00,                  // wLength
    0x01,0x00,                  // wDescriptorType
    0x00,                       // ���������� 1
    0x00,                       // bReserved
    0XA8,0X00,                  // Size, MS OS 2.0 configuration subset
    
    // Microsoft OS 2.0 �����Ӽ�ͷ
    0x08, 0x00,  // Descriptor size (8 bytes)
    0x02, 0x00,  // MS OS 2.0 function subset header
    0x02,        // ��2���ӿ�
    0x00,        // ��������Ϊ 0
    0xA0, 0x00,
    
    // Microsoft OS 2.0 ���� ID ������
    // ���� ID ���������� Windows ���豸�� WinUSB �����������
    0x14, 0x00,             // wLength  20
    0x03, 0x00,             // MS_OS_20_FEATURE_COMPATIBLE_ID
    'W',  'I',  'N',  'U',  'S',  'B',  0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    // Microsoft OS 2.0 ע�������������
    // ע������Է����豸�ӿ� GUID
    0x84, 0x00,             //wLength: 132
    0x04, 0x00,             // wDescriptorType: MS_OS_20_FEATURE_REG_PROPERTY: 0x04 (Table 9)
    0x07, 0x00,             //wPropertyDataType: REG_MULTI_SZ (Table 15)
    0x2a, 0x00,             //wPropertyNameLength: 
    //bPropertyName: ��DeviceInterfaceGUID��
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00, 
    'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 
    0x00, 0x00,
    0x50, 0x00,             // wPropertyDataLength
    //bPropertyData: ��{975F44D9-0D08-43FD-8B3E-127CA8AFFF9D}��.
    '{', 0x00, '9', 0x00, 'd', 0x00, '7', 0x00, 'd', 0x00, 'e', 0x00, 'b', 0x00, 'b', 0x00, 'c', 0x00, '-', 0x00, 
    'c', 0x00, '8', 0x00, '5', 0x00, 'd', 0x00, '-', 0x00, '1', 0x00, '1', 0x00, 'd', 0x00, '1', 0x00, '-', 0x00, 
    '9', 0x00, 'e', 0x00, 'b', 0x00, '4', 0x00, '-', 0x00, '0', 0x00, '0', 0x00, '6', 0x00, '0', 0x00, '0', 0x00, 
    '8', 0x00, 'c', 0x00, '3', 0x00, 'a', 0x00, '1', 0x00, '9', 0x00, 'a', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00   
}; 

// ��ַ������
const uint8_t USBD_WebUSBURLDescriptor[]=
{
    0x1C,                       // Size of this descriptor
    WEBUSB_URL_TYPE,            // bDescriptorType
    WEBUSB_URL_SCHEME_HTTPS,    // URL scheme prefix
    WEBUSB_URL_STRINGS          // ���ӵ�ַ UTF-8 ����� URL ����
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







