#pragma once

#include <CanDataHandler.h>
#include <DisplayHandler.h>
#include <EncoderHandler.h>

// TODO: Delete when implemented
// State manager should handle what data is currently being served to the user
// It will keep flags of what current menu is selected, what options are available, etc.
// Main will poll for inputs and push those to the state manager
// State manager will take these inputs and serve them to the display
// It will funnel the right CAN data to the display

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
  void _select();
  void _updateEncoder();
  std::vector<std::pair<GaugeData, String>> _loadStateData(GaugeView state);
};