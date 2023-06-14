#include "FlexCAN_T4.h"
#include "MegaCAN.h"
#include <Arduino.h>

#include "DisplayHandler.h"
#include "canBus.h"

#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

DisplayHandler displayHandler(TFT_RST, TFT_DC, TFT_CS);

void setup()
{
  displayHandler.showStartupScreen();

  initializeCAN();

  delay(1000);
}

void loop()
{
  can.events();

  displayHandler.displayData(getGaugeData(GaugeData::kRPM), getGaugeData(GaugeData::kTPS),
                             getGaugeData(GaugeData::kMAT), getGaugeData(GaugeData::kMAP),
                             getGaugeData(GaugeData::kCLT));
}