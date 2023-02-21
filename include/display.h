#pragma once

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "GC9A01A_t3n.h"

#include "memory.h"

#define TFT_RST 8
#define TFT_DC  9
#define TFT_CS  10

// Hardware SPI on Feather or other boards
GC9A01A_t3n tft(TFT_CS, TFT_DC, TFT_RST);

void showStartupScreen()
{
  tft.begin();
  tft.fillScreen(BLACK);
  tft.drawBitmap(20, 98, miata_logo, 200, 44, RED);
}

void displayData(int rpm, int tps, int mat, int map, int clt)
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