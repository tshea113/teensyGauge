#pragma once

#include "ProgramMemory.h"

#include <Adafruit_GC9A01A.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <utility>
#include <vector>

const int kFontWidthSmall = 6;
const int kFontHeightSmall = 8;
const int kFontWidthMedium = 12;
const int kFontHeightMedium = 16;
const int kFontWidthLarge = 18;
const int kFontHeightLarge = 24;

const int kMaxDigits = 5;

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

  void displayStartupScreen();
  void display();
  void clearScreen();

  void setCurrentData(std::vector<std::pair<String, String>> newData);
  std::vector<std::pair<String, String>> getCurrentData();

  void setCurrentGauge(GaugeView newGauge);
  GaugeView getCurrentGauge();

private:
  const int _screenHeight;
  const int _screenWidth;

  Adafruit_GC9A01A _tft;

  GaugeView _currentGauge;
  bool _gaugeUpdated;
  std::vector<std::pair<String, String>> _currentData;
  std::vector<std::pair<String, String>> _oldData;
  bool _dataUpdated;

  void _displayQuad();
  void _refreshQuad();

  int _getCenterOffset(const FontSize& fontSize, const int& length) const;
};