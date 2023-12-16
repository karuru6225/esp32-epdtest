
/*
              __|    |__
GPIO2  A0 D0 |          | 5V
GPIO3  A1 D1 |          | GND
GPIO4  A2 D2 |          | 3V3
GPIO5  A3 D3 |          | D10 MOSI GPIO10
GPIO6 SDA D4 |          | D9  MISO GPIO9 BOOT_Btn
GPIO7 SCL D5 |          | D8   SCK GPIO8 
GPIO21 TX D6 |          | D7    RX GPIO20
*/

#include <SPI.h>
#include "epd3in0g.h"
#include "imagedata.h"

Epd epd;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);
  Serial.print("e-Paper init ");
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  // Serial.print("black \r\n");
  // epd.Clear(black);
  // delay(12100);

// #define  black   0x0
// #define  white   0x1
// #define  yellow  0x2
// #define  red     0x3

  // epd.Init();
  Serial.print("yellow \r\n");

  // epd.Clear(yellow);
  // epd.Refresh();
  // epd.PowerOff();
  // delay(15000);
  
  // epd.Init();
  // Serial.print("red \r\n");
  // epd.Clear(red);
  // epd.Refresh();
  // epd.PowerOff();
  // delay(15000);

  // Serial.print("yellow \r\n");
  // epd.Clear(yellow);
  // delay(2000);

  // Serial.print("red \r\n");
  // epd.Clear(red);
  // delay(2000);

  // Serial.print("Image \r\n");

  // epd.Init();
  epd.setPartialRamArea(50, 50, 100, 200);
  epd.DisplayPart(IMAGE_DATA, 50, 50, 100, 200);
  epd.RefreshFloating();
  // epd.Display(IMAGE_DATA);
  // epd.Refresh();
  epd.PowerOff();
  delay(15000);

  epd.setFullScreen();
  epd.Display(IMAGE_DATA);
  epd.Refresh();
  epd.PowerOff();
  delay(15000);

  // // epd.Init();
  // Serial.print("Small Image \r\n");
  // epd.Display_part(IMAGE_DATA, 0 ,0, 168, 400);
  // delay(2000);

  // epd.Init();
  // Serial.print("Clear...\r\n");
  // epd.Clear(white);
  // delay(2000);
  
  Serial.print("Clear...\r\n");

  // epd.Init();
  epd.Clear(white);
  epd.Refresh();
  epd.PowerOff();
  delay(15000);

  Serial.print("Goto Sleep...\r\n");
  epd.Sleep();

}

void loop() {
  // put your main code here, to run repeatedly:

}
