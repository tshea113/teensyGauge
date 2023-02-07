#include <Arduino.h>
#include "FlexCAN_T4.h"
#include "MegaCAN.h"
#include "constants.h"

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;
MegaCAN mega_can;

void setup() {
  can.begin();
  can.setBaudRate(kCanBaud);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void initializeCAN()
{
  can.begin();
  can.setBaudRate(500000); //set to 500000 for normal Megasquirt usage - need to change Megasquirt firmware to change MS CAN baud rate
  can.setMaxMB(16); //sets maximum number of mailboxes for FlexCAN_T4 usage
  can.enableFIFO();
  can.enableFIFOInterrupt();
  can.onReceive(canMShandler); //when a CAN message is received, runs the canMShandler function
  can.mailboxStatus();
}

void canMShandler(const CAN_message_t &msg)
{
  // For Megasquirt CAN broadcast data:
  if (!msg.flags.extended) { //broadcast data from MS does not use extended flag, therefore this should be broadcast data from MS
    //unpack megasquirt broadcast data into bCastMsg:
    MegaCAN.getBCastData(msg.id, msg.buf, bCastMsg); //baseID fixed in library based on const parameter entered for baseID above - converts the raw CAN id and buf to bCastMsg format

    if (msg.id == finalID) {
      /*~~~Final message for this batch of data, do stuff with the data - this is a simple example~~~*/
      float MAP = bCastMsg.map; //you could work directly with bCastMsg.map (or any parameter), or store as a separate variable as in this example
      float RPM = bCastMsg.rpm;
      float TPS = bCastMsg.tps;
      Serial.print(MAP); Serial.print(" | "); //should be kPa
      Serial.print(RPM); Serial.print(" | "); //should be rpm
      Serial.println(TPS);                      //should be %
    }
  }
}