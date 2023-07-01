#include "DisplayHandler.h"
#include "canBus.h"

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <MegaCAN.h>

#include <utility>

// HW specific definitions
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

#define SCREEN_WIDTH 240
#define SCREEEN_HEIGHT 240

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

DisplayHandler displayHandler(TFT_RST, TFT_DC, TFT_CS, SCREEEN_HEIGHT, SCREEN_WIDTH);

void setup()
{
  displayHandler.displayStartupScreen();

  initializeCAN();

  delay(1000);

  displayHandler.clearScreen();
}

void loop()
{
  can.events();

  std::pair<String, String> data[] = {
      {"RPM", bCastMsg.rpm}, {"CLT", bCastMsg.clt}, {"Boost Duty", bCastMsg.boostduty}, {"MAT", bCastMsg.mat}};

  displayHandler.displayQuad(data);
}