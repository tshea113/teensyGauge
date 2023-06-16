#pragma once

#include "ProgramMemory.h"

#include <Adafruit_GFX.h>
#include <GC9A01A_t3n.h>
#include <SPI.h>

#include <utility>

const int kScreenWidth = 240;
const int kScreenHeight = 240;

enum FontSize : int
{
  kFontSizeSmall = 1,  // Font size 6x8
  kFontSizeMedium = 2, // Font size 12x16
  kFontSizeLarge = 3,  // Font size 18x24
};

class DisplayHandler
{
public:
  DisplayHandler(int TFT_RST, int TFT_DC, int TFT_CS);

  void displayStartupScreen();
  void displayData(int rpm, int tps, int mat, int map, int clt);
  void displayQuad(const std::pair<String, String>* data);
  void clearScreen();

private:
  GC9A01A_t3n tft;

  int getCenterOffset(FontSize fontSize, const String& text) const;
};