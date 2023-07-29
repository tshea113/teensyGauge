#include "DisplayHandler.h"

DisplayHandler::DisplayHandler(int _tft_RST, int _tft_DC, int _tft_CS, int _screenHeight, int _screenWidth)
    : _screenHeight(_screenHeight), _screenWidth(_screenWidth), _tft(_tft_CS, _tft_DC, _tft_RST) // Hardware SPI
{
  // Need to keep track of the previously highlighted gauge when moving cursors. Setting -1 indicates not in use.
  _gaugeCursorIndex = -1;

  // GaugeMin and GaugeMax window the selectable gauges. Stuff in development can be put outside of this window
  // temporarily.
  _currentGaugeView = kGaugeMin;

  // We need to draw the first gauge.
  _gaugeViewUpdated = true;
}

void DisplayHandler::displayStartupScreen()
{
  _tft.begin();
  clearScreen();
  _tft.drawBitmap(20, 98, miata_logo, 200, 44, GC9A01A_RED);
}

void DisplayHandler::display()
{
  // If we select a new gauge, we need to redraw EVERYTHING.
  // Otherwise we are just refreshing the data.
  switch (_currentGaugeView)
  {
  case kDashboard:
    if (_gaugeViewUpdated)
      _displayDashboard();
    break;
  case kQuadGauge:
    _gaugeViewUpdated ? _drawQuad() : _refreshQuad();
    break;
  case kDualGauge:
    _gaugeViewUpdated ? _drawDual() : _refreshDual();
    break;
  case kSingleGauge:
    _gaugeViewUpdated ? _drawSingle() : _refreshSingle();
    break;
  default:
    Serial.print(_currentGaugeView);
    Serial.print(" is not a valid gauge index!\n");
  }

  _gaugeViewUpdated = false;
}

void DisplayHandler::clearScreen()
{
  _tft.fillScreen(GC9A01A_BLACK);
}

// Moves the cursor from the current gauge to the provided new index
void DisplayHandler::moveGaugeCursor(int gaugeIndex)
{
  switch (_currentGaugeView)
  {
  case kQuadGauge:
    _highlightQuadGauge(GC9A01A_WHITE, GC9A01A_BLACK);
    _gaugeCursorIndex = gaugeIndex;
    _highlightQuadGauge(GC9A01A_BLACK, GC9A01A_WHITE);
    break;
  case kDualGauge:
    _highlightDualGauge(GC9A01A_WHITE, GC9A01A_BLACK);
    _gaugeCursorIndex = gaugeIndex;
    _highlightDualGauge(GC9A01A_BLACK, GC9A01A_WHITE);
    break;
  default:
    Serial.println("Cursor not supported on this view!");
  }
}

void DisplayHandler::clearGaugeCursor()
{
  switch (_currentGaugeView)
  {
  case kQuadGauge:
    _highlightQuadGauge(GC9A01A_WHITE, GC9A01A_BLACK);
    break;
  case kDualGauge:
    _highlightDualGauge(GC9A01A_WHITE, GC9A01A_BLACK);
    break;
  default:
    Serial.println("Cursor not supported on this view!");
  }
  _gaugeCursorIndex = -1;
}

// Updates the gauge data and caches the old data for display refreshing
void DisplayHandler::setCurrentData(std::vector<std::pair<GaugeData, String>> newData)
{
  _oldData = _currentData;
  _currentData = newData;
  _dataUpdated = true;
}

std::vector<std::pair<GaugeData, String>> DisplayHandler::getCurrentData()
{
  return _currentData;
}

void DisplayHandler::setCurrentView(GaugeView newGauge)
{
  _currentGaugeView = newGauge;
  _gaugeViewUpdated = true;
}

GaugeView DisplayHandler::getCurrentView()
{
  return _currentGaugeView;
}

// Refreshes the data on a by digit basis given the data center X coordinate and top Y coordinate.
void DisplayHandler::_refreshData(int dataIndex, FontSize fontSize, int cursorX, int cursorY)
{
  _tft.setTextSize(fontSize);

  // To avoid flickering:
  // - Only update the data if it has changed
  // - GC9A01A_BLACK out only the old data pixels

  // We need to redraw everything if the data length changes.
  if (_oldData[dataIndex].second.length() != _currentData[dataIndex].second.length())
  {
    _tft.setTextColor(GC9A01A_BLACK);
    _tft.setCursor(cursorX - _getCenterOffset(fontSize, _oldData[dataIndex].second.length()), cursorY);
    _tft.println(_oldData[dataIndex].second);

    _tft.setTextColor(GC9A01A_WHITE);
    _tft.setCursor(cursorX - _getCenterOffset(fontSize, _currentData[dataIndex].second.length()), cursorY);
    _tft.println(_currentData[dataIndex].second);
  }
  // Otherwise only draw changed digits
  else
  {
    int dataStart = _getCenterOffset(fontSize, _oldData[dataIndex].second.length());
    for (uint8_t i = 0; i < _oldData[dataIndex].second.length(); i++)
    {
      if (_oldData[dataIndex].second.charAt(i) != _currentData[dataIndex].second.charAt(i))
      {
        _tft.setTextColor(GC9A01A_BLACK);
        _tft.setCursor(cursorX - dataStart + (_getFontWidth(fontSize) * i), cursorY);
        _tft.println(_oldData[dataIndex].second.charAt(i));

        _tft.setTextColor(GC9A01A_WHITE);
        _tft.setCursor(cursorX - dataStart + (_getFontWidth(fontSize) * i), cursorY);
        _tft.write(_currentData[dataIndex].second.charAt(i));
      }
    }
  }
}

// Draws the data given the data center X coordinate and top Y coordinate.
void DisplayHandler::_drawData(int dataIndex, FontSize fontSize, int cursorX, int cursorY)
{
  _tft.setTextSize(fontSize);
  _tft.setTextColor(GC9A01A_WHITE);

  _tft.setCursor(cursorX - _getCenterOffset(fontSize, _currentData[dataIndex].second.length()), cursorY);
  _tft.println(_currentData[dataIndex].second);
}

// Draws the label given the data center X coordinate and top Y coordinate.
void DisplayHandler::_drawLabel(int dataIndex, FontSize fontSize, int cursorX, int cursorY)
{
  _tft.setTextSize(fontSize);
  _tft.setTextColor(GC9A01A_WHITE);

  _tft.setCursor(cursorX - _getCenterOffset(fontSize, GaugeLabels[_currentData[dataIndex].first].length()), cursorY);
  _tft.println(GaugeLabels[_currentData[dataIndex].first]);
}

// Highlights the label given the data center X coordinate and top Y coordinate.
void DisplayHandler::_highlightLabel(int dataIndex, FontSize fontSize, int cursorX, int cursorY, uint16_t textColor,
                                     uint16_t backgroundColor)
{
  _tft.setTextSize(fontSize);
  _tft.setTextColor(textColor);

  _tft.fillRect(cursorX - _getCenterOffset(fontSize, GaugeLabels[_currentData[dataIndex].first].length()) - 1,
                cursorY - 1, _getFontWidth(kFontSizeMedium) * GaugeLabels[_currentData[dataIndex].first].length(),
                _getFontHeight(kFontSizeMedium), backgroundColor);
  _tft.setCursor(cursorX - _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[dataIndex].first].length()),
                 cursorY);
  _tft.println(GaugeLabels[_currentData[dataIndex].first]);
}

// Returns the width in pixels of a given font size
int DisplayHandler::_getFontWidth(FontSize fontSize) const
{
  // Font width grows in multiples of 6
  return fontSize * 6;
}

// Returns the height in pixels of a given font size
int DisplayHandler::_getFontHeight(FontSize fontSize) const
{
  // Font height grows in multiples of 8
  return fontSize * 8;
}

// Returns the offset needed to center text at a given coordinate
int DisplayHandler::_getCenterOffset(FontSize fontSize, int length) const
{
  return (length * _getFontWidth(fontSize)) / 2;
}

void DisplayHandler::_displayDashboard()
{
  clearScreen();

  _tft.setTextSize(kFontSizeMedium);
  _tft.setTextColor(GC9A01A_WHITE);

  _tft.setCursor(5, _screenHeight / 2);

  _tft.println("Dashboard goes here!");
}

void DisplayHandler::_drawQuad()
{
  clearScreen();

  _tft.drawFastHLine(0, _screenHeight / 2, _screenWidth, GC9A01A_WHITE);
  _tft.drawFastVLine(_screenWidth / 2, 0, _screenHeight, GC9A01A_WHITE);

  if (_currentData.size() < 4)
  {
    Serial.println("Current data has less than 4 gauges!");
  }

  _drawLabel(0, kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) - 20);
  _drawLabel(1, kFontSizeMedium, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 20);
  _drawLabel(2, kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) + 6);
  _drawLabel(3, kFontSizeMedium, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 6);

  _drawData(0, kFontSizeLarge, _screenWidth / 4, (_screenHeight / 2) - 50);
  _drawData(1, kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 50);
  _drawData(2, kFontSizeLarge, _screenWidth / 4, (_screenHeight / 2) + 30);
  _drawData(3, kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 30);
}

// Refreshes changed data on the 4 gauge view.
void DisplayHandler::_refreshQuad()
{
  if (_dataUpdated)
  {
    if (_oldData[0].second != _currentData[0].second)
    {
      _refreshData(0, kFontSizeLarge, (_screenWidth / 4), (_screenHeight / 2) - 50);
    }
    if (_oldData[1].second != _currentData[1].second)
    {
      _refreshData(1, kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 50);
    }
    if (_oldData[2].second != _currentData[2].second)
    {
      _refreshData(2, kFontSizeLarge, (_screenWidth / 4), (_screenHeight / 2) + 30);
    }
    if (_oldData[3].second != _currentData[3].second)
    {
      _refreshData(3, kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 30);
    }
  }
}

void DisplayHandler::_drawDual()
{
  clearScreen();

  _tft.drawFastHLine(0, _screenHeight / 2, _screenWidth, GC9A01A_WHITE);

  if (_currentData.size() < 2)
  {
    Serial.println("Current data has less than 2 gauges!");
  }

  _drawLabel(0, kFontSizeMedium, _screenWidth / 2, (_screenHeight / 2) - 20);
  _drawLabel(1, kFontSizeMedium, _screenWidth / 2, (_screenHeight / 2) + 6);

  _drawData(0, kFontSizeXL, _screenWidth / 2, (_screenHeight / 2) - 75);
  _drawData(1, kFontSizeXL, _screenWidth / 2, (_screenHeight / 2) + 55);
}

// Refreshes changed data on the 2 gauge view.
void DisplayHandler::_refreshDual()
{
  if (_dataUpdated)
  {
    if (_oldData[0].second != _currentData[0].second)
    {
      _refreshData(0, kFontSizeXL, (_screenWidth / 2), (_screenHeight / 2) - 75);
    }
    if (_oldData[1].second != _currentData[1].second)
    {
      _refreshData(1, kFontSizeXL, (_screenWidth / 2), (_screenHeight / 2) + 55);
    }
  }
}

void DisplayHandler::_drawSingle()
{
  clearScreen();

  if (_currentData.size() < 1)
  {
    Serial.println("Current data has less than 1 gauge!");
  }

  _drawLabel(0, kFontSizeLarge, _screenWidth / 2, (_screenHeight / 2) + 55);

  _drawData(0, kFontSizeXXXL, _screenWidth / 2, (_screenHeight / 2) - 70);
}

// Refreshes changed data on the 1 gauge view.
void DisplayHandler::_refreshSingle()
{
  if (_dataUpdated)
  {
    if (_oldData[0].second != _currentData[0].second)
    {
      _refreshData(0, kFontSizeXXXL, (_screenWidth / 2), (_screenHeight / 2) - 70);
    }
  }
}

// Highlights a gauge to be used as a cursor. Invert can be set to move the cursor.
void DisplayHandler::_highlightQuadGauge(uint16_t textColor, uint16_t backgroundColor)
{
  switch (_gaugeCursorIndex)
  {
  case 0:
    _highlightLabel(_gaugeCursorIndex, kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) - 20, textColor,
                    backgroundColor);
    break;
  case 1:
    _highlightLabel(_gaugeCursorIndex, kFontSizeMedium, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 20,
                    textColor, backgroundColor);
    break;
  case 2:
    _highlightLabel(_gaugeCursorIndex, kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) + 6, textColor,
                    backgroundColor);
    break;
  case 3:
    _highlightLabel(_gaugeCursorIndex, kFontSizeMedium, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 6,
                    textColor, backgroundColor);
    break;
  }
}

// Highlights a gauge to be used as a cursor. Invert can be set to move the cursor.
void DisplayHandler::_highlightDualGauge(uint16_t textColor, uint16_t backgroundColor)
{
  switch (_gaugeCursorIndex)
  {
  case 0:
    _highlightLabel(_gaugeCursorIndex, kFontSizeMedium, _screenWidth / 2, (_screenHeight / 2) - 20, textColor,
                    backgroundColor);
    break;
  case 1:
    _highlightLabel(_gaugeCursorIndex, kFontSizeMedium, _screenWidth / 2, (_screenHeight / 2) + 6, textColor,
                    backgroundColor);
    break;
  }
}