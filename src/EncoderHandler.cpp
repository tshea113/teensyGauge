#include "EncoderHandler.h"

EncoderHandler::EncoderHandler(int ENC_1, int ENC_2, int ENC_B, int doubleClickSpeed)
    : _doubleClickSpeed(doubleClickSpeed)
{
  _encoder.begin(ENC_1, ENC_2);
  _button.attach(ENC_B, INPUT_PULLUP);
  _clicks.singleClick = false;
  _clicks.doubleClick = false;

  // Set some arbitrary default values
  _encoder.setLimits(0, 1, false);
  _min = 0;
  _max = 1;
}

void EncoderHandler::pollButton()
{
  _button.update();
  // When the button is first pressed, register the click and start a timer to
  // determine if it is a double click.
  if (_button.pressed())
  {
    if (_clickTimer.hasPassed(_doubleClickSpeed))
    {
      _clicks.singleClick = true;
      _clickTimer.restart();
    }
    else
    {
      _clicks.doubleClick = true;
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

// Returns the struct of click information
Clicks EncoderHandler::buttonPressed()
{
  Clicks click = _clicks;
  _clicks.singleClick = false;
  _clicks.doubleClick = false;
  return click;
}

void EncoderHandler::setEncoderInterval(int lowerLimit, int upperLimit, bool periodic)
{
  _encoder.setLimits(lowerLimit, upperLimit, periodic);
  _min = lowerLimit;
  _max = upperLimit;
}

void EncoderHandler::setDoubleClickSpeed(int doubleClickSpeed)
{
  _doubleClickSpeed = doubleClickSpeed;
}

int EncoderHandler::getDoubleClickSpeed()
{
  return _doubleClickSpeed;
}

int EncoderHandler::getMin()
{
  return _min;
}

int EncoderHandler::getMax()
{
  return _max;
}