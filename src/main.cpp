#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <Button2.h>
#include <ESPDateTime.h>
#include <ESP.h>
#include <tools/Utils.h>
#include <classes/Screen.h>
#include <classes/API.h>

Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

Screen screen;
API api;

int vref = 1100;

void wifi_scan();
void fetchTime();
void processResponseBuffer();
void drawTime();

int lastRedraw_1m = millis();
int lastRedraw_1s = millis();

void button_init()
{
    btn1.setPressedHandler([](Button2 & b) {
        Serial.println("btn1 press");
        fetchTime();
    });

    btn2.setPressedHandler([](Button2 & b) {
        Serial.println("btn2 press");
        screen.TextTester();
    });
}

void button_loop()
{
    btn1.loop();
    btn2.loop();
}

void wifi_scan()
{
    screen.WifiScan_P1();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int16_t n = WiFi.scanNetworks();
    screen.WifiScan_P2(n);
}

void fetchTime() {
  // setup this after wifi connected
  // you can use custom timeZone,server and timeout
  // DateTime.setTimeZone(-4);
  // DateTime.setServer("0.us.pool.ntp.org");
  // DateTime.begin(15 * 1000);
  DateTime.setTimeZone(-7);
  // this method config ntp and wait for time sync
  // default timeout is 10 seconds
  DateTime.begin(/* timeout param */);
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
  Serial.println(DateTime.toString());
}

void loop()
{
  button_loop();
  api.Sweep();

  if (millis() - lastRedraw_1m > 60 * 1000) {
    screen.ProgressBarClock();
    lastRedraw_1m = millis();
    Serial.println("redrawing 1m");
  }

  if (millis() - lastRedraw_1s > 1000) {
    screen.Clock();
    lastRedraw_1s = millis();
    Serial.println("redrawing 1s");
  }

}


void setup()
{
    Serial.begin(115200);

    Serial.println("Start");
    Serial.println(ESP.getCpuFreqMHz());

    api.initialize();
    screen.initialize();
    button_init();
    fetchTime();

    screen.Clear();
    screen.ProgressBarClock();
    screen.Clock();
}
