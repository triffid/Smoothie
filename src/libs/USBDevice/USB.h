#ifndef _USB_HPP
#define _USB_HPP

#include "descriptor.h"

#ifndef N_DESCRIPTORS
#define N_DESCRIPTORS 32
#endif

#include "USBDevice.h"

class USB : public USBDevice {
    static usbdesc_base *descriptors[N_DESCRIPTORS];

    static usbdesc_device device;
    static usbdesc_configuration conf;
public:
    USB();
    USB(uint16_t idVendor, uint16_t idProduct, uint16_t bcdFirmwareRevision);

    void init(void);

    virtual bool USBEvent_busReset(void);
    virtual bool USBEvent_connectStateChanged(bool connected);
    virtual bool USBEvent_suspendStateChanged(bool suspended);

    virtual bool USBEvent_Request(CONTROL_TRANSFER&);
    virtual bool USBEvent_RequestComplete(CONTROL_TRANSFER&, uint8_t *, uint32_t);

    virtual bool USBEvent_EPIn(uint8_t, uint8_t);
    virtual bool USBEvent_EPOut(uint8_t, uint8_t);

    virtual bool USBCallback_setConfiguration(uint8_t configuration);
    virtual bool USBCallback_setInterface(uint16_t interface, uint8_t alternate);

    int addDescriptor(usbdesc_base *descriptor);
    int addDescriptor(void *descriptor);
//     int findDescriptor(uint8_t start, uint8_t type, uint8_t index, uint8_t alternate);
    int addInterface(usbdesc_interface *);
    int addEndpoint(usbdesc_endpoint *);
    int addString(void *);
    int getFreeEndpoint();
    int findStringIndex(uint8_t strid);

    void dumpDescriptors();
    void dumpDevice(usbdesc_device *);
    void dumpConfiguration(usbdesc_configuration *);
    void dumpInterface(usbdesc_interface *);
    void dumpEndpoint(usbdesc_endpoint *);
    void dumpString(int i);
    void dumpString(usbdesc_string *);
    void dumpCDC(uint8_t *);
};

#endif /* _USB_HPP */
