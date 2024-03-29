#include "DisplayHandler.h"

DisplayHandler::DisplayHandler(int _tft_RST, int _tft_DC, int _tft_CS, int _screenHeight, int _screenWidth)
    : _screenHeight(_screenHeight), _screenWidth(_screenWidth), _tft(_tft_CS, _tft_DC, _tft_RST) // Hardware SPI
{
  // Need to keep track of the previously highlighted gauge when moving cursors. Setting -1 indicates not in use.
  _gaugeCursorIndex = -1;

  // GaugeMin and GaugeMax window the selectable gauges. Stuff in development can be put outside of this window
  // temporarily.
  _currentGaugeView = GaugeView::kGaugeMin;

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
  case GaugeView::kDashboard:
    _gaugeViewUpdated ? _drawDashboard() : _refreshDashboard();
    break;
  case GaugeView::kQuadGauge:
    _gaugeViewUpdated ? _drawQuad() : _refreshQuad();
    break;
  case GaugeView::kDualGauge:
    _gaugeViewUpdated ? _drawDual() : _refreshDual();
    break;
  case GaugeView::kSingleGauge:
    _gaugeViewUpdated ? _drawSingle() : _refreshSingle();
    break;
  default:
    Serial.print(int(_currentGaugeView));
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
  case GaugeView::kQuadGauge:
    _highlightQuadGauge(GC9A01A_WHITE, GC9A01A_BLACK);
    _gaugeCursorIndex = gaugeIndex;
    _highlightQuadGauge(GC9A01A_BLACK, GC9A01A_WHITE);
    break;
  case GaugeView::kDualGauge:
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
  case GaugeView::kQuadGauge:
    _highlightQuadGauge(GC9A01A_WHITE, GC9A01A_BLACK);
    break;
  case GaugeView::kDualGauge:
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
  _tft.setTextSize(int(fontSize));

  if ((unsigned int)dataIndex >= _currentData.size())
  {
    Serial.println("Data index outside of range!");
    return;
  }

  // To avoid flickering:
  // - Only update the data if it has changed
  // - Black out only the old data pixels

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
  if ((unsigned int)dataIndex >= _currentData.size())
  {
    Serial.println("Data index outside of range!");
    return;
  }

  _tft.setTextSize(int(fontSize));
  _tft.setTextColor(GC9A01A_WHITE);

  _tft.setCursor(cursorX - _getCenterOffset(fontSize, _currentData[dataIndex].second.length()), cursorY);
  _tft.println(_currentData[dataIndex].second);
}

// Draws the label given the data center X coordinate and top Y coordinate.
void DisplayHandler::_drawLabel(int dataIndex, FontSize fontSize, int cursorX, int cursorY)
{
  if ((unsigned int)dataIndex >= _currentData.size())
  {
    Serial.println("Data index outside of range!");
    return;
  }

  _tft.setTextSize(int(fontSize));
  _tft.setTextColor(GC9A01A_WHITE);

  _tft.setCursor(cursorX - _getCenterOffset(fontSize, GaugeLabels[int(_currentData[dataIndex].first)].length()),
                 cursorY);
  _tft.println(GaugeLabels[int(_currentData[dataIndex].first)]);
}

// Draws an icon if the data at the current index is active.
void DisplayHandler::_drawIcon(int dataIndex, const uint8_t* bitmap, int iconHeight, int iconWidth, int cursorX,
                               int cursorY, int color)
{
  if ((unsigned int)dataIndex >= _currentData.size())
  {
    Serial.println("Data index outside of range!");
    return;
  }

  if (_currentData[dataIndex].second == 1)
  {
    _tft.drawBitmap(cursorX, cursorY, bitmap, iconWidth, iconHeight, color);
  }
  else
  {
    _tft.drawBitmap(cursorX, cursorY, bitmap, iconWidth, iconHeight, GC9A01A_BLACK);
  }
}

// Highlights the label given the data center X coordinate and top Y coordinate.
void DisplayHandler::_highlightLabel(int dataIndex, FontSize fontSize, int cursorX, int cursorY, uint16_t textColor,
                                     uint16_t backgroundColor)
{
  _tft.setTextSize(int(fontSize));
  _tft.setTextColor(textColor);

  if ((unsigned int)dataIndex >= _currentData.size())
  {
    Serial.println("Data index outside of range!");
    return;
  }

  _tft.fillRect(cursorX - _getCenterOffset(fontSize, GaugeLabels[int(_currentData[dataIndex].first)].length()) - 1,
                cursorY - 1,
                _getFontWidth(FontSize::kFontSizeMedium) * GaugeLabels[int(_currentData[dataIndex].first)].length(),
                _getFontHeight(FontSize::kFontSizeMedium), backgroundColor);
  _tft.setCursor(
      cursorX - _getCenterOffset(FontSize::kFontSizeMedium, GaugeLabels[int(_currentData[dataIndex].first)].length()),
      cursorY);
  _tft.println(GaugeLabels[int(_currentData[dataIndex].first)]);
}

// Returns the width in pixels of a given font size
int DisplayHandler::_getFontWidth(FontSize fontSize) const
{
  // Font width grows in multiples of 6
  return int(fontSize) * 6;
}

// Returns the height in pixels of a given font size
int DisplayHandler::_getFontHeight(FontSize fontSize) const
{
  // Font height grows in multiples of 8
  return int(fontSize) * 8;
}

// Returns the offset needed to center text at a given coordinate
int DisplayHandler::_getCenterOffset(FontSize fontSize, int length) const
{
  return (length * _getFontWidth(fontSize)) / 2;
}

void DisplayHandler::_drawDashboard()
{
  clearScreen();

  if (_currentData.size() < 5)
  {
    Serial.println("Current data has less than 5 gauges!");
  }

  _drawData(0, FontSize::kFontSizeLarge, _screenWidth / 2, (_screenHeight / 2) - 90);
  _drawLabel(0, FontSize::kFontSizeMedium, _screenWidth / 2, (_screenHeight / 2) - 60);

  _drawData(1, FontSize::kFontSizeLarge, _screenWidth / 4, (_screenHeight / 2) - 40);
  _drawLabel(1, FontSize::kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) - 10);

  _drawData(2, FontSize::kFontSizeLarge, _screenWidth / 4, (_screenHeight / 2) + 20);
  _drawLabel(2, FontSize::kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) + 50);

  _drawData(3, FontSize::kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 40);
  _drawLabel(3, FontSize::kFontSizeMedium, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 10);

  _drawData(4, FontSize::kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 20);
  _drawLabel(4, FontSize::kFontSizeMedium, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 50);

  _drawIcon(5, fan_icon, 32, 32, _screenWidth - (_screenWidth / 4) - 30, (_screenHeight / 2) + 72, GC9A01A_YELLOW);
  _drawIcon(6, cold_icon, 32, 32, (_screenWidth / 2) - 16, (_screenHeight / 2) + 80, GC9A01A_BLUE);
}

void DisplayHandler::_refreshDashboard()
{
  if (_dataUpdated)
  {
    if (_oldData[0].second != _currentData[0].second)
    {
      _refreshData(0, FontSize::kFontSizeLarge, _screenWidth / 2, (_screenHeight / 2) - 90);
    }
    if (_oldData[1].second != _currentData[1].second)
    {
      _refreshData(1, FontSize::kFontSizeLarge, _screenWidth / 4, (_screenHeight / 2) - 40);
    }
    if (_oldData[2].second != _currentData[2].second)
    {
      _refreshData(2, FontSize::kFontSizeLarge, _screenWidth / 4, (_screenHeight / 2) + 20);
    }
    if (_oldData[3].second != _currentData[3].second)
    {
      _refreshData(3, FontSize::kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 40);
    }
    if (_oldData[4].second != _currentData[4].second)
    {
      _refreshData(4, FontSize::kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 20);
    }
    if (_oldData[5].second != _currentData[5].second)
    {
      _drawIcon(5, fan_icon, 32, 32, _screenWidth - (_screenWidth / 4) - 30, (_screenHeight / 2) + 72, GC9A01A_YELLOW);
    }
    if (_oldData[6].second != _currentData[6].second)
    {
      _drawIcon(6, cold_icon, 32, 32, (_screenWidth / 2) - 16, (_screenHeight / 2) + 80, GC9A01A_BLUE);
    }
  }
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

  _drawLabel(0, FontSize::kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) - 20);
  _drawLabel(1, FontSize::kFontSizeMedium, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 20);
  _drawLabel(2, FontSize::kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) + 6);
  _drawLabel(3, FontSize::kFontSizeMedium, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 6);

  _drawData(0, FontSize::kFontSizeLarge, _screenWidth / 4, (_screenHeight / 2) - 50);
  _drawData(1, FontSize::kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 50);
  _drawData(2, FontSize::kFontSizeLarge, _screenWidth / 4, (_screenHeight / 2) + 30);
  _drawData(3, FontSize::kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 30);
}

// Refreshes changed data on the 4 gauge view.
void DisplayHandler::_refreshQuad()
{
  if (_dataUpdated)
  {
    if (_oldData[0].second != _currentData[0].second)
    {
      _refreshData(0, FontSize::kFontSizeLarge, (_screenWidth / 4), (_screenHeight / 2) - 50);
    }
    if (_oldData[1].second != _currentData[1].second)
    {
      _refreshData(1, FontSize::kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) - 50);
    }
    if (_oldData[2].second != _currentData[2].second)
    {
      _refreshData(2, FontSize::kFontSizeLarge, (_screenWidth / 4), (_screenHeight / 2) + 30);
    }
    if (_oldData[3].second != _currentData[3].second)
    {
      _refreshData(3, FontSize::kFontSizeLarge, _screenWidth - (_screenWidth / 4), (_screenHeight / 2) + 30);
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

  _drawLabel(0, FontSize::kFontSizeMedium, _screenWidth / 2, (_screenHeight / 2) - 20);
  _drawLabel(1, FontSize::kFontSizeMedium, _screenWidth / 2, (_screenHeight / 2) + 6);

  _drawData(0, FontSize::kFontSizeXL, _screenWidth / 2, (_screenHeight / 2) - 75);
  _drawData(1, FontSize::kFontSizeXL, _screenWidth / 2, (_screenHeight / 2) + 55);
}

// Refreshes changed data on the 2 gauge view.
void DisplayHandler::_refreshDual()
{
  if (_dataUpdated)
  {
    if (_oldData[0].second != _currentData[0].second)
    {
      _refreshData(0, FontSize::kFontSizeXL, (_screenWidth / 2), (_screenHeight / 2) - 75);
    }
    if (_oldData[1].second != _currentData[1].second)
    {
      _refreshData(1, FontSize::kFontSizeXL, (_screenWidth / 2), (_screenHeight / 2) + 55);
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

  _drawLabel(0, FontSize::kFontSizeLarge, _screenWidth / 2, (_screenHeight / 2) + 55);

  _drawData(0, FontSize::kFontSizeXXXL, _screenWidth / 2, (_screenHeight / 2) - 70);
}

// Refreshes changed data on the 1 gauge view.
void DisplayHandler::_refreshSingle()
{
  if (_dataUpdated)
  {
    if (_oldData[0].second != _currentData[0].second)
    {
      _refreshData(0, FontSize::kFontSizeXXXL, (_screenWidth / 2), (_screenHeight / 2) - 70);
    }
  }
}

// Highlights a gauge to be used as a cursor. Invert can be set to move the cursor.
void DisplayHandler::_highlightQuadGauge(uint16_t textColor, uint16_t backgroundColor)
{
  switch (_gaugeCursorIndex)
  {
  case 0:
    _highlightLabel(_gaugeCursorIndex, FontSize::kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) - 20, textColor,
                    backgroundColor);
    break;
  case 1:
    _highlightLabel(_gaugeCursorIndex, FontSize::kFontSizeMedium, _screenWidth - (_screenWidth / 4),
                    (_screenHeight / 2) - 20, textColor, backgroundColor);
    break;
  case 2:
    _highlightLabel(_gaugeCursorIndex, FontSize::kFontSizeMedium, _screenWidth / 4, (_screenHeight / 2) + 6, textColor,
                    backgroundColor);
    break;
  case 3:
    _highlightLabel(_gaugeCursorIndex, FontSize::kFontSizeMedium, _screenWidth - (_screenWidth / 4),
                    (_screenHeight / 2) + 6, textColor, backgroundColor);
    break;
  }
}

// Highlights a gauge to be used as a cursor. Invert can be set to move the cursor.
void DisplayHandler::_highlightDualGauge(uint16_t textColor, uint16_t backgroundColor)
{
  switch (_gaugeCursorIndex)
  {
  case 0:
    _highlightLabel(_gaugeCursorIndex, FontSize::kFontSizeMedium, _screenWidth / 2, (_screenHeight / 2) - 20, textColor,
                    backgroundColor);
    break;
  case 1:
    _highlightLabel(_gaugeCursorIndex, FontSize::kFontSizeMedium, _screenWidth / 2, (_screenHeight / 2) + 6, textColor,
                    backgroundColor);
    break;
  }
}