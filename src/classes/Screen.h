

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <tools/Utils.h>
#include <static/bmp.h>
#include <ESPDateTime.h>

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23

#define TFT_BL              4   // Display backlight control pin
#define ADC_EN              14  //ADC_EN is the ADC detection enable port
#define ADC_PIN             34
#define BUTTON_1            35
#define BUTTON_2            0

#define SCALE               2


class Screen {       // The class
  public:             // Access specifier
    Screen();

    void initialize();

    void DemoIntro();
    void WifiScan_P1();
    void WifiScan_P2(int16_t results);
    void WifiConnect_P1();
    void WifiConnect_P2();
    void ClockBar(String timestamp);
    void TextTester();
    void Home();
    void ProgressBarClock();
    void Clock();
    void Clear();

  private:
    char buff[2048];
    char clock_lastHour[3];
    char clock_lastMinute[3];
    char clock_lastSecond[3];

};