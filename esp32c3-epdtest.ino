
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
#include <WiFi.h>
#include <Adafruit_GFX.h>

#include "epd3in0g.h"
#include "secrets.h";

Epd epd;
UBYTE buf[EPD_WIDTH * EPD_HEIGHT / 4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);

  WiFi.begin(WIFI_SSID, WIFI_PSWD);
  Serial.println("connecting");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected IP: ");
  Serial.println(WiFi.localIP());


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
  Serial.print("black \r\n");


  // epd.Clear(black);
  // epd.Refresh();
  // epd.PowerOff();
  // delay(15000);
  
  GFXcanvas8 canvas(EPD_WIDTH, EPD_HEIGHT);
  canvas.fillScreen(0x00);
  canvas.setRotation(1);
  canvas.fillCircle(200, 50, 50, white);
  canvas.fillCircle(200, 50, 30, yellow);
  canvas.fillCircle(200, 50, 20, red);
  // canvas.writeFastHLine(0, 0, 110, 0x1);
  // canvas.writeFastHLine(100, 100, -11, 0x2);
  // canvas.writeFastHLine(0, 100, 110, 0x3);
  // canvas.writeFastVLine(0, 0, 100, 0x1);
  // canvas.writeFastVLine(100, 100, -21, 0x2);
  canvas.fillRect(30, 120, 80, 20, yellow);
  int16_t cur_x = 5;
  int16_t cur_y = 5;
  int16_t b_x, b_y;
  uint16_t b_w, b_h;
  const char *line1 = "Test";
  const char *line2 = "01234";
  /*
  void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1,
                     int16_t *y1, uint16_t *w, uint16_t *h);
  */
  
  
  canvas.setCursor(cur_x, cur_y);
  canvas.setTextColor(white);
  canvas.setTextSize(3);
  canvas.getTextBounds("Test", cur_x, cur_y, &b_x, &b_y, &b_w, &b_h);
  
  canvas.write('T');
  canvas.write('e');
  canvas.write('s');
  canvas.write('t');
  canvas.write('\n');
  cur_y = canvas.getCursorY();
  canvas.setCursor(cur_x, cur_y);
  canvas.write('0');
  canvas.write('1');
  canvas.write('2');
  canvas.write('3');
  canvas.write('\n');
  cur_y = canvas.getCursorY();
  canvas.setCursor(cur_x, cur_y);
  canvas.setTextColor(red, white);
  canvas.write('T');
  canvas.write('e');
  canvas.write('s');
  canvas.write('t');
  canvas.write('\n');
  cur_y = canvas.getCursorY();
  canvas.setCursor(cur_x, cur_y);
  canvas.write('0');
  canvas.write('1');
  canvas.write('2');
  canvas.write('3');
  canvas.write('\n');

  cur_x = cur_x + 5 + b_w;
  cur_y = 5;
  canvas.setCursor(cur_x, cur_y);
  canvas.setTextColor(yellow, black);
  canvas.setTextSize(2);
  canvas.write('T');
  canvas.write('e');
  canvas.write('s');
  canvas.write('t');
  canvas.write('\n');
  cur_y = canvas.getCursorY();
  canvas.setCursor(cur_x, cur_y);
  canvas.write('0');
  canvas.write('1');
  canvas.write('2');
  canvas.write('3');
  canvas.write('\n');
  cur_y = canvas.getCursorY();
  canvas.setCursor(cur_x, cur_y);
  canvas.setTextColor(red, white);
  canvas.write('T');
  canvas.write('e');
  canvas.write('s');
  canvas.write('t');
  canvas.write('\n');
  cur_y = canvas.getCursorY();
  canvas.setCursor(cur_x, cur_y);
  canvas.write('0');
  canvas.write('1');
  canvas.write('2');
  canvas.write('3');
  canvas.write('\n');
  
  canvas.setRotation(0);
  for(uint16_t y = 0; y < EPD_HEIGHT; y++) {
    for(uint16_t x = 0; x < EPD_WIDTH; x += 4) {
      buf[x / 4 + y * EPD_WIDTH / 4] =
      ((UBYTE)canvas.getPixel(x+0, y))<<6 |
      ((UBYTE)canvas.getPixel(x+1, y))<<4 |
      ((UBYTE)canvas.getPixel(x+2, y))<<2 |
      ((UBYTE)canvas.getPixel(x+3, y))<<0;
    }
  }
  epd.setFullScreen();
  epd.Display(buf);
  epd.Refresh();
  epd.PowerOff();
  delay(15000);

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
  // epd.setPartialRamArea(50, 50, 100, 200);
  // epd.DisplayPart(IMAGE_DATA, 50, 50, 100, 200);
  // epd.RefreshFloating();
  // // epd.Display(IMAGE_DATA);
  // // epd.Refresh();
  // epd.PowerOff();
  // delay(15000);

  // epd.setFullScreen();
  // epd.Display(IMAGE_DATA);
  // epd.Refresh();
  // epd.PowerOff();
  // delay(15000);

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
  delay(5000);
  epd.Clear(white);
  epd.Refresh();
  epd.Sleep();

}

void loop() {
  // put your main code here, to run repeatedly:

}
