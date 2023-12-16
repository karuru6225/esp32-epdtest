/**
 *  @filename   :   epd3in0g.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Waveshare
 *
 *  Copyright (C) Waveshare     2022/7/22
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include "epd3in0g.h"

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    WIDTH = EPD_WIDTH;
    HEIGHT = EPD_HEIGHT;
    IfInit();
};

int Epd::Init() {
    Serial.println("Init");
    Reset();

    SendCommand(0x66);
    SendData(0x49);
    SendData(0x55);
    SendData(0x13);
    SendData(0x5D);
    SendData(0x05);
    SendData(0x10);

    SendCommand(0xB0);
    SendData(0x00);//1 boost

    SendCommand(0x01);
    SendData(0x0F);
    SendData(0x00);

    SendCommand(0x00);
    SendData(0x4F);
    SendData(0x6B);

    SendCommand(0x06);
    SendData(0xD7);
    SendData(0xDE);
    SendData(0x12);

    SendCommand(0x61);
    SendData(0x00);
    SendData(0xA8);
    SendData(0x01); 
    SendData(0x90); 

    SendCommand(0x50); // border color
    SendData(0x37); // white
    // SendData(0x17); // black
    // SendData(0x57); // yellow
    // SendData(0x77); // red
    // SendData(0x97); // floating

    SendCommand(0x60);
    SendData(0x0C); 
    SendData(0x05);

    SendCommand(0xE3);
    SendData(0xFF);

    SendCommand(0x84);
    SendData(0x00);

    PowerOn();
    delay(1000);
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command) {
    // char* buf;
    // switch(command) {
    //   case 0x02:
    //     buf = "\nC POFF ";
    //     break;
    //   case 0x04:
    //     buf = "\nC P ON ";
    //     break;
    //   case 0x07:
    //     buf = "\nC DSLP ";
    //     break;
    //   case 0x10:
    //     buf = "\nC DTRS ";
    //     break;
    //   case 0x12:
    //     buf = "\nC DREF ";
    //     break;
    //   default:
    //     buf = "\nC UNKN ";
    // }
    // sprintf(buf, "\nC 0x%02x ", command);
    // Serial.print(buf);
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data) {
    // char buf[10];
    // sprintf(buf, "%02x", data);
    // Serial.print(buf);
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes LOW
 */
void Epd::ReadBusyH(char* reason) {
    Serial.print("\nbusy H ");
    Serial.print(reason);
    while(DigitalRead(busy_pin) == LOW) {      //LOW: busy, HIGH: idle
        DelayMs(5);
    } 
    Serial.print(" release\n");
}

void Epd::Refresh(void) {
  SendCommand(0x50);
  SendData(0x37);
  SendCommand(0x12);
  SendData(0x00);
  DelayMs(1);
  ReadBusyH("Refresh");
  DelayMs(12000);
}

void Epd::RefreshFloating(void) {
  SendCommand(0x50);
  SendData(0x97);
  SendCommand(0x12);
  SendData(0x00);
  DelayMs(1);
  ReadBusyH("Refresh");
  DelayMs(12000);
}

void Epd::PowerOff(void) {
  // Serial.println("PowerOff");
  SendCommand(0x02);
  SendData(0x00);
  ReadBusyH("PowerOff");
}

void Epd::PowerOn(void) {
  Serial.println("PowerOn");
  SendCommand(0x04);
  ReadBusyH("PowerOn");
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);    
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);
    ReadBusyH("Reset");
}

void Epd::setFullScreen(void) {
  SendCommand(0x83);
  SendData(0x00);
  SendData(0x00);
  SendData(0x00);
  SendData(0xA7);
  SendData(0x00);
  SendData(0x00);
  SendData(0x01);
  SendData(0x8F);
  SendData(0x01);
}

void Epd::setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  SendCommand(0x83);
  SendData(x / 256);
  SendData(x % 256);
  SendData((x + w - 1) / 256);
  SendData((x + w - 1) % 256);
  SendData(y / 256);
  SendData(y % 256);
  SendData((y + h - 1) / 256);
  SendData((y + h - 1) % 256);
  SendData(0x01);
}
/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void Epd::Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (WIDTH % 4 == 0)? (WIDTH / 4 ): (WIDTH / 4 + 1);
    Height = HEIGHT;

    SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            SendData((color<<6) | (color<<4) | (color<<2) | color);
            // SendData((color<<6) | (color<<4) | (0x00<<2) | 0x00);
        }
        // Serial.print("\n");
    }
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd::Display(UBYTE *Image)
{
    UWORD Width, Height;
    UWORD addr;
    Width = (WIDTH % 4 == 0)? (WIDTH / 4 ): (WIDTH / 4 + 1);
    Height = HEIGHT;

    SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            SendData(pgm_read_byte(&Image[i + j * Width]));
        }
        // Serial.print("\n");
    }
}

uint16_t Epd::_getXIndex(uint16_t x) {
  return (x % 4 == 0)? (x / 4 ): (x / 4 + 1);
}
void Epd::DisplayPart(UBYTE *Image, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    UWORD Width, Height;
    UWORD addr;
    Width = _getXIndex(WIDTH);
    Height = HEIGHT;
    uint16_t _x, _y, _w;
    _w = _getXIndex(x + w);

    SendCommand(0x10);
    for (UWORD j = 0; j < h; j++) {
        _y = y + j;
        for (UWORD i = x / 4; i < _w; i ++) {
            SendData(pgm_read_byte(&Image[i + _y * Width]));
        }
        // Serial.print("\n");
    }
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void Epd::Sleep(void)
{
    SendCommand(0x02); // POWER_OFF
    SendData(0X00);
    SendCommand(0x07); // DEEP_SLEEP
    SendData(0XA5);
}

/* END OF FILE */


