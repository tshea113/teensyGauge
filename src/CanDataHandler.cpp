#include "CanDataHandler.h"

#include <FlexCAN_T4.h>

constexpr uint32_t kBaseID = 1520; // This is set in megasquirt (typically 1520)

MegaCAN_broadcast_message_t CanDataHandler::_bCastMsg = {};

bool CanDataHandler::_newData = false;
MegaCAN CanDataHandler::_mega_can(kBaseID);

CanDataHandler::CanDataHandler(const int& canBaud, FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>& can)
{
  can.begin();
  can.setBaudRate(canBaud);
  can.setMaxMB(FLEXCAN_MAILBOX::MB16); // sets maximum number of mailboxes for FlexCAN_T4 usage
  can.enableFIFO();
  can.enableFIFOInterrupt();
  can.onReceive(CanDataHandler::canMShandler); // when a CAN message is received, runs the canMShandler function
  can.mailboxStatus();
}

float CanDataHandler::getGaugeData(GaugeData data)
{
  switch (data)
  {
  case kRPM:
    return _bCastMsg.rpm;
  case kMAP:
    return _bCastMsg.map;
  case kMAT:
    return _bCastMsg.mat;
  case kTPS:
    return _bCastMsg.tps;
  case kCLT:
    return _bCastMsg.clt;
  default:
    return 0;
  };
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