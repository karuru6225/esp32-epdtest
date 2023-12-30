#include <SPI.h>
#include <WiFi.h>
#include <esp_sntp.h>
#include <TimeLib.h>

#include "epd3in0g.h"
#include "my_canvas8.h"
#include "secrets.h"

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

Epd epd;
UBYTE buf[EPD_WIDTH * EPD_HEIGHT / 4];

// テスト開始　2023/12/31 00:38
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);

  syncTime();

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
  
  MyCanvas8 canvas(EPD_WIDTH, EPD_HEIGHT);
  canvas.fillScreen(0x00);
  canvas.setRotation(1);
  // canvas.fillCircle(200, 50, 50, white);
  // canvas.fillCircle(200, 50, 30, yellow);
  // canvas.fillCircle(200, 50, 20, red);
  // canvas.writeFastHLine(0, 0, 110, 0x1);
  // canvas.writeFastHLine(100, 100, -11, 0x2);
  // canvas.writeFastHLine(0, 100, 110, 0x3);
  // canvas.writeFastVLine(0, 0, 100, 0x1);
  // canvas.writeFastVLine(100, 100, -21, 0x2);
  // canvas.fillRect(30, 120, 80, 20, yellow);
  int16_t cur_x = 5;
  int16_t cur_y = 5;
  // int16_t b_x, b_y;
  // uint16_t b_w, b_h;
  /*
  void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1,
                     int16_t *y1, uint16_t *w, uint16_t *h);
  */
  
  canvas.setCursor(cur_x, cur_y);
  canvas.setTextColor(white);
  canvas.setTextSize(5);

  char timeStr[10];
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  int d_year = tm->tm_year + 1900;
  int d_mon  = tm->tm_mon+1;
  int d_mday = tm->tm_mday;
  int d_hour = tm->tm_hour;
  int d_min  = tm->tm_min;
  int d_sec  = tm->tm_sec;

  sprintf(timeStr, "%04d/%02d/%02d\n", tm->tm_year + 1900, tm->tm_mon+1, tm->tm_mday);
  canvas.writeLines(timeStr);
  sprintf(timeStr, "%02d:%02d:%02d\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
  canvas.writeLines(timeStr);
  // canvas.getTextBounds("Test", cur_x, cur_y, &b_x, &b_y, &b_w, &b_h);
  
  // canvas.writeLines("Test\n0123\n");
  // canvas.setTextColor(red, white);
  // canvas.writeLines("Test\n0123\n");

  // cur_x = cur_x + 5 + b_w;
  // cur_y = 5;
  // canvas.setCursor(cur_x, cur_y);
  // canvas.setTextColor(yellow, black);
  // canvas.setTextSize(2);
  // canvas.writeLines("Test\n0123\n");
  // canvas.setTextColor(red, white);
  // canvas.writeLines("Test\n0123\n");
  
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
  
  // Serial.print("Clear...\r\n");

  // epd.Init();
  // epd.Clear(white);
  // epd.Refresh();

  // epd.PowerOff();
  // delay(15000);

  // Serial.print("Goto Sleep...\r\n");
  // delay(5000);
  // epd.Clear(white);
  // epd.Refresh();
  epd.Sleep();

  const unsigned long interval_sec = 60;
  esp_sleep_enable_timer_wakeup(interval_sec * 1000000);
  Serial.println("### DEEP SLEEP START");
  esp_deep_sleep_start();
}

const char* ntpServer = "ntp.nict.jp";
const long  gmtOffset_sec = 9 * 60 * 60;
const int   daylightOffset_sec = 0;

void syncTime() {
  WiFi.begin(WIFI_SSID, WIFI_PSWD);
  Serial.println("wifi connecting");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected IP: ");
  Serial.println(WiFi.localIP());
  delay(500);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // 内蔵時計の時刻がNTP時刻に合うまで待機
  while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) {
    switch(sntp_get_sync_status()) {
      case SNTP_SYNC_STATUS_RESET:
        Serial.println("SNTP_SYNC_STATUS_RESET");
        break;
      case SNTP_SYNC_STATUS_IN_PROGRESS:
        Serial.println("SNTP_SYNC_STATUS_IN_PROGRESS");
        break;
      default:
        Serial.println("SNTP_SYNC_UNKNOWN");
    }
    delay(1000);
  }
  WiFi.disconnect();
}

void loop() {
  // put your main code here, to run repeatedly:

}
