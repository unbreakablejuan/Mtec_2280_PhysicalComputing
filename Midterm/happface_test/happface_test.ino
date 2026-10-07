// Shows Juan's 'happface' (240x240, RGB565 from image2cpp) on the Waveshare 1.28" round LCD.
// Wiring and library: Plant Logger Build Runbook, section "Face display".
// happface.h must sit in this same folder; it shows up as a second tab in the IDE.

#include <Arduino_GFX_Library.h>
#include "happyface.h"

#define TFT_SCLK 12   // CLK, orange
#define TFT_MOSI 11   // DIN, green
#define TFT_CS   14   // CS, yellow
#define TFT_DC   17   // DC, blue
#define TFT_RST  18   // RST, brown
#define TFT_BL   21   // BL, grey

Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS panel */);

void setup() {
  Serial.begin(115200);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  gfx->begin();

  uint32_t t = millis();
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)epd_bitmap_happface, 240, 240);
  Serial.printf("happface drawn in %lu ms\n", (unsigned long)(millis() - t));
}

void loop() {}
