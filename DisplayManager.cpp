#include "DisplayManager.h"

void DisplayManager::initDisplay()
{
    if (epd.Init() != 0)
    {
        Serial.println("e-Paper init failed");
        return;
    }
    epd.Clear();
}

void DisplayManager::showMessage(const String &msg)
{
    epd.display_NUM(EPD_3IN52_WHITE);
    epd.lut_GC();
    epd.refresh();
    epd.SendCommand(0x50);
    epd.SendData(0x17);
    delay(100);

    Paint paint(image, 240, 360);
    paint.SetRotate(3);
    paint.Clear(COLORED);
    paint.DrawStringAt(10, 100, msg.c_str(), &Font48, UNCOLORED);

    epd.display_part(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
    epd.lut_GC();
    epd.refresh();
    epd.sleep();
}

void DisplayManager::showTime(const char *weekDay, const String &timeStr, const String &dateStr,
                              const String &battLevel, const String &percentStr, byte percent,
                              const String &temp)
{
    epd.display_NUM(EPD_3IN52_WHITE);
    epd.lut_GC();
    epd.refresh();
    epd.SendCommand(0x50);
    epd.SendData(0x17);
    delay(100);

    Paint paint(image, 240, 360);
    paint.SetRotate(3);
    paint.Clear(UNCOLORED);

    // Draw battery icon
    paint.DrawRectangle(10, 4, 26, 12, COLORED);
    paint.DrawRectangle(8, 6, 10, 10, COLORED);

    int fillWidth = map(percent, 0, 100, 25, 11);
    paint.DrawFilledRectangle(fillWidth, 4, 25, 11, COLORED);

    // Draw text elements
    paint.DrawStringAt(325, 5, percentStr.c_str(), &Font12, COLORED);
    paint.DrawStringAt(280, 5, battLevel.c_str(), &Font12, COLORED);
    paint.DrawStringAt(60, 30, weekDay, &Font48, COLORED);
    paint.DrawStringAt(295, 50, temp.c_str(), &Font24, COLORED);
    paint.DrawStringAt(327, 47, "o", &Font8, COLORED);
    paint.DrawStringAt(60, 100, timeStr.c_str(), &Font48, COLORED);
    paint.DrawStringAt(60, 170, dateStr.c_str(), &Font48, COLORED);

    epd.display_part(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
    epd.lut_GC();
    epd.refresh();
    epd.sleep();
}
