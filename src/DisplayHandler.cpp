#include "DisplayHandler.h"

DisplayHandler::DisplayHandler(int _tft_RST, int _tft_DC, int _tft_CS, int _screenHeight, int _screenWidth)
    : _screenHeight(_screenHeight),
    _screenWidth(_screenWidth),
    _tft(_tft_CS, _tft_DC, _tft_RST) // Hardware SPI
{
  _currentGauge = kQuadGauge;

  // We need to draw the first gauge.
  _gaugeUpdated = true;
}

void DisplayHandler::displayStartupScreen()
{
  _tft.begin();
  clearScreen();
  _tft.drawBitmap(20, 98, miata_logo, 200, 44, RED);
}

void DisplayHandler::display()
{
  // If we select a new gauge, we need to redraw EVERYTHING.
  // Otherwise we are just refreshing the data.
  switch (_currentGauge)
  {
  case kQuadGauge:
    _gaugeUpdated ? _displayQuad() : _refreshQuad();
    break;
  default:
    Serial.println("Invalid Gauge!");
  }

  _gaugeUpdated = false;
}

void DisplayHandler::clearScreen()
{
  _tft.fillScreen(BLACK);
}

void DisplayHandler::setCurrentData(std::vector<std::pair<String, String>> newData)
{
  _currentData = newData;
  _dataUpdated = true;
}

std::vector<std::pair<String, String>> DisplayHandler::getCurrentData()
{
  return _currentData;
}

void DisplayHandler::setCurrentGauge(GaugeView newGauge)
{
  _currentGauge = newGauge;
  _gaugeUpdated = true;
}

GaugeView DisplayHandler::getCurrentGauge()
{
  return _currentGauge;
}

// Displays 4 gauges divided into 4 quadrants.
// Labels 10 characters and data 5 characters max.
void DisplayHandler::_displayQuad()
{
  clearScreen();

  _tft.drawFastHLine(0, _screenHeight / 2, _screenWidth, WHITE);
  _tft.drawFastVLine(_screenWidth / 2, 0, _screenHeight, WHITE);

  _tft.setTextSize(kFontSizeMedium);
  _tft.setTextColor(WHITE);

  // Print labels
  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, _currentData[0].first.length()), (_screenHeight / 2) - 20);
  _tft.println(_currentData[0].first);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, _currentData[1].first.length()),
                 (_screenHeight / 2) - 20);
  _tft.println(_currentData[1].first);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, _currentData[2].first.length()), (_screenHeight / 2) + 6);
  _tft.println(_currentData[2].first);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, _currentData[3].first.length()),
                 (_screenHeight / 2) + 6);
  _tft.println(_currentData[3].first);

  // Print data
  _tft.setTextSize(kFontSizeLarge);
  _tft.setTextColor(WHITE);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[0].second.length()), (_screenHeight / 2) - 50);
  _tft.println(_currentData[0].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[1].second.length()),
                 (_screenHeight / 2) - 50);
  _tft.println(_currentData[1].second);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[2].second.length()), (_screenHeight / 2) + 30);
  _tft.println(_currentData[2].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[3].second.length()),
                 (_screenHeight / 2) + 30);
  _tft.println(_currentData[3].second);
}

void DisplayHandler::_refreshQuad()
{
  // Clear the old data
  _tft.fillRect((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, kMaxDigits), (_screenHeight / 2) - 50, kFontWidthLarge * kMaxDigits, kFontHeightLarge, BLACK);
  _tft.fillRect(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, kMaxDigits), (_screenHeight / 2) - 50, kFontWidthLarge * kMaxDigits, kFontHeightLarge, BLACK);
  _tft.fillRect((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, kMaxDigits), (_screenHeight / 2) + 30, kFontWidthLarge * kMaxDigits, kFontHeightLarge, BLACK);
  _tft.fillRect(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, kMaxDigits), (_screenHeight / 2) + 30, kFontWidthLarge * kMaxDigits, kFontHeightLarge, BLACK);

  // Print data
  _tft.setTextSize(kFontSizeLarge);
  _tft.setTextColor(WHITE);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[0].second.length()), (_screenHeight / 2) - 50);
  _tft.println(_currentData[0].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[1].second.length()), (_screenHeight / 2) - 50);
  _tft.println(_currentData[1].second);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[2].second.length()), (_screenHeight / 2) + 30);
  _tft.println(_currentData[2].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[3].second.length()), (_screenHeight / 2) + 30);
  _tft.println(_currentData[3].second);
}

int DisplayHandler::_getCenterOffset(const FontSize& fontSize, const int& length) const
{
  switch (fontSize)
  {
  case kFontSizeSmall:
    return (length * 6) / 2;
  case kFontSizeMedium:
    return (length * 12) / 2;
  case kFontSizeLarge:
    return (length * 18) / 2;
  default:
    return -1;
  }
}