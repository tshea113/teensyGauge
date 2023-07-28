#pragma once

#include "CanDataHandler.h"
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
  kDashboard = 0,
  kQuadGauge = 1,
  kDualGauge = 2,
  kSingleGauge = 3,
  kGaugeMin = kDashboard,
  kGaugeMax = kSingleGauge,
};

class DisplayHandler
{
public:
  DisplayHandler() = delete;
  DisplayHandler(int TFT_RST, int TFT_DC, int TFT_CS, int screenHeight, int screenWidth);

  void displayStartupScreen();
  void display();
  void clearScreen();

  void moveGaugeCursor(int gaugeIndex);
  void clearGaugeCursor();

  void setCurrentData(std::vector<std::pair<GaugeData, String>> newData);
  std::vector<std::pair<GaugeData, String>> getCurrentData();

  void setCurrentView(GaugeView newGauge);
  GaugeView getCurrentView();

private:
  const int _screenHeight;
  const int _screenWidth;

  Adafruit_GC9A01A _tft;

  GaugeView _currentGaugeView;
  bool _gaugeViewUpdated;
  std::vector<std::pair<GaugeData, String>> _currentData;
  std::vector<std::pair<GaugeData, String>> _oldData;
  bool _dataUpdated;
  int _gaugeCursorIndex;

  void _displayDashboard();
  void _refreshDashboard();

  void _displayQuad();
  void _refreshQuad();

  void _displayDual();
  void _refreshDual();

  void _displaySingle();
  void _refreshSingle();

  void _highlightQuadGauge(uint16_t textColor, uint16_t backgroundColor);

  int _getCenterOffset(FontSize fontSize, int length) const;
};