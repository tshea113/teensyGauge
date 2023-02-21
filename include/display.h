#pragma once

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"

#include "memory.h"

#define TFT_RST 8
#define TFT_DC  9
#define TFT_CS  10

// Hardware SPI on Feather or other boards
Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_RST);

void showStartupScreen()
{
  tft.begin();
  tft.fillScreen(GC9A01A_BLACK);
  tft.drawBitmap(20, 98, miata_logo, 200, 44, GC9A01A_BLUE);
}