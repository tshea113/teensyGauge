#include "FlexCAN_T4.h"
#include "MegaCAN.h"
#include <Arduino.h>

#include "canBus.h"
#include "display.h"

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

void setup()
{
  showStartupScreen();

  initializeCAN();

  delay(1000);
}

void loop()
{
  can.events();

  displayData(getGaugeData(GaugeData::kRPM), getGaugeData(GaugeData::kTPS), getGaugeData(GaugeData::kMAT),
              getGaugeData(GaugeData::kMAP), getGaugeData(GaugeData::kCLT));
}