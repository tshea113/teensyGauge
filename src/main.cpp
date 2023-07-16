#include "CanDataHandler.h"
#include "DisplayHandler.h"

#include <Arduino.h>
#include <utility>

// HW specific definitions
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

#define SCREEN_WIDTH 240
#define SCREEEN_HEIGHT 240

// CAN baud rate
const int kCanBaud = 500000;

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

DisplayHandler displayHandler(TFT_RST, TFT_DC, TFT_CS, SCREEEN_HEIGHT, SCREEN_WIDTH);

void setup()
{
  displayHandler.displayStartupScreen();

  Serial.begin(115200);

  canDataHandler.initCan(kCanBaud);

  delay(1000);

  if (Serial)
    printHeader();

  // Test the onboard LED in startup
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  displayHandler.clearScreen();
}

void loop()
{
  can.events();

  // TODO: This should be set by user not hard coded.
  float map = canDataHandler.getGaugeData(kMAP);
  float rpm = canDataHandler.getGaugeData(kRPM);
  float tps = canDataHandler.getGaugeData(kTPS);
  float clt = canDataHandler.getGaugeData(kCLT);
  bool newData = canDataHandler.getNewData();

  std::pair<String, String> data[] = {
      {GaugeLabels[kRPM], rpm}, {GaugeLabels[kCLT], clt}, {GaugeLabels[kTPS], tps}, {GaugeLabels[kMAP], map}};

  displayHandler.displayQuad(data);

  // TODO: Remove this debug code once the display is working
  if (Serial)
    printData(newData, map, rpm, tps, clt);

  if (!newData)
  {
    canDataHandler.setNewData(newData);
  }
  // end TODO
}