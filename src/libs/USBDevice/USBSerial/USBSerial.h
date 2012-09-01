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

#ifndef USBSERIAL_H
#define USBSERIAL_H

#include "USBCDC.h"
// #include "Stream.h"
#include "CircBuffer.h"

// #include "../../../../gcc4mbed/external/mbed/FunctionPointer.h"


class USBSerial_Receiver {
protected:
    virtual bool SerialEvent_RX(void) = 0;
};
/**
* USBSerial example
*
* @code
* #include "mbed.h"
* #include "USBSerial.h"
*
* //Virtual serial port over USB
* USBSerial serial;
*
* int main(void) {
*
*    while(1)
*    {
*        serial.printf("I am a virtual serial port\n");
*        wait(1);
*    }
* }
* @endcode
*/
class USBSerial: public USBCDC, public USBSerial_Receiver {
public:

    /**
    *   Constructor
    *
    * @param vendor_id Your vendor_id (default: 0x1f00)
    * @param product_id Your product_id (default: 0x2012)
    * @param product_release Your preoduct_release (default: 0x0001)
    *
    */
    USBSerial(USB *);


    /**
    * Send a character. You can use puts, printf.
    *
    * @param c character to be sent
    * @returns true if there is no error, false otherwise
    */
    virtual int _putc(int c);

    /**
    * Read a character: blocking
    *
    * @returns character read
    */
    virtual int _getc();

    /**
    * Check the number of bytes available.
    *
    * @returns the number of bytes available
    */
    uint8_t available();

    /**
    * Write a block of data.
    *
    * For more efficiency, a block of size 64 (maximum size of a bulk endpoint) has to be written.
    *
    * @param buf pointer on data which will be written
    * @param size size of the buffer. The maximum size of a block is limited by the size of the endpoint (64 bytes)
    *
    * @returns number of bytes successfully buffered
    */
    uint16_t writeBlock(uint8_t * buf, uint16_t size);

    /**
     *  Attach a member function to call when a packet is received.
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     */
//     template<typename T>
//     void attach(T* tptr, void (T::*mptr)(void)) {
//         if((mptr != NULL) && (tptr != NULL)) {
//             rx.attach(tptr, mptr);
//         }
//     }

    /**
     * Attach a callback called when a packet is received
     *
     * @param fptr function pointer
     */
//     void attach(void (*fn)(void)) {
//         if(fn != NULL) {
//             rx.attach(fn);
//         }
//     }

    CircBuffer<uint8_t> rxbuf;
    CircBuffer<uint8_t> txbuf;

protected:
//     virtual bool EpCallback(uint8_t, uint8_t);
    virtual bool USBEvent_EPIn(uint8_t, uint8_t);
    virtual bool USBEvent_EPOut(uint8_t, uint8_t);

    virtual bool SerialEvent_RX(void){return false;};


private:
    USB *usb;
//     mbed::FunctionPointer rx;
};

#endif
