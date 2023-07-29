#include "CanDataHandler.h"

#include <Chrono.h>
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

constexpr uint32_t kBaseID = 1520; // This is set in megasquirt (typically 1520)

uint8_t counter = 0;
Chrono counterTimer;

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

  if (counterTimer.hasPassed(100))
  {
    counter++;
    counterTimer.restart();
  }
}

std::vector<std::pair<GaugeData, String>> CanDataHandler::getGaugeData(const std::vector<GaugeData>& gauges)
{
  std::vector<std::pair<GaugeData, String>> data;

  for (auto gauge : gauges)
  {
    switch (gauge)
    {
    case kRPM:
      // data.push_back({kRPM, _bCastMsg.rpm});
      data.push_back({kRPM, counter});
      break;
    case kAFR:
      data.push_back({kAFR, _bCastMsg.afr1_old});
      break;
    case kMAP:
      // data.push_back({kMAP, _bCastMsg.map});
      data.push_back({kMAP, counter / 2});
      break;
    case kMAT:
      data.push_back({kMAT, _bCastMsg.mat});
      break;
    case kTPS:
      // data.push_back({kTPS, _bCastMsg.tps});
      data.push_back({kTPS, counter + 1});
      break;
    case kCLT:
      // data.push_back({kCLT, _bCastMsg.clt});
      data.push_back({kCLT, counter + 10});
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
