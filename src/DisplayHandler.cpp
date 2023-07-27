#include "DisplayHandler.h"

DisplayHandler::DisplayHandler(int _tft_RST, int _tft_DC, int _tft_CS, int _screenHeight, int _screenWidth)
    : _screenHeight(_screenHeight), _screenWidth(_screenWidth), _tft(_tft_CS, _tft_DC, _tft_RST) // Hardware SPI
{
  // Need to keep track of the previously highlighted gauge when moving cursors. Setting -1 indicates not in use.
  _gaugeCursorIndex = -1;

  // We need to draw the first gauge.
  _gaugeViewUpdated = true;
}

void DisplayHandler::displayStartupScreen()
{
  _tft.begin();
  clearScreen();
  _tft.drawBitmap(20, 98, miata_logo, 200, 44, RED);
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
    _gaugeViewUpdated ? _displayQuad() : _refreshQuad();
    break;
  case kDualGauge:
    if (_gaugeViewUpdated)
      _displayDual();
    break;
  case kSingleGauge:
    if (_gaugeViewUpdated)
      _displaySingle();
    break;
  default:
    Serial.print(_currentGaugeView);
    Serial.print(" is not a valid gauge index!\n");
  }

  _gaugeViewUpdated = false;
}

void DisplayHandler::clearScreen()
{
  _tft.fillScreen(BLACK);
}

// Moves the cursor from the current gauge to the provided new index
void DisplayHandler::moveGaugeCursor(int gaugeIndex)
{
  switch (_currentGaugeView)
  {
  case kQuadGauge:
    _highlightQuadGauge(WHITE, BLACK);
    _gaugeCursorIndex = gaugeIndex;
    _highlightQuadGauge(BLACK, WHITE);
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
    _highlightQuadGauge(WHITE, BLACK);
    _gaugeCursorIndex = -1;
    break;
  default:
    Serial.println("Cursor not supported on this view!");
  }
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

void DisplayHandler::_displayDashboard()
{
  clearScreen();

  _tft.setTextSize(kFontSizeMedium);
  _tft.setTextColor(WHITE);

  _tft.setCursor(5, _screenHeight / 2);

  _tft.println("Dashboard goes here!");
}

// Displays 4 gauges divided into 4 quadrants.
// Labels 10 characters and data 5 characters max.
void DisplayHandler::_displayQuad()
{
  clearScreen();

  _tft.drawFastHLine(0, _screenHeight / 2, _screenWidth, WHITE);
  _tft.drawFastVLine(_screenWidth / 2, 0, _screenHeight, WHITE);

  _tft.setTextSize(kFontSizeMedium);
  _tft.setTextColor(WHITE);

  if (_currentData.size() < 4)
  {
    Serial.println("Current data has less than 4 gauges!");
  }

  // Print labels
  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[0].first].length()),
                 (_screenHeight / 2) - 20);
  _tft.println(GaugeLabels[_currentData[0].first]);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) -
                     _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[1].first].length()),
                 (_screenHeight / 2) - 20);
  _tft.println(GaugeLabels[_currentData[1].first]);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[2].first].length()),
                 (_screenHeight / 2) + 6);
  _tft.println(GaugeLabels[_currentData[2].first]);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) -
                     _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[3].first].length()),
                 (_screenHeight / 2) + 6);
  _tft.println(GaugeLabels[_currentData[3].first]);

  // Print data
  _tft.setTextSize(kFontSizeLarge);
  _tft.setTextColor(WHITE);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[0].second.length()),
                 (_screenHeight / 2) - 50);
  _tft.println(_currentData[0].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[1].second.length()),
                 (_screenHeight / 2) - 50);
  _tft.println(_currentData[1].second);

  _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[2].second.length()),
                 (_screenHeight / 2) + 30);
  _tft.println(_currentData[2].second);

  _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[3].second.length()),
                 (_screenHeight / 2) + 30);
  _tft.println(_currentData[3].second);
}

// Refreshes changed data on the 4 gauge view.
void DisplayHandler::_refreshQuad()
{
  _tft.setTextSize(kFontSizeLarge);

  // TODO: We should really only overwrite individual characters that change. There is still some minor flickering.
  if (_dataUpdated)
  {
    // To avoid flickering:
    // - Only update the data if it has changed
    // - Black out only the old data pixels
    if (_oldData[0] != _currentData[0])
    {
      _tft.setTextColor(BLACK);
      _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _oldData[0].second.length()),
                     (_screenHeight / 2) - 50);
      _tft.println(_oldData[0].second);

      _tft.setTextColor(WHITE);
      _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[0].second.length()),
                     (_screenHeight / 2) - 50);
      _tft.println(_currentData[0].second);
    }
    if (_oldData[1] != _currentData[1])
    {
      _tft.setTextColor(BLACK);
      _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _oldData[1].second.length()),
                     (_screenHeight / 2) - 50);
      _tft.println(_oldData[1].second);

      _tft.setTextColor(WHITE);
      _tft.setCursor(_screenWidth - (_screenWidth / 4) -
                         _getCenterOffset(kFontSizeLarge, _currentData[1].second.length()),
                     (_screenHeight / 2) - 50);
      _tft.println(_currentData[1].second);
    }
    if (_oldData[2] != _currentData[2])
    {
      _tft.setTextColor(BLACK);
      _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _oldData[2].second.length()),
                     (_screenHeight / 2) + 30);
      _tft.println(_oldData[2].second);

      _tft.setTextColor(WHITE);
      _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _currentData[2].second.length()),
                     (_screenHeight / 2) + 30);
      _tft.println(_currentData[2].second);
    }
    if (_oldData[3] != _currentData[3])
    {
      _tft.setTextColor(BLACK);
      _tft.setCursor(_screenWidth - (_screenWidth / 4) - _getCenterOffset(kFontSizeLarge, _oldData[3].second.length()),
                     (_screenHeight / 2) + 30);
      _tft.println(_oldData[3].second);

      _tft.setTextColor(WHITE);
      _tft.setCursor(_screenWidth - (_screenWidth / 4) -
                         _getCenterOffset(kFontSizeLarge, _currentData[3].second.length()),
                     (_screenHeight / 2) + 30);
      _tft.println(_currentData[3].second);
    }
  }
}

void DisplayHandler::_displayDual()
{
  clearScreen();

  _tft.setTextSize(kFontSizeMedium);
  _tft.setTextColor(WHITE);

  _tft.setCursor(5, _screenHeight / 2);

  _tft.println("Dual gauges goes here!");
}

void DisplayHandler::_displaySingle()
{
  clearScreen();

  _tft.setTextSize(kFontSizeMedium);
  _tft.setTextColor(WHITE);

  _tft.setCursor(5, _screenHeight / 2);

  _tft.println("Single gauges goes here!");
}

// Highlights a gauge to be used as a cursor. Invert can be set to move the cursor.
void DisplayHandler::_highlightQuadGauge(const uint16_t textColor, const uint16_t backgroundColor)
{
  _tft.setTextColor(textColor);
  _tft.setTextSize(kFontSizeMedium);
  switch (_gaugeCursorIndex)
  {
  case 0:
    _tft.fillRect((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[0].first].length()) -
                      1,
                  (_screenHeight / 2) - 20 - 1, kFontWidthMedium * GaugeLabels[_currentData[0].first].length(),
                  kFontHeightMedium, backgroundColor);
    _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[0].first].length()),
                   (_screenHeight / 2) - 20);
    _tft.println(GaugeLabels[_currentData[0].first]);
    break;
  case 1:
    _tft.fillRect(_screenWidth - (_screenWidth / 4) -
                      _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[1].first].length()) - 1,
                  (_screenHeight / 2) - 20 - 1, kFontWidthMedium * GaugeLabels[_currentData[1].first].length(),
                  kFontHeightMedium, backgroundColor);
    _tft.setCursor(_screenWidth - (_screenWidth / 4) -
                       _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[1].first].length()),
                   (_screenHeight / 2) - 20);
    _tft.println(GaugeLabels[_currentData[1].first]);
    break;
  case 2:
    _tft.fillRect((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[2].first].length()) -
                      1,
                  (_screenHeight / 2) + 6 - 1, kFontWidthMedium * GaugeLabels[_currentData[2].first].length(),
                  kFontHeightMedium, backgroundColor);
    _tft.setCursor((_screenWidth / 4) - _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[2].first].length()),
                   (_screenHeight / 2) + 6);
    _tft.println(GaugeLabels[_currentData[2].first]);
    break;
  case 3:
    _tft.fillRect(_screenWidth - (_screenWidth / 4) -
                      _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[3].first].length()) - 1,
                  (_screenHeight / 2) + 6 - 1, kFontWidthMedium * GaugeLabels[_currentData[3].first].length(),
                  kFontHeightMedium, backgroundColor);
    _tft.setCursor(_screenWidth - (_screenWidth / 4) -
                       _getCenterOffset(kFontSizeMedium, GaugeLabels[_currentData[3].first].length()),
                   (_screenHeight / 2) + 6);
    _tft.println(GaugeLabels[_currentData[3].first]);
    break;
  }
}

int DisplayHandler::_getCenterOffset(const FontSize& fontSize, const int& length) const
{
  switch (fontSize)
  {
  case kFontSizeSmall:
    return (length * 6) / 2;
  case kFontSizeMedium:
    return (length * 12) / 2;
  case kFontSizeLarge:
    return (length * 18) / 2;
  default:
    return -1;
  }
}