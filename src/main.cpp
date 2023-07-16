#include "CanDataHandler.h"

#include <Arduino.h>

const int kCanBaud = 500000; // CAN baud rate

CanDataHandler canDataHandler;

// TODO: this is debug stuff remove later
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

// TODO: this is debug stuff remove later
void printHeader()
{
  Serial.println("MAP | RPM | TPS | CLT");
}

void setup()
{
  Serial.begin(115200);

  canDataHandler.initCan(kCanBaud);

  if (Serial)
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

  if (Serial)
    printData(newData, map, rpm, tps, clt);

  if (!newData)
  {
    canDataHandler.setNewData(newData);
  }
  // end TODO
}