#include <Arduino.h>
#include "FlexCAN_T4.h"
#include "MegaCAN.h"

#include "canBus.h"

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

void setup()
{
  while (!Serial);
  Serial.begin(115200);

  initializeCAN();
  Serial.println("CAN setup!");

  printHeader();
}

void loop()
{
  can.events();

  printData();
}