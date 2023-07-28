#pragma once

#include <CanDataHandler.h>
#include <DisplayHandler.h>
#include <EncoderHandler.h>

// This is used to track the current state of the UI.
// - Idle consists of a scrollable list of views (dashboard, quad, dual, etc.).
// - Selecting a given view will allow you to scroll through the items on that view.
// - Selecting an item will allow you to scroll through a list of items to replace in the current view.
enum State : int
{
  kIdle = 0,
  kViewSelected = 1,
  kItemSelected = 2,
  kSettingsSelected = 3,
};

class StateManager
{
public:
  StateManager() = delete;
  StateManager(EncoderHandler& encoderHandler, CanDataHandler& canDataHandler, DisplayHandler& displayHandler);

  void poll();
  void serveData();

private:
  EncoderHandler& _encoderHandler;
  CanDataHandler& _canDataHandler;
  DisplayHandler& _displayHandler;

  State _menuState;

  void _scrollGauge(int newValue);
  void _select(int numClicks);
  void _updateEncoder();
  std::vector<std::pair<GaugeData, String>> _loadStateData(GaugeView state);
};