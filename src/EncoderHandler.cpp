#include "EncoderHandler.h"

EncoderHandler::EncoderHandler(int ENC_1, int ENC_2, int ENC_B)
{
  _encoder.begin(ENC_1, ENC_2);
  _button.attach(ENC_B, INPUT_PULLUP);
}

void EncoderHandler::pollButton()
{
  _button.update();
}

void EncoderHandler::setEncoderValue(int value)
{
  _encoder.setValue(value);
}

int EncoderHandler::getEncoderValue()
{
  return _encoder.getValue();
}

bool EncoderHandler::encoderValueChanged()
{
  return _encoder.valueChanged();
}

bool EncoderHandler::buttonPressed()
{
  return _button.pressed();
}

void EncoderHandler::setEncoderInterval(int lowerLimit, int upperLimit, bool periodic)
{
  _encoder.setLimits(lowerLimit, upperLimit, periodic);
}