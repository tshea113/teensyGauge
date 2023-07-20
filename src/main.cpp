#include "CanDataHandler.h"
#include "DisplayHandler.h"

#include <Arduino.h>
#include <utility>
#include <vector>

// HW specific definitions
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

#define SCREEN_WIDTH 240
#define SCREEEN_HEIGHT 240

// CAN baud rate
const int kCanBaud = 500000;

CanDataHandler canDataHandler;
DisplayHandler displayHandler(TFT_RST, TFT_DC, TFT_CS, SCREEEN_HEIGHT, SCREEN_WIDTH);

void setup()
{
  displayHandler.displayStartupScreen();

  Serial.begin(115200);

  canDataHandler.initCan(kCanBaud);

  if (Serial)
    printHeader();

  displayHandler.clearScreen();
}

void loop()
{
  can.events();

  std::vector<std::pair<String, String>> data = canDataHandler.getGaugeData({kRPM, kTPS, kMAP, kCLT});

  displayHandler.setCurrentData(data);
  displayHandler.display();
}