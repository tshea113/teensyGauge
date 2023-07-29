#pragma once

#include "CanDataHandler.h"
#include "ProgramMemory.h"

#include <Adafruit_GFX.h>
#include <GC9A01A_t3n.h>
#include <SPI.h>
#include <utility>
#include <vector>

const int kMaxDigits = 5;

enum FontSize : int
{
  kFontSizeSmall = 1,  // Font size 6x8
  kFontSizeMedium = 2, // Font size 12x16
  kFontSizeLarge = 3,  // Font size 18x24
  kFontSizeXL = 4,     // Font size 24x36
  kFontSizeXXL = 5,    // Font size 30x44
  kFontSizeXXXL = 6,   // Font size 36x52
};

enum GaugeView : int
{
  kDashboard = 0,
  kQuadGauge = 1,
  kDualGauge = 2,
  kSingleGauge = 3,
  kGaugeMin = kQuadGauge,
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

  GC9A01A_t3n _tft;

  GaugeView _currentGaugeView;
  bool _gaugeViewUpdated;
  std::vector<std::pair<GaugeData, String>> _currentData;
  std::vector<std::pair<GaugeData, String>> _oldData;
  bool _dataUpdated;
  int _gaugeCursorIndex;

  void _refreshData(int dataIndex, FontSize fontSize, int cursorX, int cursorY);
  int _getFontWidth(FontSize fontSize) const;
  int _getFontHeight(FontSize fontSize) const;
  int _getCenterOffset(FontSize fontSize, int length) const;

  void _displayDashboard();
  void _refreshDashboard();

  void _displayQuad();
  void _refreshQuad();

  void _displayDual();
  void _refreshDual();

  void _displaySingle();
  void _refreshSingle();

  void _highlightQuadGauge(uint16_t textColor, uint16_t backgroundColor);
  void _highlightDualGauge(uint16_t textColor, uint16_t backgroundColor);
  void _highlightSingleGauge(uint16_t textColor, uint16_t backgroundColor);
};