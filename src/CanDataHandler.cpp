#include "CanDataHandler.h"

extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can;

MegaCAN_broadcast_message_t bCastMsg;

constexpr uint32_t kBaseID = 1520; // This is set in megasquirt (typically 1520)
MegaCAN mega_can(kBaseID);

bool newData = false;

void canMShandler(const CAN_message_t& msg)
{
  if (!msg.flags.extended)
  {
    // broadcast data from MS does not use extended flag, therefore this should be broadcast data from MS
    mega_can.getBCastData(msg.id, msg.buf, bCastMsg);
    newData = true;
  }
}

float CanDataHandler::getGaugeData(GaugeData data)
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

void CanDataHandler::initializeCAN()
{
  can.begin();
  can.setBaudRate(kCanBaud);
  can.setMaxMB(FLEXCAN_MAILBOX::MB16); // sets maximum number of mailboxes for FlexCAN_T4 usage
  can.enableFIFO();
  can.enableFIFOInterrupt();
  can.onReceive(canMShandler); // when a CAN message is received, runs the canMShandler function
  can.mailboxStatus();
}