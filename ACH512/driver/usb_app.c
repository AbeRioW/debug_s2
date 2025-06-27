/************************************************************************
 *Updated date:2020-04-26
 ************************************************************************/
#include "usb_app.h"
#include "usb.h"
#include "usb_desc.h"
#include "webusb.h"

static DEVICE_REQUEST dev_req;
static UINT8 config_value = 0;
static UINT8 AltSetting = 0;
static UINT16 usb_device_status = 0;  // ??????????????

static data_reveived_callback_t data_reveived_callback;

volatile UINT8 flag_usb_int = 0;
volatile UINT8 flag_usb_ep1_int = 0;
volatile UINT8 flag_usb_ep2_int = 0;
volatile UINT8 flag_usb_enum_finish = 0;

UINT8 out_ep_index = 0;
UINT8 in_ep_index = 0;

__align(8) UINT32 g_data_buf[512];

void USB_IRQHandler(void) {
    UINT8 temp = 0;
    UINT8 backup_index;

    backup_index = REG_USBC_Eindex;

    temp = REG_USBC_IntrUSB;

    if (temp & 0x01)  //suspend
    {
        flag_usb_int |= USB_SUSPEND;
    }
    if (temp & 0x02)  //resume
    {
        flag_usb_int |= USB_RESUME;
    }
    if (temp & 0x04)  //bus reset
    {
        flag_usb_int |= USB_BUS_RESET;
    }

    temp = REG_USBC_IntrTx;

    if (temp & 0x01)  //EP0
    {
        flag_usb_int |= USB_EP0_SETUP_PACKET;
    }
    else {
        temp = REG_USBC_IntrRx;

        if (temp & 0x02)  //EP1
        {
            flag_usb_ep1_int = USB_EP1_OUT_PACKET;
        }
        if (temp & 0x04)  //EP2
        {
            flag_usb_ep2_int = USB_EP2_OUT_PACKET;
        }
    }

    usb_transfer_monitor();

    usb_monitor();

    REG_USBC_Eindex = backup_index;
}

/*----------------- ???????--------------------*/
void usb_get_status(void) {
    UINT8 recipient = 0;
    UINT16 status = 0;
    UINT8 bEpIndex = 0;
    UINT8 bEpDir = 0;
    UINT16 ep_status = 0;

    recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
    if (recipient == USB_RECIPIENT_DEVICE) { // 设备
        usb_write_ep_fifo((UINT8*)(&usb_device_status), 2, USB_EP0);
        start_ep_transfer(2, USB_EP0);
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
    else if (recipient == USB_RECIPIENT_INTERFACE) { // 接口
        usb_write_ep_fifo((UINT8*)(&status), 2, USB_EP0);
        start_ep_transfer(2, USB_EP0);
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
    else if (recipient == USB_RECIPIENT_ENDPOINT) { // 端点
        bEpIndex = dev_req.wIndex & 0x0f;
        bEpDir = dev_req.wIndex & 0x80;
        ep_status = get_stall_status(bEpIndex, bEpDir);

        usb_write_ep_fifo((UINT8*)(&ep_status), 2, USB_EP0);
        start_ep_transfer(2, USB_EP0);
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
    else
        usb_ep0_send_stall();
}

void usb_clear_feature(void) {
    UINT8 recipient = 0;
    UINT8 bEpIndex = 0;
    UINT8 bEpDir = 0;

    recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
    if (recipient == USB_RECIPIENT_DEVICE) {
        if (dev_req.wValue == USB_FEATURE_REMOTE_WAKEUP) {
            usb_device_status &= (~USB_GETSTATUS_REMOTE_WAKEUP);
        }

        REG_USBC_E0CSR |= 0x08;
    }
    else if (recipient == USB_RECIPIENT_ENDPOINT) {
        bEpIndex = dev_req.wIndex & 0x0f;
        bEpDir = dev_req.wIndex & 0x80;
        usb_clear_stall(bEpIndex, bEpDir);

        REG_USBC_E0CSR |= 0x08;  //set DataEnd bit

        flag_clear_stall = 1;
    }
    else
        usb_ep0_send_stall();
}

void usb_set_feature(void) {
    UINT8 recipient = 0;
    UINT8 bEpIndex = 0;
    UINT8 bEpDir = 0;

    recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
    if (recipient == USB_RECIPIENT_DEVICE) {
        if (dev_req.wValue == USB_FEATURE_REMOTE_WAKEUP) {
            usb_device_status |= USB_GETSTATUS_REMOTE_WAKEUP;
        }
        if (dev_req.wValue == 0x02) {
            switch (dev_req.wIndex >> 8) {
                case 0x01:  //test_J
                    REG_USBC_Testmode = 0x02;
                    break;

                case 0x02:  //test_K
                    REG_USBC_Testmode = 0x4;
                    break;

                case 0x03:  //test_SE0_NAK
                    REG_USBC_Testmode = 0x1;
                    break;

                case 0x04:                   //test_Packet
                    REG_USBC_E0CSR |= 0x02;  //set the TxPktRdy
                    REG_USBC_Testmode = 0x08;
                    break;

                case 0x05:  //test_Force_HS_Enable
                    REG_USBC_Testmode = 0x10;
                    break;
            }
        }
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
    else if (recipient == USB_RECIPIENT_ENDPOINT) {
        bEpIndex = dev_req.wIndex & 0x0f;
        bEpDir = dev_req.wIndex & 0x80;
        usb_send_stall(bEpIndex, bEpDir);
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
    else
        usb_ep0_send_stall();
}

void usb_set_address(void) {
    REG_USBC_FADDRR = dev_req.wValue;

    REG_USBC_E0CSR |= 0x08;  //set DataEnd
}

void usb_get_configuration(void) {
    usb_send_data(&config_value, 1, USB_EP0);

    REG_USBC_E0CSR |= 0x08;  //set DataEnd
}

// 設置配製
void usb_set_configuration(void) {
    if ((dev_req.wValue == 0) || (dev_req.wValue == 1)) {
        config_value = dev_req.wValue;
    }
    else
        usb_ep0_send_stall();
}

void usb_get_interface(void) {
    UINT8 inter_value = 0;
    UINT8 recipient = 0;

    recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
    if (recipient == USB_RECIPIENT_INTERFACE) {
        inter_value = AltSetting + dev_req.wIndex;
        usb_send_data(&inter_value, 1, USB_EP0);
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
    else
        usb_ep0_send_stall();
}

void usb_set_interface(void) {
    UINT8 recipient = 0;

    recipient = dev_req.bmRequestType & USB_RECIPIENT_MASK;
    if (recipient == USB_RECIPIENT_INTERFACE) {
        AltSetting = dev_req.wValue;
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
    else
        usb_ep0_send_stall();
}

void usb_get_description(void) {
    UINT16 wValue;
    UINT16 wLength;
    UINT8 wValue_bIndex;
    UINT8 interface_Index;

    wValue = (dev_req.wValue >> 8);
    wValue_bIndex = dev_req.wValue & 0xff;
    wLength = dev_req.wLength;
    interface_Index = dev_req.wIndex & 0xff;

    if (wValue == USB_DEVICE_DESCRIPTOR_TYPE)
    { 
        if (wLength > device_descr.bLength)
        {
            wLength = device_descr.bLength;
        }
            usb_send_data((UINT8*)(&device_descr), wLength, USB_EP0);
            REG_USBC_E0CSR |= 0x08;  //set DataEnd        
    }

    else if (wValue == USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE) {
        if (wLength > device_qualifier_descr.bLength) {
            wLength = device_qualifier_descr.bLength;
        }
        usb_send_data((UINT8*)(&device_qualifier_descr), wLength, USB_EP0);
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
    else if (wValue == 0x22) {
        if (interface_Index == 0x00) {
            if (wLength > sizeof(Vendor_HID_ReportDescriptor)) {
                wLength = sizeof(Vendor_HID_ReportDescriptor);
            }
            usb_send_data((UINT8*)(&Vendor_HID_ReportDescriptor), wLength, USB_EP0);
            REG_USBC_E0CSR |= 0x08;  //set DataEnd
        }
        else if (interface_Index == 0x01) {
            if (wLength > sizeof(Fido_HID_ReportDescriptor)) {
                wLength = sizeof(Fido_HID_ReportDescriptor);
            }
            usb_send_data((UINT8*)(&Fido_HID_ReportDescriptor), wLength, USB_EP0);
            REG_USBC_E0CSR |= 0x08;  //set DataEnd
            flag_usb_enum_finish = 1;
        }
    }
    else if (wValue == 0x21) {
        if (wLength > sizeof(USBD_HID_Desc)) {
            wLength = sizeof(USBD_HID_Desc);
        }
        usb_send_data((UINT8*)(&USBD_HID_Desc), wLength, USB_EP0);
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }

    else if (wValue == USB_CONFIGURATION_DESCRIPTOR_TYPE) {
        if (wLength > sizeof(usb_descriptor_config)) {
            wLength = sizeof(usb_descriptor_config);
        }
        usb_send_data((UINT8*)(&usb_descriptor_config), wLength, USB_EP0);
        REG_USBC_E0CSR |= 0x08;  //set DataEnd
    }
    else if (wValue == USB_STRING_DESCRIPTOR_TYPE) {
        if (wValue_bIndex == INDEX_LANGUAGE_ID)  //LANGID string
        {
            if (wLength > StrDesc_LanguageID[0]) {
                wLength = StrDesc_LanguageID[0];
            }
            usb_send_data(StrDesc_LanguageID, wLength, USB_EP0);
            REG_USBC_E0CSR |= 0x08;  //set DataEnd
        }
        else if (wValue_bIndex == INDEX_MANUFACTURER)  //vender string
        {
            if (wLength > StrDesc_Manufacturer[0]) {
                wLength = StrDesc_Manufacturer[0];
            }
            usb_send_data(StrDesc_Manufacturer, wLength, USB_EP0);
            REG_USBC_E0CSR |= 0x08;  //set DataEnd
        }
        else if (wValue_bIndex == INDEX_PRODUCT)  //product string
        {
            if (wLength > StrDesc_Product[0]) {
                wLength = StrDesc_Product[0];
            }
            usb_send_data(StrDesc_Product, wLength, USB_EP0);
            REG_USBC_E0CSR |= 0x08;  //set DataEnd
        }
        else if (wValue_bIndex == INDEX_SERIALNUMBER)  //SerialNumber	string
        {
            if (wLength > StrDesc_SerialNumber[0]) {
                wLength = StrDesc_SerialNumber[0];
            }
            usb_send_data(StrDesc_SerialNumber, wLength, USB_EP0);
            REG_USBC_E0CSR |= 0x08;  //set DataEnd
        }
        else if(wValue_bIndex == INDEX_WEBUSBNUMBER)
        {
            if (wLength > StrDesc_WebusbNumber[0]) {
                wLength = StrDesc_WebusbNumber[0];
            }
            usb_send_data(StrDesc_WebusbNumber, wLength, USB_EP0);
            REG_USBC_E0CSR |= 0x08;  //set DataEnd        
        }
        else {
            usb_ep0_send_stall();
        }
    }
    else if(wValue==USB_BINARY_OBJECT_STORE_DESCRIPTOR_TYPE)
    {
        uint8_t *pD;
        pD= (uint8_t *)USBD_BinaryObjectStoreDescriptor;
        
        wLength=((USB_BINARY_OBJECT_STORE_DESCRIPTOR *)pD)->wTotalLength;
        
        usb_send_data((uint8_t *)USBD_BinaryObjectStoreDescriptor, wLength, USB_EP0);
        REG_USBC_E0CSR |= 0x08;  //set DataEnd  
    }
    
    else {
        usb_ep0_send_stall();
    }
}

void reserved(void) {
    usb_ep0_send_stall();
}

/*************************************************************************
 * USB????豸?????????????
 * ??????????4??bitλ??????16????????????16?????
 *************************************************************************/
void (*StandardDeviceRequest[])(void) =
    {
        usb_get_status,     //0x00
        usb_clear_feature,  //0x01
        reserved,
        usb_set_feature,  //0x03
        reserved,
        usb_set_address,      //0x05
        usb_get_description,  //0x06
        reserved,
        usb_get_configuration,  //0x08
        usb_set_configuration,  //0x09
        usb_get_interface,      //0x0a
        usb_set_interface,      //0x0b
        reserved,
        reserved,
        reserved,
        reserved};

//*************************************************************************
//HID??(class)?????????????
//??????????4??bitλ??????16????????????16?????
//*************************************************************************

void get_report(void) {
    usb_send_data(DATABUF_HID_EP0, HID_REPORT_SIZE, USB_EP0);
    REG_USBC_E0CSR |= 0x08;  //set DataEnd
}

void set_report(void) {
    usb_receive_data(DATABUF_HID_EP0, HID_REPORT_SIZE, USB_EP0);
    REG_USBC_E0CSR |= 0x08;  //set DataEnd
}

void get_idle(void) {
    REG_USBC_E0CSR |= 0x08;  //set DataEnd
}

void get_protocol(void) {
    REG_USBC_E0CSR |= 0x08;  //set DataEnd
}

void set_idle(void) {
    REG_USBC_E0CSR |= 0x08;  //set DataEnd
}

void set_protocol(void) {
    REG_USBC_E0CSR |= 0x08;  //set DataEnd
}

void (*ClassRequest[])(void) =
    {
        reserved,
        get_report,    //???????
        get_idle,      //???
        get_protocol,  //????boot device ???
        reserved,
        reserved,
        reserved,
        reserved,
        reserved,
        set_report,    //?????ж???interrupt out?????????
        set_idle,      //???
        set_protocol,  //????boot device ???
};

void usb_control_transfer(void) {
    UINT8 type, req;

    REG_USBC_Eindex = 0;

    if (REG_USBC_E0COUNTR == 8) {
        usb_read_ep_fifo((UINT8*)(&dev_req), 8, USB_EP0);
        REG_USBC_E0CSR |= 0x40;  //set the ServicedRxPktRdy

        type = dev_req.bmRequestType & USB_REQUEST_TYPE_MASK;
        req = dev_req.bRequest & USB_REQUEST_MASK;
        if (type == USB_STANDARD_REQUEST)  //???????
        {
            (*StandardDeviceRequest[req])();
        }
        else if (type == USB_CLASS_REQUEST)  //??????
        {
            (*ClassRequest[req])();
        }
        else if (type == USB_VENDOR_REQUEST)
        {
            switch(dev_req.bRequest)
            {
                case WL_REQUEST_WEBUSB:
                    USBD_EndPoint0_Setup_WebUSB_ReqToDevice(dev_req);
                break;
                
                case WL_REQUEST_WINUSB:
                    USBD_EndPoint0_Setup_WinUSB_ReqToDevice(dev_req);
                break;
           
                default:break;
            }  
        }
        else
            usb_ep0_send_stall();
    }
}

void usb_bus_reset(void) {
    REG_USBC_INTRTXE = 0x01;  //disable TX interrupt,enable EP0 interrupt
    REG_USBC_INTRRXE = 0x1E;  //enable EP1/4 RX interrupt

    REG_USBC_Eindex = USB_EP1;
    REG_USBC_RxCSR |= 0x1000;  //close NYET
    REG_USBC_RxFIFO_ADD = (UINT32)USBBUF_EP1_OUT >> 3;

    REG_USBC_Eindex = USB_EP1;
    REG_USBC_TxFIFO_ADD = (UINT32)USBBUF_EP1_IN >> 3;

    REG_USBC_Eindex = USB_EP2;
    REG_USBC_RxCSR |= 0x1000;  //close NYET
    REG_USBC_RxFIFO_ADD = (UINT32)USBBUF_EP2_OUT >> 3;

    REG_USBC_Eindex = USB_EP2;
    REG_USBC_TxFIFO_ADD = (UINT32)USBBUF_EP2_IN >> 3;

    REG_USBC_Eindex = USB_EP1;
    REG_USBC_RxFIFO_SIZE = 0x03;  //Rx FIFO size:64byte
    REG_USBC_RXPSZR = EPX_MAX_PACKET_SIZE_FS;

    REG_USBC_Eindex = USB_EP1;
    REG_USBC_TxFIFO_SIZE = 0x03;  //Tx FIFO size:64byte
    REG_USBC_TXPSZR = EPX_MAX_PACKET_SIZE_FS;

    REG_USBC_Eindex = USB_EP2;
    REG_USBC_RxFIFO_SIZE = 0x03;  //Rx FIFO size:64byte
    REG_USBC_RXPSZR = EPX_MAX_PACKET_SIZE_FS;

    REG_USBC_Eindex = USB_EP2;
    REG_USBC_TxFIFO_SIZE = 0x03;  //Tx FIFO size:64byte
    REG_USBC_TXPSZR = EPX_MAX_PACKET_SIZE_FS;

    REG_USBC_Eindex = 0;
}

void usb_resume(void) {
    REG_USBC_UCSR &= ~0x01;  // clear suspend mode enable bit
}

void usb_suspend(void) {
    REG_USBC_UCSR |= 0x01;  // set into suspend mode
}

void usb_monitor(void) {
    if ((flag_usb_int & USB_BUS_RESET))  //bus reset
    {
        flag_usb_int &= (~USB_BUS_RESET);

        usb_bus_reset();
    }

    if ((flag_usb_int & USB_SUSPEND))  //suspend
    {
        flag_usb_int &= (~USB_SUSPEND);

        usb_suspend();
    }

    if ((flag_usb_int & USB_RESUME))  //resume
    {
        flag_usb_int &= (~USB_RESUME);

        usb_resume();
    }

    if ((flag_usb_int & USB_EP0_SETUP_PACKET))  //EP0 setup packet received
    {
        flag_usb_int &= (~USB_EP0_SETUP_PACKET);
        usb_control_transfer();
    }
}

void usb_transfer_monitor(void) {
    if (flag_usb_ep1_int == USB_EP1_OUT_PACKET)  //EP1 OUT packet received
    {
        REG_USBC_Eindex = 1;
        out_ep_index = USB_EP1;
        in_ep_index = USB_EP1;

         usb_hid_fuction();
    }

    if (flag_usb_ep2_int == USB_EP2_OUT_PACKET)  //EP2 OUT packet received
    {
        REG_USBC_Eindex = 2;
        out_ep_index = USB_EP2;
        in_ep_index = USB_EP2;

       usb_u2f_fuction();
    }
}

void usb_initial(void) {
#ifdef LOW_POWER
    enable_module(BIT_USB);  //enable USB
#endif

    NVIC_ClearPendingIRQ(USB_IRQn);
    NVIC_EnableIRQ(USB_IRQn);

    REG_SCU_USBPHYCSR = (REG_SCU_USBPHYCSR & 0xffffe1ff) | (0x01 << 9);  //??????????

    REG_USBC_UCSR &= (~0x40);  //disconnect usb
    delay(1000);

    REG_USBC_IntrUSBE = 0x07;  // enable Reset,Resume, Suspend interrupt
    REG_USBC_UCSR = 0x40;      //bit6 Soft Conn 5 HS enable
                               //REG_USBC_UCSR = 0x40;
}

void usb_remote_wakeup(void) {
    REG_USBC_UCSR |= 1 << 2;     // set into remote waleup mode
    delay(3870 * 8);             //2~15ms
    REG_USBC_UCSR &= ~(1 << 2);  // set into remote waleup mode
}

void usb_hid_fuction(void) {
    INT16 length;

    length = usb_get_fifo_length(out_ep_index);

    if (length == 0)
        return;

    usb_read_ep_fifo(USB_IN_BUF, length, out_ep_index);
    if (out_ep_index == USB_EP1)
        flag_usb_ep1_int = 0;

    if (((USB_IN_BUF[2] >> 4) == 0x0f) || ((USB_IN_BUF[2] >> 4) == 0x0c)) {
        //vendor_cmd_usb_transport();
    }
    else {
        REG_USBC_RxCSR &= (~0x01);
        if (data_reveived_callback) {
            data_reveived_callback(USB_CHANNEL_HID, USB_IN_BUF, length);
        }
    }
}

void usb_u2f_fuction(void) {
    INT16 length;

    length = usb_get_fifo_length(out_ep_index);

    if (length == 0)
        return;

    usb_read_ep_fifo(USB_IN_BUF, length, out_ep_index);
    if (out_ep_index == USB_EP2)
        flag_usb_ep2_int = 0;

    REG_USBC_RxCSR &= (~0x01);
    
    if (data_reveived_callback) {
        data_reveived_callback(USB_CHANNEL_U2F, USB_IN_BUF, length);
    }
}

void usb_set_data_reveived_callback(data_reveived_callback_t callback) {
    data_reveived_callback = callback;
}
