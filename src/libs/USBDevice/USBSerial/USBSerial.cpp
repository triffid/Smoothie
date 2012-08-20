/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stdint.h"
#include "USBSerial.h"

USBSerial::USBSerial(USB *u): USBCDC(u), rxbuf(128), txbuf(128)
{
    usb = u;
}

int USBSerial::_putc(int c)
{
    send((uint8_t *)&c, 1);
    return 1;
}

int USBSerial::_getc()
{
    uint8_t c;
    while (rxbuf.isEmpty());
    rxbuf.dequeue(&c);
    return c;
}


bool USBSerial::writeBlock(uint8_t * buf, uint16_t size)
{
//     if(size > MAX_PACKET_SIZE_EPBULK) {
//         return false;
//     }
//     if(!send(buf, size)) {
//         return false;
//     }
    bool r = false;
    if (size <= txbuf.free())
    {
        r = true;
    }
    else
    {
        r = false;
        size= txbuf.free();
    }
    for (uint8_t i = 0; i < size; i++)
    {
        txbuf.queue(buf[i]);
    }
    return r;
}

bool USBSerial::USBEvent_EPIn(uint8_t bEP, uint8_t bEPStatus)
{
    if (bEP != CDC_BulkIn.bEndpointAddress)
        return false;

    uint8_t b[MAX_PACKET_SIZE_EPBULK];

    int l = txbuf.available();
    if (l > 0) {
        if (l > MAX_PACKET_SIZE_EPBULK)
            l = MAX_PACKET_SIZE_EPBULK;
        int i;
        for (i = 0; i < l; i++) {
            txbuf.dequeue(&b[i]);
        }
        send(b, l);
    }
    return true;
}

bool USBSerial::USBEvent_EPOut(uint8_t bEP, uint8_t bEPStatus)
{
    if (bEP != CDC_BulkOut.bEndpointAddress)
        return false;

    uint8_t c[65];
    uint32_t size = 0;

    //we read the packet received and put it on the circular buffer
    readEP(c, &size);
    for (uint8_t i = 0; i < size; i++) {
        rxbuf.queue(c[i]);
    }

    //call a potential handler
//     rx.call();

    // We reactivate the endpoint to receive next characters
    usb->readStart(CDC_BulkOut.bEndpointAddress, MAX_PACKET_SIZE_EPBULK);
    return true;
}

/*
bool USBSerial::EpCallback(uint8_t bEP, uint8_t bEPStatus) {
    if (bEP == CDC_BulkOut.bEndpointAddress) {
        uint8_t c[65];
        uint32_t size = 0;

        //we read the packet received and put it on the circular buffer
        readEP(c, &size);
        for (uint8_t i = 0; i < size; i++) {
            buf.queue(c[i]);
        }

        //call a potential handler
        rx.call();

        // We reactivate the endpoint to receive next characters
        usb->readStart(CDC_BulkOut.bEndpointAddress, MAX_PACKET_SIZE_EPBULK);
        return true;
    }
    return false;
}*/

uint8_t USBSerial::available() {
    return rxbuf.available();
}
