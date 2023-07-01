#pragma once

#include "FlexCAN_T4.h"
#include "MegaCAN.h"

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

constexpr int kCanBaud = 500000; // CAN baud rate

const String GaugeLabels[] = {"RPM",        "AFR",        "Coolant",    "MAP",        "MAT",      "Boost",
                              "Voltage",    "TPS",        "Knock",      "Barometer",  "EGO Corr", "IAC",
                              "Sprk Dwell", "Boost Duty", "Idl Target", "AFR Target", "Timing"};

class CanDataHandler
{
public:
  CanDataHandler();

  void initializeCAN();
  float getGaugeData(GaugeData data);

private:
  MegaCAN_broadcast_message_t _canData;
};