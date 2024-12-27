#pragma once
#include "RTClib.h"
#include <NTPClient.h>
#include "Config.h"

class TimeManager
{
private:
    RTC_DS3231 &rtc;
    NTPClient &timeClient;

public:
    TimeManager(RTC_DS3231 &_rtc, NTPClient &_client) : rtc(_rtc), timeClient(_client) {}
    bool needsUpdate();
    void updateTime();
    DateTime getCurrentTime();
    String getFormattedTime();
    String getFormattedDate();
};
