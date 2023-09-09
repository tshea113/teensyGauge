#include "StateManager.h"

#include <CanDataHandler.h>
#include <EncoderHandler.h>

#include <utility>
#include <vector>

StateManager::StateManager(EncoderHandler& encoderHandler, CanDataHandler& canDataHandler,
                           DisplayHandler& displayHandler)
    : _encoderHandler(encoderHandler), _canDataHandler(canDataHandler), _displayHandler(displayHandler)
{
  _currentView = _displayHandler.getCurrentView();

  // Encoder needs to be initialized in the Idle view
  _encoderHandler.setEncoderInterval(int(GaugeView::kGaugeMin), int(GaugeView::kGaugeMax), true);
  _encoderHandler.setEncoderValue(int(_currentView));

  // Need to feed the display some initial data
  _displayHandler.setCurrentData(_loadStateData(_currentView));

  StateInfo info;
  _stateMap.insert(std::make_pair(kIdle, info));
}

// Polls for new information to update the display
void StateManager::poll()
{
  _canDataHandler.pollCan();
  _encoderHandler.pollButton();

  Clicks buttonPressed = _encoderHandler.buttonPressed();
  _currentView = _displayHandler.getCurrentView();
  _currentIndex = _encoderHandler.getEncoderValue();

  // Handle user input
  if (_encoderHandler.encoderValueChanged())
  {
    _scrollGauge(_currentIndex);
  }

  if (kSingleClick == buttonPressed || kDoubleClick == buttonPressed)
  {
    _select(buttonPressed);
  }
}

// Retrieves the CAN data for the currently selected gauges and serves to the display.
void StateManager::serveData()
{
  auto currentData = _displayHandler.getCurrentData();
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
  case kItemSelected:
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
  case GaugeView::kDashboard:
    currentGauges = {GaugeData::kAFR,     GaugeData::kCLT, GaugeData::kMAT, GaugeData::kMAP,
                     GaugeData::kVoltage, GaugeData::kFan, GaugeData::kWUE};
    break;
  case GaugeView::kQuadGauge:
    currentGauges = {GaugeData::kRPM, GaugeData::kTPS, GaugeData::kMAP, GaugeData::kCLT};
    break;
  case GaugeView::kDualGauge:
    currentGauges = {GaugeData::kRPM, GaugeData::kTPS};
    break;
  case GaugeView::kSingleGauge:
    currentGauges = {GaugeData::kRPM};
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
    switch (_currentView)
    {
    case GaugeView::kQuadGauge:
    case GaugeView::kDualGauge: {
      auto currentStateInfo = _getCurrentStateInfo(_menuState);
      currentStateInfo->second.index = _currentIndex;

      _menuState = kViewSelected;
      _updateEncoder(0);

      _displayHandler.moveGaugeCursor(0);
      _displayHandler.createBackArrow();
      break;
    }
    default:
      Serial.println("Select not supported on this gauge view!");
    }
  }
  else if (kViewSelected == _menuState)
  {
    if (kSingleClick == numClicks)
    {
      // Back arrow goes back up to top level view
      if (_encoderHandler.getMax() == _currentIndex)
      {
        _menuState = kIdle;

        auto currentStateInfo = _getCurrentStateInfo(_menuState);
        _updateEncoder(currentStateInfo->second.index);

        _displayHandler.clearGaugeCursor();
        _displayHandler.clearBackArrow();
      }
      else
      {
        // Save the index of the previous screen
        auto currentStateInfo = _getCurrentStateInfo(_menuState);
        currentStateInfo->second.index = _currentIndex;

        _menuState = kItemSelected;
        _updateEncoder(0);
      }
    }
  }
  else if (kItemSelected == _menuState)
  {
    if (kSingleClick == numClicks)
    {
      _menuState = kViewSelected;

      auto currentStateInfo = _getCurrentStateInfo(_menuState);
      _updateEncoder(currentStateInfo->second.index);
    }
  }
  else
  {
    Serial.println("Select is not supported on this view!");
  }
}

// Updates the encoder range and current value based upon view state
void StateManager::_updateEncoder(int initialValue)
{
  // Interval is dependent on the current state
  if (kIdle == _menuState)
  {
    _encoderHandler.setEncoderInterval(int(GaugeView::kGaugeMin), int(GaugeView::kGaugeMax), true);
  }
  else if (kViewSelected == _menuState)
  {
    switch (_currentView)
    {
    case GaugeView::kQuadGauge:
      _encoderHandler.setEncoderInterval(0, 4, true);
      break;
    case GaugeView::kDualGauge:
      _encoderHandler.setEncoderInterval(0, 2, true);
      break;
    default:
      Serial.println("Encoder not valid for this view!");
    }
  }
  else if (kItemSelected == _menuState)
  {
    _encoderHandler.setEncoderInterval(0, 1, true);
  }

  _encoderHandler.setEncoderValue(initialValue);
}

std::unordered_map<State, StateInfo>::iterator StateManager::_getCurrentStateInfo(State currentState)
{
  auto info = _stateMap.find(currentState);

  if (_stateMap.end() == info)
  {
    StateInfo stateInfo;
    _stateMap.insert(std::make_pair(currentState, stateInfo));
    info = _stateMap.find(currentState);
  }

  return info;
}
