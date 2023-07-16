#include "DisplayHandler.h"

DisplayHandler::DisplayHandler(int _tft_RST, int _tft_DC, int _tft_CS, int _screenHeight, int _screenWidth)
    : _screenHeight(_screenHeight),
    _screenWidth(_screenWidth),
    _tft(_tft_CS, _tft_DC, _tft_RST) // Hardware SPI on Feather or other boards
{
}

void DisplayHandler::displayStartupScreen()
{
  _tft.begin();
  clearScreen();
  _tft.drawBitmap(20, 98, miata_logo, 200, 44, RED);
}

// TODO: Delete this when inputs are working. This is useful while in dev.
void DisplayHandler::displayData(int rpm, int tps, int mat, int map, int clt)
{
  _tft.useFrameBuffer(1);
  clearScreen();
  _tft.setTextSize(2);

  _tft.setCursor(20, 50);
  _tft.print("RPM: ");
  _tft.println(rpm);

  _tft.setCursor(20, 66);
  _tft.print("TPS: ");
  _tft.println(tps);

  _tft.setCursor(20, 82);
  _tft.print("MAT: ");
  _tft.println(mat);

  _tft.setCursor(20, 98);
  _tft.print("MAP: ");
  _tft.println(map);

  _tft.setCursor(20, 114);
  _tft.print("CLT: ");
  _tft.println(clt);

  _tft.updateScreen();
}

// Displays 4 gauges divided into 4 quadrants.
// Labels 10 characters and data 5 characters max.
void DisplayHandler::displayQuad(const std::pair<String, String>* data)
{
  _tft.drawFastHLine(0, _screenHeight / 2, _screenWidth, WHITE);
  _tft.drawFastVLine(_screenWidth / 2, 0, _screenHeight, WHITE);

  _tft.setTextSize(kFontSizeMedium);
  _tft.setTextColor(WHITE);

  // Print labels
  _tft.setCursor((_screenWidth / 4) - getCenterOffset(kFontSizeMedium, data[0].first), (_screenHeight / 2) - 20);
  _tft.println(data[0].first);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - getCenterOffset(kFontSizeMedium, data[1].first),
                 (_screenHeight / 2) - 20);
  _tft.println(data[1].first);

  _tft.setCursor((_screenWidth / 4) - getCenterOffset(kFontSizeMedium, data[2].first), (_screenHeight / 2) + 6);
  _tft.println(data[2].first);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - getCenterOffset(kFontSizeMedium, data[3].first),
                 (_screenHeight / 2) + 6);
  _tft.println(data[3].first);

  // Print data
  _tft.setTextSize(kFontSizeLarge);
  _tft.setTextColor(WHITE);

  _tft.setCursor((_screenWidth / 4) - getCenterOffset(kFontSizeLarge, data[0].second), (_screenHeight / 2) - 50);
  _tft.println(data[0].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - getCenterOffset(kFontSizeLarge, data[1].second),
                 (_screenHeight / 2) - 50);
  _tft.println(data[1].second);

  _tft.setCursor((_screenWidth / 4) - getCenterOffset(kFontSizeLarge, data[2].second), (_screenHeight / 2) + 30);
  _tft.println(data[2].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - getCenterOffset(kFontSizeLarge, data[3].second),
                 (_screenHeight / 2) + 30);
  _tft.println(data[3].second);
}

void DisplayHandler::clearScreen()
{
  _tft.fillScreen(BLACK);
}

int DisplayHandler::getCenterOffset(FontSize fontSize, const String& text) const
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