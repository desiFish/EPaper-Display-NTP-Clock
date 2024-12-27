#pragma once
#include "Config.h"
#include <Arduino.h>

class BatteryManager
{
private:
    float lastBattLevel;

public:
    BatteryManager() : lastBattLevel(3.5) {}
    float getBatteryLevel();
    byte getBatteryPercent();
    String getBatteryString();
    void updateBatteryLevel(float newLevel);
};
