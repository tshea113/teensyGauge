#include "CanDataHandler.h"

#include "FlexCAN_T4.h"

#include <Arduino.h>

const int kCanBaud = 500000; // CAN baud rate

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;
CanDataHandler canDataHandler(kCanBaud, can);

void printData(bool& newData, const float& map, const float& rpm, const float& tps, const float& clt)
{
  if (newData)
  {
    Serial.print(map);
    Serial.print(" | "); // should be kPa
    Serial.print(rpm);
    Serial.print(" | "); // should be rpm
    Serial.print(tps);
    Serial.print(" | "); // should be %
    Serial.println(clt); // should be F

    newData = false;
  }
}

void printHeader()
{
  Serial.println("MAP | RPM | TPS | CLT");
}

void setup()
{
  Serial.begin(115200);

  printHeader();

  // Test the onboard LED in startup
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  can.events();

  // TODO: Remove this debug code once the display is working
  float map = canDataHandler.getGaugeData(kMAP);
  float rpm = canDataHandler.getGaugeData(kRPM);
  float tps = canDataHandler.getGaugeData(kTPS);
  float clt = canDataHandler.getGaugeData(kCLT);
  bool newData = canDataHandler.getNewData();

  printData(newData, map, rpm, tps, clt);

  if (!newData)
  {
    canDataHandler.setNewData(newData);
    digitalWrite(LED_BUILTIN, LOW);
  }
  // end TODO
}