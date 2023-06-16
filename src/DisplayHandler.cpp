#include "DisplayHandler.h"

DisplayHandler::DisplayHandler(int TFT_RST, int TFT_DC, int TFT_CS)
    : tft(TFT_CS, TFT_DC, TFT_RST) // Hardware SPI on Feather or other boards
{
}

void DisplayHandler::displayStartupScreen()
{
  tft.begin();
  clearScreen();
  tft.drawBitmap(20, 98, miata_logo, 200, 44, RED);
}

// TODO: Delete this when inputs are working. This is useful while in dev.
void DisplayHandler::displayData(int rpm, int tps, int mat, int map, int clt)
{
  tft.useFrameBuffer(1);
  clearScreen();
  tft.setTextSize(2);

  tft.setCursor(20, 50);
  tft.print("RPM: ");
  tft.println(rpm);

  tft.setCursor(20, 66);
  tft.print("TPS: ");
  tft.println(tps);

  tft.setCursor(20, 82);
  tft.print("MAT: ");
  tft.println(mat);

  tft.setCursor(20, 98);
  tft.print("MAP: ");
  tft.println(map);

  tft.setCursor(20, 114);
  tft.print("CLT: ");
  tft.println(clt);

  tft.updateScreen();
}

// Displays 4 gauges divided into 4 quadrants.
// Labels 10 characters and data 5 characters max.
void DisplayHandler::displayQuad(const std::pair<String, String>* data)
{
  tft.drawFastHLine(0, kScreenHeight / 2, kScreenWidth, WHITE);
  tft.drawFastVLine(kScreenWidth / 2, 0, kScreenHeight, WHITE);

  tft.setTextSize(kFontSizeMedium);
  tft.setTextColor(WHITE);

  // Print labels
  tft.setCursor((kScreenWidth / 4) - getCenterOffset(kFontSizeMedium, data[0].first), (kScreenHeight / 2) - 20);
  tft.println(data[0].first);

  tft.setCursor(kScreenWidth - (kScreenWidth / 4) - getCenterOffset(kFontSizeMedium, data[1].first),
                (kScreenHeight / 2) - 20);
  tft.println(data[1].first);

  tft.setCursor((kScreenWidth / 4) - getCenterOffset(kFontSizeMedium, data[2].first), (kScreenHeight / 2) + 6);
  tft.println(data[2].first);

  tft.setCursor(kScreenWidth - (kScreenWidth / 4) - getCenterOffset(kFontSizeMedium, data[3].first),
                (kScreenHeight / 2) + 6);
  tft.println(data[3].first);

  // Print data
  tft.setTextSize(kFontSizeLarge);
  tft.setTextColor(WHITE);

  tft.setCursor((kScreenWidth / 4) - getCenterOffset(kFontSizeLarge, data[0].second), (kScreenHeight / 2) - 50);
  tft.println(data[0].second);

  tft.setCursor(kScreenWidth - (kScreenWidth / 4) - getCenterOffset(kFontSizeLarge, data[1].second),
                (kScreenHeight / 2) - 50);
  tft.println(data[1].second);

  tft.setCursor((kScreenWidth / 4) - getCenterOffset(kFontSizeLarge, data[2].second), (kScreenHeight / 2) + 30);
  tft.println(data[2].second);

  tft.setCursor(kScreenWidth - (kScreenWidth / 4) - getCenterOffset(kFontSizeLarge, data[3].second),
                (kScreenHeight / 2) + 30);
  tft.println(data[3].second);
}

void DisplayHandler::clearScreen()
{
  tft.fillScreen(BLACK);
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