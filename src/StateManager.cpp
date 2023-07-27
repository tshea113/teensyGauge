#include "StateManager.h"

#include <utility>
#include <vector>

StateManager::StateManager(EncoderHandler& encoderHandler, CanDataHandler& canDataHandler,
                           DisplayHandler& displayHandler)
    : _encoderHandler(encoderHandler), _canDataHandler(canDataHandler), _displayHandler(displayHandler)
{
  _encoderHandler.setEncoderInterval(kGaugeMin, kGaugeMax, true);
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

  if (_encoderHandler.buttonPressed())
  {
    _select();
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

void StateManager::_select()
{
  switch (_menuState)
  {
  case kIdle:
    _menuState = kViewSelected;
    _updateEncoder();
    _displayHandler.moveGaugeCursor(0);
    break;
  default:
    _menuState = kIdle; // TODO: Delete
    Serial.println("Select is not supported on this view!");
  }
}

void StateManager::_updateEncoder()
{
  if (kViewSelected == _menuState)
  {
    switch (_displayHandler.getCurrentView())
    {
    case kQuadGauge:
      _encoderHandler.setEncoderInterval(0, 3, true);
      _encoderHandler.setEncoderValue(0);
    }
  }
}
