#include "CanDataHandler.h"
#include "DisplayHandler.h"

#include <Arduino.h>
#include <Metro.h>
#include <utility>
#include <vector>

// HW specific definitions
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

#define SCREEN_WIDTH 240
#define SCREEEN_HEIGHT 240

const int kCanBaud = 500000; // Baud rate of the CAN bus

CanDataHandler canDataHandler;
DisplayHandler displayHandler(TFT_RST, TFT_DC, TFT_CS, SCREEEN_HEIGHT, SCREEN_WIDTH);

Metro displayTimer = Metro(500);

// TODO: delete debug code
  std::vector<std::pair<String, String>> data = {{"RPM", "1000"}, {"TPS", "45"}, {"MAP", "59"}, {"CLT", "225"}};

uint8_t i = 0;

void setup()
{
  displayHandler.displayStartupScreen();

  canDataHandler.initCan(kCanBaud);

  delay(500);
  displayHandler.clearScreen();
}

void loop()
{
  can.events();

  // std::vector<std::pair<String, String>> data = canDataHandler.getGaugeData({kRPM, kTPS, kMAP, kCLT});

  // TODO: This is debug code
  if (displayTimer.check())
  {
    data[0].second = String(i++);
    data[1].second = String(i++);
    data[2].second = String(i++);
    data[3].second = String(i++);
    displayHandler.setCurrentData(data);
    displayHandler.display();

    displayTimer.reset();
  }
}