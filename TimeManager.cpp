#include "TimeManager.h"

bool TimeManager::needsUpdate()
{
    DateTime now = rtc.now();
    return (now.year() == 2070) || rtc.lostPower();
}

void TimeManager::updateTime()
{
    if (timeClient.update())
    {
        time_t rawtime = timeClient.getEpochTime();
        struct tm *ti = localtime(&rawtime);

        rtc.adjust(DateTime(
            ti->tm_year + 1900,
            ti->tm_mon + 1,
            ti->tm_mday,
            timeClient.getHours(),
            timeClient.getMinutes(),
            timeClient.getSeconds()));
    }
}

DateTime TimeManager::getCurrentTime()
{
    return rtc.now();
}

String TimeManager::getFormattedTime()
{
    DateTime now = getCurrentTime();
    int hour = now.hour();
    bool isPM = hour >= 12;

    if (hour > 12)
        hour -= 12;
    else if (hour == 0)
        hour = 12;

    String hourStr = hour < 10 ? "0" + String(hour) : String(hour);
    String minStr = now.minute() < 10 ? "0" + String(now.minute()) : String(now.minute());

    return hourStr + ":" + minStr + (isPM ? " PM" : " AM");
}

String TimeManager::getFormattedDate()
{
    DateTime now = getCurrentTime();
    String dayStr = now.day() < 10 ? "0" + String(now.day()) : String(now.day());
    String monthStr = now.month() < 10 ? "0" + String(now.month()) : String(now.month());
    return dayStr + "/" + monthStr + "/" + String(now.year());
}
