#pragma once

#include "Adafruit_GFX.h"
#include "GC9A01A_t3n.h"
#include "SPI.h"

#include "ProgramMemory.h"

class DisplayHandler
{
public:
  DisplayHandler(int TFT_RST, int TFT_DC, int TFT_CS);

  void showStartupScreen();
  void displayData(int rpm, int tps, int mat, int map, int clt);

private:
  GC9A01A_t3n tft;
};