#include "DisplayHandler.h"

DisplayHandler::DisplayHandler(int _tft_RST, int _tft_DC, int _tft_CS, int _screenHeight, int _screenWidth)
    : _screenHeight(_screenHeight),
    _screenWidth(_screenWidth),
    _tft(_tft_CS, _tft_DC, _tft_RST) // Hardware SPI on Feather or other boards
{
  _currentGauge = kQuadGauge;
}

void DisplayHandler::display()
{
  switch (_currentGauge)
  {
  case kQuadGauge:
    _displayQuad();
    break;

  default:
    Serial.println("Invalid Gauge!");
  }
}

void DisplayHandler::displayStartupScreen()
{
  _tft.begin();
  clearScreen();
  _tft.drawBitmap(20, 98, miata_logo, 200, 44, RED);
}

void DisplayHandler::clearScreen()
{
  _tft.fillScreen(BLACK);
}

void DisplayHandler::setCurrentData(std::vector<std::pair<String, String>> newData)
{
  _currentData = newData;
}

// Displays 4 gauges divided into 4 quadrants.
// Labels 10 characters and data 5 characters max.
void DisplayHandler::_displayQuad()
{
  _tft.drawFastHLine(0, _screenHeight / 2, _screenWidth, WHITE);
  _tft.drawFastVLine(_screenWidth / 2, 0, _screenHeight, WHITE);

  _tft.setTextSize(kFontSizeMedium);
  _tft.setTextColor(WHITE);

  // Print labels
  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, _currentData[0].first), (_screenHeight / 2) - 20);
  _tft.println(_currentData[0].first);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, _currentData[1].first),
                 (_screenHeight / 2) - 20);
  _tft.println(_currentData[1].first);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, _currentData[2].first), (_screenHeight / 2) + 6);
  _tft.println(_currentData[2].first);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, _currentData[3].first),
                 (_screenHeight / 2) + 6);
  _tft.println(_currentData[3].first);

  // Print data
  _tft.setTextSize(kFontSizeLarge);
  _tft.setTextColor(WHITE);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[0].second), (_screenHeight / 2) - 50);
  _tft.println(_currentData[0].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[1].second),
                 (_screenHeight / 2) - 50);
  _tft.println(_currentData[1].second);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[2].second), (_screenHeight / 2) + 30);
  _tft.println(_currentData[2].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[3].second),
                 (_screenHeight / 2) + 30);
  _tft.println(_currentData[3].second);
}

int DisplayHandler::_getCenterOffset(FontSize fontSize, const String& text) const
{
  switch (fontSize)
  {
  case kFontSizeSmall:
    return (text.length() * 6) / 2;
  case kFontSizeMedium:
    return (text.length() * 12) / 2;
  case kFontSizeLarge:
    return (text.length() * 18) / 2;
  default:
    return -1;
  }
}