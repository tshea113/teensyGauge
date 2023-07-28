#pragma once

#include <Bounce2.h>
#include <Chrono.h>
#include <EncoderTool.h>

enum Clicks : int
{
  kNoClick = 0,
  kSingleClick = 1,
  kDoubleClick = 2,
};

class EncoderHandler
{
public:
  EncoderHandler() = delete;
  EncoderHandler(int ENC_1, int ENC_2, int ENC_B, int doubleClickSpeed);

  void pollButton();

  void setEncoderValue(int value);
  int getEncoderValue();
  bool encoderValueChanged();
  void setEncoderInterval(int lowerLimit, int upperLimit, bool periodic);
  void setDoubleClickSpeed(int doubleClickSpeed);
  int getDoubleClickSpeed();

  int buttonPressed();

private:
  EncoderTool::Encoder _encoder;
  Bounce2::Button _button;

  Chrono _clickTimer;
  Clicks _clicks;
  int _doubleClickSpeed;
};