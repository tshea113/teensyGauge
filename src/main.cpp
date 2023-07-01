#include "CanDataHandler.h"

#include "FlexCAN_T4.h"

#include <Arduino.h>

const int kCanBaud = 500000; // CAN baud rate

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;
CanDataHandler canDataHandler(kCanBaud, can);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  can.events();

  // printData();
}