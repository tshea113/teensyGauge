#pragma once

#include "FlexCAN_T4.h"
#include "MegaCAN.h"

const int kCanBaud = 500000;  // CAN baud rate
const uint32_t baseID = 1520; // This is set in megasquirt (typically 1520)

extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

MegaCAN mega_can;
MegaCAN_broadcast_message_t bCastMsg;

bool newData = false;

enum GaugeData : int
{
  kRPM = 0,
  kAFR = 1,
  kCLT = 2,
  kMAP = 3,
  kMAT = 4,
  kBoost = 5,
  kVoltage = 6,
  kTPS = 7,
  kKnock = 8,
  kBarometer = 9,
  kEGOCorrection = 10,
  kIAC = 11,
  kSparkDwell = 12,
  kBoostDuty = 13,
  kIdleTarget = 14,
  kAfrTarget = 15,
  kTiming = 16
};

String GaugeLabels[] = {"RPM",        "AFR",        "Coolant",    "MAP",        "MAT",      "Boost",
                        "Voltage",    "TPS",        "Knock",      "Barometer",  "EGO Corr", "IAC",
                        "Sprk Dwell", "Boost Duty", "Idl Target", "AFR Target", "Timing"};

void canMShandler(const CAN_message_t& msg)
{
  if (!msg.flags.extended)
  {
    // broadcast data from MS does not use extended flag, therefore this should be broadcast data from MS
    mega_can.getBCastData(msg.id, msg.buf, bCastMsg);
    newData = true;
  }
}

void initializeCAN()
{
  can.begin();
  can.setBaudRate(kCanBaud);
  can.setMaxMB(FLEXCAN_MAILBOX::MB16); // sets maximum number of mailboxes for FlexCAN_T4 usage
  can.enableFIFO();
  can.enableFIFOInterrupt();
  can.onReceive(canMShandler); // when a CAN message is received, runs the canMShandler function
  can.mailboxStatus();
}

float getGaugeData(GaugeData data)
{
  switch (data)
  {
  case kRPM:
    return bCastMsg.rpm;
  case kMAP:
    return bCastMsg.map;
  case kMAT:
    return bCastMsg.mat;
  case kTPS:
    return bCastMsg.tps;
  case kCLT:
    return bCastMsg.clt;
  default:
    return 0;
  };
}