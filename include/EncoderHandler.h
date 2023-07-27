#pragma once

#include <Bounce2.h>
#include <EncoderTool.h>

class EncoderHandler
{
public:
  EncoderHandler() = delete;
  EncoderHandler(int ENC_1, int ENC_2, int ENC_B);

  void pollButton();

  void setEncoderValue(int value);
  int getEncoderValue();
  bool encoderValueChanged();
  void setEncoderInterval(int lowerLimit, int upperLimit, bool periodic);

  bool buttonPressed();

private:
  EncoderTool::Encoder _encoder;
  Bounce2::Button _button;
};