
#include <classes/Screen.h>

// I believe this will create it outside the scope of the class instance itself, sort of a singleton
TFT_eSPI tft = TFT_eSPI(135, 240);

// const int CHOSEN_FONT = &FreeSansBold9pt7b;
const int FONT_DEFAULT_WIDTH = 6;
const int FONT_DEFAULT_HEIGHT = 8;
const int FONT_PT9_WIDTH = 11;
const int FONT_PT9_HEIGHT = 30;
const int CHAR_WIDTH = SCALE * FONT_DEFAULT_WIDTH;

Screen::Screen() {
  sprintf(clock_lastHour, "%s", "00");
  sprintf(clock_lastMinute, "%s", "11");
  sprintf(clock_lastSecond, "%s", "22");
}

void Screen::initialize() {
  tft.init();
  tft.setRotation(-1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);                 // (this is a guess) sets the "origin" of text to the middle of the char
  tft.setTextSize(1);

  if (TFT_BL > 0) {                           // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
      pinMode(TFT_BL, OUTPUT);                // Set backlight pin to output mode
      digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
  }

  tft.drawString("LeftButton:", tft.width() / 2, tft.height() / 2 - 16);
  tft.drawString("[Get Time]", tft.width() / 2, tft.height() / 2 );
  tft.drawString("RightButton:", tft.width() / 2, tft.height() / 2 + 16);
  tft.drawString("[WiFi Connect]", tft.width() / 2, tft.height() / 2 + 32 );
  tft.setTextDatum(TL_DATUM);                 // (this is a guess) sets the "origin" of text to the top left of the char
}

// Deprecated
void Screen::DemoIntro() {
  // the bitmap
  tft.setSwapBytes(true);                     // if colors in images are wrong, setting this will "correct for the endianess of the byte order"
  tft.pushImage(0, 0,  240, 135, ttgo);
  espDelay(5000);
  // now the random colors
  tft.setRotation(0);
  tft.fillScreen(TFT_RED);
  espDelay(1000);
  tft.fillScreen(TFT_BLUE);
  espDelay(1000);
  tft.fillScreen(TFT_GREEN);
  espDelay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
}

void Screen::WifiScan_P1() {
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1);
  tft.drawString("Scan Network", tft.width() / 2, tft.height() / 2);
}

void Screen::WifiScan_P2(int16_t results) {
  tft.fillScreen(TFT_BLACK);
  if (results == 0) {
    tft.drawString("no networks found", tft.width() / 2, tft.height() / 2);
  } else {
    tft.setTextDatum(TL_DATUM);
    tft.setCursor(0, 0);
    for (int i = 0; i < results; ++i) {
      sprintf(buff,
              "[%d]:%s(%d)",
              i + 1,
              WiFi.SSID(i).c_str(),
              WiFi.RSSI(i));
      tft.println(buff);
    }
  }
}

void Screen::WifiConnect_P1() {
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Connecting...", 0, tft.height() / 2);
}

void Screen::WifiConnect_P2() {
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Connected to: guest wifi", 0, tft.height() / 2);
}

void Screen::ClockBar(String timestamp) {
  Serial.println(timestamp);
  char b[timestamp.length()];
  timestamp.toCharArray(b, timestamp.length() + 1);

  tft.fillRect(0, 0, tft.width(), 10, 0xF800);
  tft.drawString(b, 0, 0);
  //  tft.setCursor(0, 0);
  //  tft.fillScreen(TFT_BLACK);
  //  tft.drawString(b, 0, tft.height() / 2);
  //  tft.println(b);
}

void Screen::TextTester() {
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  tft.drawString("1234abcgABCG", 0, tft.height() / 2);

  tft.setCursor(0, 0);
  tft.println("1234abcgABCG");
  tft.println("1234abcgABCG");
  tft.println("1234abcgABCG");

  int height = (int32_t)(tft.height());
  int width = (int32_t)(tft.width());
  tft.drawLine( 12 * FONT_PT9_WIDTH, 0, 12 * FONT_PT9_WIDTH, height, 0xFFFFFF );
  tft.drawLine( 0, 3 * FONT_PT9_HEIGHT, width, 3 * FONT_PT9_HEIGHT, 0xFFFFFF );
}

void Screen::Home() {

}

/** @todo - let's redraw smarter, we only really need to draw 2 lines every cycle once the initial painting is in place */
const bool DRAW_MARKERS = true;
const bool DRAW_LABELS = false;

void Screen::ProgressBarClock() {
  const uint8_t ogDatum = tft.getTextDatum();
  const uint8_t ogTextSize = tft.textsize;
  tft.fillRect(0, tft.height() / 2, tft.width(), tft.height(), 0x000000);
  DateTimeParts parts = DateTime.getParts();
  int hours = parts.getHours();
  int minutes = parts.getMinutes();
  float percent = (hours + (minutes / 60.0)) / 24.0;

  int rx = tft.width() / 10; // start 10% away from the left of the screen
  int ry = tft.height() / 2; // start halfway down the screen
  int rw = tft.width() - rx * 2; // end 10% away from right of screen
  int rh = (tft.height() / 2) - tft.height() / 10; // end 10% away from bottom of screen

  int hy = ry - tft.height() / 20 * 3; // start the marker line 15% higher than the top of the rect
  int hw = tft.height() / 20 * 2; // end the line 5% above the top of the rect
  int hx8 = rx + rw / 24 * 8;
  int hx10 = rx + rw / 24 * 10;
  int hx12 = rx + rw / 24 * 12;
  int hx14 = rx + rw / 24 * 14;
  int hx16 = rx + rw / 24 * 16;

  // Draw bounding box and progress bar
  int nowX = (int32_t)(percent * rw);
  tft.fillRect(rx, ry, nowX, rh, 0xFFFFFF);
  tft.drawRect(rx, ry, rw, rh, TFT_SKYBLUE);

  if (DRAW_MARKERS) {
    tft.drawFastVLine(hx8, hy, hw, 0xFFFFFF);
    tft.drawFastVLine(hx10, hy + hw / 2, hw / 2, 0xFFFFFF);
    tft.drawFastVLine(hx12, hy, hw, 0xFFFFFF);
    tft.drawFastVLine(hx14, hy + hw / 2, hw / 2, 0xFFFFFF);
    tft.drawFastVLine(hx16, hy, hw, 0xFFFFFF);
  }
  
  if (DRAW_LABELS) {
    const float LABEL_SCALE_FACTOR = 0.5;
    tft.setTextSize(SCALE * LABEL_SCALE_FACTOR);
    int ty = ry - tft.height() / 20 * 5; // draw the char 25% above the rect
    // tft.setTextDatum(MC_DATUM);
    tft.drawString("8", hx8 - CHAR_WIDTH/2 * LABEL_SCALE_FACTOR, ty);
    tft.drawString("12", hx12 - CHAR_WIDTH * LABEL_SCALE_FACTOR, ty);
    tft.drawString("4", hx16 - CHAR_WIDTH/2 * LABEL_SCALE_FACTOR, ty);
  }

  tft.setTextDatum(ogDatum);  // cleanup
  tft.setTextSize(ogTextSize);  // cleanup
}

void Screen::Clock() {
  const int ogTextSize = tft.textsize;
  const int ogTextColor = tft.textcolor;

  int len = 8; // "00:00:00"
  int x = tft.width() / 2 - (len * CHAR_WIDTH) / 2;
  int y = tft.height() / 10;

  tft.setTextSize(SCALE);

  DateTimeParts parts = DateTime.getParts();
  int hours = parts.getHours();
  int minutes = parts.getMinutes();
  int seconds = parts.getSeconds();
  const bool redrawHours = atoi(clock_lastHour) != hours;
  const bool redrawMinutes = atoi(clock_lastMinute) != minutes;
  const bool redrawSeconds = atoi(clock_lastSecond) != seconds;

  // blackout w/ smart redraw
  tft.setTextColor(TFT_BLACK);
  if (redrawHours) tft.drawString(clock_lastHour, x, y);
  // tft.drawString(":", x + CHAR_WIDTH * 2, y);
  if (redrawMinutes) tft.drawString(clock_lastMinute, x + CHAR_WIDTH * 3, y);
  // tft.drawString(":", x + CHAR_WIDTH * 5, y);
  if (redrawSeconds) tft.drawString(clock_lastSecond, x + CHAR_WIDTH * 6, y);
  
  // generate the new time segments (btw this is so disgusting)
  if (hours > 9) itoa(hours, clock_lastHour, 10);
  else {
    itoa(hours, clock_lastHour, 10);
    clock_lastHour[1] = clock_lastHour[0];
    clock_lastHour[0] = '0';
  }
  if (minutes > 9) itoa(minutes, clock_lastMinute, 10);
  else {
    itoa(minutes, clock_lastMinute, 10);
    clock_lastMinute[1] = clock_lastMinute[0];
    clock_lastMinute[0] = '0';
  }
  if (seconds > 9) itoa(seconds, clock_lastSecond, 10);
  else {
    itoa(seconds, clock_lastSecond, 10);
    clock_lastSecond[1] = clock_lastSecond[0];
    clock_lastSecond[0] = '0';
  }

  // now draw in the new time w/ smart redraw
  tft.setTextColor(TFT_WHITE);
  if (redrawHours)
    tft.drawString(clock_lastHour, x, y);
  if (redrawMinutes)
    tft.drawString(clock_lastMinute, x + CHAR_WIDTH * 3, y);
  if (redrawSeconds)
    tft.drawString(clock_lastSecond, x + CHAR_WIDTH * 6, y);

  // we don't really need to draw this every time since they're static, but leaving for now
  tft.drawString(":", x + CHAR_WIDTH * 2, y);
  tft.drawString(":", x + CHAR_WIDTH * 5, y);

  tft.setTextSize(ogTextSize);
  tft.setTextColor(ogTextColor);
}

void Screen::Clear() {
  tft.fillScreen(TFT_BLACK);
  // tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_WHITE);
}
