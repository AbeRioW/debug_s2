#include  "usb.h"
#include  "usb_app.h"
#include  "usb_desc.h"

// �豸������
USB_DEVICE_DESCRIPTOR device_descr =
    {
        0x12,
        USB_DEVICE_DESCRIPTOR_TYPE,  //bDescriptorType: DEVICE
        0x0210,                      //bcdUSB: usb 2.0

        //USB_CLASS_CODE_TEST_CLASS_DEVICE,		//bDeviceClass: independent interfaces
        0x00,
        0x00,                    //bDeviceSubClass: 0
        0x00,                    //bDeviceProtocol: class specific protocols NOT used on device basis
        EP0_MAX_PACKET_SIZE,     //bMaxPacketSize0: maximum packet size for endpoint zero

        0x6A64,               //idVendor	
        0x0821,               //idProduct       

        0x0100,               //bcdDevice
        INDEX_MANUFACTURER,   //iManufacturer: index of string
        INDEX_PRODUCT,        //iProduct: index of manufacturer string
        INDEX_SERIALNUMBER,   //iSerialNumber: index of product string
        0x01                  //bNumConfigurations: 1 configuration
};

// �豸�޶���������
USB_DEVICE_QUALIFIER_DESCRIPTOR device_qualifier_descr =
{
        0x0a,
        USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE,  //bDescriptorType: DEVICE
        SWAP(0x0210),                          //bcdUSB: usb 2.0

        //USB_CLASS_CODE_TEST_CLASS_DEVICE,		//bDeviceClass: independent interfaces
        0,                    // Mass Storage Class

        0,                    //bDeviceSubClass: 0
        0,                    //bDeviceProtocol: class specific protocols NOT used on device basis
        EP0_MAX_PACKET_SIZE,  //bMaxPacketSize0: maximum packet size for endpoint zero
        0x01,                 //bNumConfigurations: 1 configuration
        0x00
};

// ����������
const UINT8 usb_descriptor_config[82] =
{
        //usb config descriptor
        0x09,
        0x02,
        0x52,       // wTotalLength
        0x00,
        0x03,       // bNumInterfaces�� �ӿ�����
        0x01,       // bConfigurationValue: 0x01 is used to select this configuration
        0x00,       // iConfiguration: �ַ�������
        0xc0,
        0x32,
              
        //usb interface 0 descriptor
        0x09,
        0x04,
        0x00,
        0x00,
        0x02,
        0x03,
        0x00,
        0x00,
        0x02,

        //usb interface 0 hid descriptor
        0x09, /*bLength: HID Descriptor size*/
        0x21, /*bDescriptorType: HID*/
        0x11, /*bcdHID: HID Class Spec release number*/
        0x01,
        0x00, /*bCountryCode: Hardware target country*/
        0x01, /*bNumDescriptors: Number of HID class descriptors to follow*/
        0x22, /*bDescriptorType*/
        0x22, /*wItemLength: Total length of Report descriptor*/
        0x00,

        //usb Endpoint 2 intput descriptor
        0x07,
        0x05,
        0x81,
        0x03,
        0x40,
        0x00,
        0x01,

        //usb Endpoint 2 output descriptor
        0x07,
        0x05,
        0x01,
        0x03,
        0x40,
        0x00,
        0x01,

        //usb interface 1 descriptor
        0x09,
        0x04,
        0x01,
        0x00,
        0x02,
        0x03,
        0x01,
        0x01,
        0x02,

        //usb interface 1 hid descriptor
        0x09, /*bLength: HID Descriptor size*/
        0x21, /*bDescriptorType: HID*/
        0x11, /*bcdHID: HID Class Spec release number*/
        0x01,
        0x21,  /*bCountryCode: Hardware target country*/
        0x01,  /*bNumDescriptors: Number of HID class descriptors to follow*/
        0x22,  /*bDescriptorType*/
        0x22,  /*wItemLength: Total length of Report descriptor*/
        0x00,  //50-58

        //usb Endpoint 1 intput descriptor
        0x07,
        0x05,
        0x82,
        0x03,
        0x40,
        0x00,
        0x01,

        //usb Endpoint 2 output descriptor
        0x07,
        0x05,
        0x02,
        0x03,
        0x40,
        0x00,
        0x01,

        //WEBUSB interface 2 descriptor 
        0x09,   //�豸���������ֽ�����С��Ϊ0x09 
        0x04,   //���������ͱ�ţ�Ϊ0x04
        0x02,   //�ӿڵı�� 
        0x00,   //���õĽӿ����������
        0x00,   //�ýӿ�ʹ�ö˵������������˵�0 
        0xff,   //�ӿ����� 
        0X00,   //�ӿ������� 
        0x00,   //�ӿ�����ѭ��Э��
        0x00    //�����ýӿڵ��ַ�������ֵ
};

const UINT8 Vendor_HID_ReportDescriptor[34] =
    {
        0x06,
        0xa0,
        0xff,
        0x09,
        0x01,
        0xa1,
        0x01,
        0x09,
        0x03,
        0x15,
        0x00,
        0x26,
        0xff,
        0x00,
        0x75,
        0x08,
        0x95,
        0x40,
        0x81,
        0x08,
        0x09,
        0x04,
        0x15,
        0x00,
        0x26,
        0xff,
        0x00,
        0x75,
        0x08,
        0x95,
        0x40,
        0x91,
        0x08,
        0xc0,
};

const UINT8 Fido_HID_ReportDescriptor[34] =
    {
        0x06,
        0xd0,
        0xf1,
        0x09,
        0x01,
        0xa1,
        0x01,
        0x09,
        0x03,
        0x15,
        0x00,
        0x26,
        0xff,
        0x00,
        0x75,
        0x08,
        0x95,
        0x40,
        0x81,
        0x08,
        0x09,
        0x04,
        0x15,
        0x00,
        0x26,
        0xff,
        0x00,
        0x75,
        0x08,
        0x95,
        0x40,
        0x91,
        0x08,
        0xc0,
};

uint8_t USBD_HID_Desc[9] =
    {
        /* 18 */
        0x09, /*bLength: HID Descriptor size*/
        0x21, /*bDescriptorType: HID*/
        0x11, /*bcdHID: HID Class Spec release number*/
        0x01,
        0x00, /*bCountryCode: Hardware target country*/
        0x01, /*bNumDescriptors: Number of HID class descriptors to follow*/
        0x22, /*bDescriptorType*/
        0x22, /*wItemLength: Total length of Report descriptor*/
        0x00,
};

UINT8 StrDesc_LanguageID[4] =
    {
        4,     // Num bytes of this descriptor
        3,     // String descriptor
        0x09,  // Language ID LSB
        0x04   // Language ID
};

UINT8 StrDesc_Manufacturer[16] =
    {
        7 * 2 + 2,  // Num bytes of this descriptor
        3,          // String descriptor
        'C',
        0,
        'H',
        0,
        'I',
        0,
		'P',
        0,
        'T',
        0,
        'E',
        0,
		'K',
        0,
};

UINT8 StrDesc_Product[20] =
    {
        9 * 2 + 2,  // Num bytes of this descriptor
        3,          // String descriptor
        'W',
        0,
        'a',
        0,
        'l',
        0,
        'l',
        0,
        'e',
        0,
		't',
        0,
		' ',
        0,
		'S',
        0,
		'2',
        0,
};

UINT8 StrDesc_SerialNumber[10] =
    {
        4 * 2 + 2,  // Num bytes of this descriptor
        3,          // String descriptor
        '0',
        0,
        '0',
        0,
        '0',
        0,
        '1',
        0,
};
    
UINT8 StrDesc_WebusbNumber[14] =
    {
        6 * 2 + 2,  // Num bytes of this descriptor
        3,          // String descriptor
        'W',
        0,
        'E',
        0,
        'B',
        0,
        'U',
        0,
        'S',
        0,
        'B',
        0
};



