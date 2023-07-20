#pragma once

#include "ProgramMemory.h"

#include <Adafruit_GFX.h>
#include <GC9A01A_t3n.h>
#include <SPI.h>

#include <utility>
#include <vector>

enum FontSize : int
{
  kFontSizeSmall = 1,  // Font size 6x8
  kFontSizeMedium = 2, // Font size 12x16
  kFontSizeLarge = 3,  // Font size 18x24
};

enum GaugeView : int
{
  kQuadGauge = 1
};

class DisplayHandler
{
public:
  DisplayHandler() = delete;
  DisplayHandler(int TFT_RST, int TFT_DC, int TFT_CS, int screenHeight, int screenWidth);

  void display();
  void displayStartupScreen();
  void displayData(int rpm, int tps, int mat, int map, int clt);
  void clearScreen();

  void setCurrentData(std::vector<std::pair<String, String>> newData);
  std::vector<std::pair<String, String>> getCurrentData();

private:
  const int _screenHeight;
  const int _screenWidth;

  GC9A01A_t3n _tft;

  GaugeView _currentGauge;
  std::vector<std::pair<String, String>> _currentData;

  void _displayQuad();

  int _getCenterOffset(FontSize fontSize, const String& text) const;
};