#pragma once

#include "FlexCAN_T4.h"
#include "MegaCAN.h"

const int kCanBaud = 500000;    // CAN baud rate
const uint32_t kBaseID = 1520;  // Must set to match Megasquirt Settings!

extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

MegaCAN mega_can;
MegaCAN_broadcast_message_t bCastMsg;

bool newData = false;

void canMShandler(const CAN_message_t msg)
{
  if (!msg.flags.extended)
  { 
    //broadcast data from MS does not use extended flag, therefore this should be broadcast data from MS
    mega_can.getBCastData(msg.id, msg.buf, bCastMsg);
    newData = true;
  }
}

void initializeCAN()
{
  can.begin();
  can.setBaudRate(kCanBaud);
  can.setMaxMB(FLEXCAN_MAILBOX::MB16); //sets maximum number of mailboxes for FlexCAN_T4 usage
  can.enableFIFO();
  can.enableFIFOInterrupt();
  can.onReceive(canMShandler); //when a CAN message is received, runs the canMShandler function
  can.mailboxStatus();
}

void printData()
{
  float MAP = bCastMsg.map;
  float RPM = bCastMsg.rpm;
  float TPS = bCastMsg.tps;
  Serial.print(MAP); Serial.print(" | "); //should be kPa
  Serial.print(RPM); Serial.print(" | "); //should be rpm
  Serial.println(TPS);                    //should be %
}