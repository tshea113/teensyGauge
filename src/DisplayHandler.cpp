#include "DisplayHandler.h"

DisplayHandler::DisplayHandler(int TFT_RST, int TFT_DC, int TFT_CS)
    : tft(TFT_CS, TFT_DC, TFT_RST) // Hardware SPI on Feather or other boards
{
}

void DisplayHandler::showStartupScreen()
{
  tft.begin();
  tft.fillScreen(BLACK);
  tft.drawBitmap(20, 98, miata_logo, 200, 44, RED);
}

void DisplayHandler::displayData(int rpm, int tps, int mat, int map, int clt)
{
  tft.useFrameBuffer(1);
  tft.fillScreen(BLACK);
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