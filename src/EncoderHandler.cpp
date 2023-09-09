#include "EncoderHandler.h"

EncoderHandler::EncoderHandler(int ENC_1, int ENC_2, int ENC_B, int doubleClickSpeed)
    : _doubleClickSpeed(doubleClickSpeed)
{
  _encoder.begin(ENC_1, ENC_2);
  _button.attach(ENC_B, INPUT_PULLUP);
  _clicks = kNoClick;
  _inClickEvent = false;
}

void EncoderHandler::pollButton()
{
  _button.update();
  if (!_inClickEvent)
  {
    // When the button is first pressed, start a timer to determine if it is a double click
    if (_button.pressed())
    {
      _clickTimer.restart();
      _inClickEvent = true;
    }
  }
  else
  {
    // A double click occurs when two clicks occur in the doubleClickSpeed window, otherwise it's a single click
    if (_clickTimer.hasPassed(_doubleClickSpeed))
    {
      _clicks = kSingleClick;
      _inClickEvent = false;
    }
    else if (_button.pressed())
    {
      _clicks = kDoubleClick;
      _inClickEvent = false;
    }
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
// Clears the click count when queried
Clicks EncoderHandler::buttonPressed()
{
  Clicks click = _clicks;
  _clicks = kNoClick;
  return click;
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