#include "EncoderHandler.h"

EncoderHandler::EncoderHandler(int ENC_1, int ENC_2, int ENC_B, int doubleClickSpeed)
    : _doubleClickSpeed(doubleClickSpeed)
{
  _encoder.begin(ENC_1, ENC_2);
  _button.attach(ENC_B, INPUT_PULLUP);
  _clicks = kNoClick;
}

void EncoderHandler::pollButton()
{
  _button.update();

  // A double click occurs when two clicks occur in the doubleClickSpeed window, otherwise it's a single click
  if (_button.pressed())
  {
    _clicks = (_clickTimer.hasPassed(_doubleClickSpeed)) ? kSingleClick : kDoubleClick;
    _clickTimer.restart();
  }
  else
  {
    _clicks = kNoClick;
  }
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

// Returns 1 for single click, 2 for double click, and 0 for no clicks
int EncoderHandler::buttonPressed()
{
  return _clicks;
}

void EncoderHandler::setEncoderInterval(int lowerLimit, int upperLimit, bool periodic)
{
  _encoder.setLimits(lowerLimit, upperLimit, periodic);
}

void EncoderHandler::setDoubleClickSpeed(int doubleClickSpeed)
{
  _doubleClickSpeed = doubleClickSpeed;
}

int EncoderHandler::getDoubleClickSpeed()
{
  return _doubleClickSpeed;
}