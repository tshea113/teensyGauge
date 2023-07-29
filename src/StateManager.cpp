#include "StateManager.h"

#include <utility>
#include <vector>

StateManager::StateManager(EncoderHandler& encoderHandler, CanDataHandler& canDataHandler,
                           DisplayHandler& displayHandler)
    : _encoderHandler(encoderHandler), _canDataHandler(canDataHandler), _displayHandler(displayHandler)
{
  // Encoder needs to be initialized in the Idle view
  _encoderHandler.setEncoderInterval(kGaugeMin, kGaugeMax, true);
  _encoderHandler.setEncoderValue(_displayHandler.getCurrentView());

  // Need to feed the display some initial data
  _displayHandler.setCurrentData(_loadStateData(_displayHandler.getCurrentView()));
}

// Polls for new information to update the display
void StateManager::poll()
{
  _canDataHandler.pollCan();
  _encoderHandler.pollButton();

  // Handle user input
  if (_encoderHandler.encoderValueChanged())
  {
    _scrollGauge(_encoderHandler.getEncoderValue());
  }

  if (kSingleClick == _encoderHandler.buttonPressed() || kDoubleClick == _encoderHandler.buttonPressed())
  {
    _select(_encoderHandler.buttonPressed());
  }
}

// Retrieves the CAN data for the currently selected gauges and serves to the display.
void StateManager::serveData()
{
  std::vector<std::pair<GaugeData, String>> currentData = _displayHandler.getCurrentData();
  std::vector<GaugeData> currentGauges;

  for (auto gauge : currentData)
  {
    currentGauges.push_back(gauge.first);
  }

  currentData = _canDataHandler.getGaugeData(currentGauges);
  _displayHandler.setCurrentData(currentData);

  _displayHandler.display();
}

// Updates the gauge state to a new view, and feeds all necessary info to init that view.
void StateManager::_scrollGauge(int newState)
{
  switch (_menuState)
  {
  case kIdle:
    // Scroll through the list of views
    _displayHandler.setCurrentView(static_cast<GaugeView>(newState));
    _displayHandler.setCurrentData(_loadStateData(static_cast<GaugeView>(newState)));
    break;
  case kViewSelected:
    // Scroll through the individual gauges on a view
    _displayHandler.moveGaugeCursor(newState);
    break;
  default:
    Serial.println("This state does not support scrolling!");
  }
}

// Loads data needed for a given state
std::vector<std::pair<GaugeData, String>> StateManager::_loadStateData(GaugeView state)
{
  std::vector<GaugeData> currentGauges;

  // TODO: This should come from some cached value, so that users don't have to set this up every power cycle
  switch (state)
  {
  case kDashboard:
    currentGauges = {kRPM, kTPS, kMAP, kCLT};
    break;
  case kQuadGauge:
    currentGauges = {kRPM, kTPS, kMAP, kCLT};
    break;
  case kDualGauge:
    currentGauges = {kRPM, kTPS};
    break;
  case kSingleGauge:
    currentGauges = {kRPM};
    break;
  default:
    Serial.println("No stored data for this given state!");
  }

  return _canDataHandler.getGaugeData(currentGauges);
}

// Updates display and controls when a click is input
void StateManager::_select(int numClicks)
{
  if (kIdle == _menuState)
  {
    switch (_displayHandler.getCurrentView())
    {
    case kQuadGauge:
    case kDualGauge:
      // Single click brings up the gauge selection cursor
      _menuState = kViewSelected;
      _updateEncoder();
      _displayHandler.moveGaugeCursor(0);
      break;
    default:
      Serial.println("Select not supported on this gauge view!");
    }
  }
  else if (kViewSelected == _menuState)
  {
    // Double click goes back up to top level view
    if (kDoubleClick == numClicks)
    {
      _menuState = kIdle;
      _updateEncoder();
      _displayHandler.clearGaugeCursor();
    }
  }
  else
  {
    Serial.println("Select is not supported on this view!");
  }
}

// Updates the encoder range and current value based upon view state
void StateManager::_updateEncoder()
{
  if (kIdle == _menuState)
  {
    _encoderHandler.setEncoderInterval(kGaugeMin, kGaugeMax, true);
    _encoderHandler.setEncoderValue(_displayHandler.getCurrentView());
  }
  if (kViewSelected == _menuState)
  {
    switch (_displayHandler.getCurrentView())
    {
    case kQuadGauge:
      _encoderHandler.setEncoderInterval(0, 3, true);
      _encoderHandler.setEncoderValue(0);
      break;
    case kDualGauge:
      _encoderHandler.setEncoderInterval(0, 1, true);
      _encoderHandler.setEncoderValue(0);
      break;
    default:
      Serial.println("Encoder not valid for this view!");
    }
  }
}
