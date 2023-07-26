#include "CanDataHandler.h"

#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

constexpr uint32_t kBaseID = 1520; // This is set in megasquirt (typically 1520)

MegaCAN_broadcast_message_t CanDataHandler::_bCastMsg = {};

bool CanDataHandler::_newData = false;
MegaCAN CanDataHandler::_mega_can(kBaseID);

void CanDataHandler::initCan(const int& canBaud)
{
  can.begin();
  can.setBaudRate(canBaud);
  can.setMaxMB(FLEXCAN_MAILBOX::MB16); // sets maximum number of mailboxes for FlexCAN_T4 usage
  can.enableFIFO();
  can.enableFIFOInterrupt();
  can.onReceive(CanDataHandler::canMShandler); // when a CAN message is received, runs the canMShandler function
  can.mailboxStatus();
}

void CanDataHandler::pollCan()
{
  can.events();
}

std::vector<std::pair<String, String>> CanDataHandler::getGaugeData(std::vector<GaugeData> gauges)
{
  std::vector<std::pair<String, String>> data;

  for (auto gauge : gauges)
  {
    switch (gauge)
    {
    case kRPM:
      data.push_back({GaugeLabels[kRPM], _bCastMsg.rpm});
      break;
    case kAFR:
      data.push_back({GaugeLabels[kAFR], _bCastMsg.afr1_old});
      break;
    case kMAP:
      data.push_back({GaugeLabels[kMAP], _bCastMsg.map});
      break;
    case kMAT:
      data.push_back({GaugeLabels[kMAT], _bCastMsg.mat});
      break;
    case kTPS:
      data.push_back({GaugeLabels[kTPS], _bCastMsg.tps});
      break;
    case kCLT:
      data.push_back({GaugeLabels[kCLT], _bCastMsg.clt});
      break;
    default:
      break;
    };
  }

  return data;
}

// Routine used by FlexCAN for retrieving the received CAN data.
void CanDataHandler::canMShandler(const CAN_message_t& msg)
{
  if (!msg.flags.extended)
  {
    // broadcast data from MS does not use extended flag, therefore this should be broadcast data from MS
    _mega_can.getBCastData(msg.id, msg.buf, _bCastMsg);
    _newData = true;
  }
}

void CanDataHandler::setNewData(bool newData)
{
  _newData = newData;
}

bool CanDataHandler::getNewData()
{
  return _newData;
}