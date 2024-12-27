#pragma once
#include "src/EPD_3in52.h"
#include "src/epdpaint.h"
#include "Config.h"

class DisplayManager
{
private:
    Epd &epd;
    UBYTE *image;
    void drawBatteryIcon(Paint &paint, byte percent);

public:
    DisplayManager(Epd &_epd, UBYTE *_image) : epd(_epd), image(_image) {}
    void showMessage(const String &msg);
    void showTime(const char *weekDay, const String &time, const String &date,
                  const String &battLevel, const String &percent, byte battPercent,
                  const String &temp);
};
