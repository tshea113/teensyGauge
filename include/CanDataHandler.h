#pragma once

#include <FlexCAN_T4.h>
#include <MegaCAN.h>

#include <utility>
#include <vector>

extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

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

const std::vector<String> GaugeLabels = {"RPM",        "AFR",        "Coolant",    "MAP",        "MAT",      "Boost",
                                         "Voltage",    "TPS",        "Knock",      "Barometer",  "EGO Corr", "IAC",
                                         "Sprk Dwell", "Boost Duty", "Idl Target", "AFR Target", "Timing"};

class CanDataHandler
{
public:
  CanDataHandler() = default;

  void pollCan();

  std::vector<std::pair<GaugeData, String>> getGaugeData(const std::vector<GaugeData>& data);

  static void initCan(int canBaud);
  static void canMShandler(const CAN_message_t& msg);
  static void setNewData(bool newData);
  static bool getNewData();

private:
  static bool _newData;

  static MegaCAN _mega_can;
  static MegaCAN_broadcast_message_t _bCastMsg;
};