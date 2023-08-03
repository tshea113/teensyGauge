#include "CanDataHandler.h"

#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

constexpr uint32_t kBaseID = 1520; // This is set in megasquirt (typically 1520)

MegaCAN_broadcast_message_t CanDataHandler::_bCastMsg = {};
MegaCAN CanDataHandler::_mega_can(kBaseID);

void CanDataHandler::initCan(int canBaud)
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

std::vector<std::pair<GaugeData, String>> CanDataHandler::getGaugeData(const std::vector<GaugeData>& gauges)
{
  std::vector<std::pair<GaugeData, String>> data;

  for (auto gauge : gauges)
  {
    switch (gauge)
    {
    case kRPM:
      data.push_back({kRPM, _bCastMsg.rpm});
      break;
    case kAFR:
      data.push_back({kAFR, _bCastMsg.afr1_old});
      break;
    case kMAP:
      data.push_back({kMAP, _bCastMsg.map});
      break;
    case kMAT:
      data.push_back({kMAT, _bCastMsg.mat});
      break;
    case kVoltage:
      data.push_back({kVoltage, _bCastMsg.batt});
      break;
    case kTPS:
      data.push_back({kTPS, _bCastMsg.tps});
      break;
    case kCLT:
      data.push_back({kCLT, _bCastMsg.clt});
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
  }
}
