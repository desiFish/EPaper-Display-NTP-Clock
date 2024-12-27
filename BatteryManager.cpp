#include "BatteryManager.h"

float BatteryManager::getBatteryLevel()
{
    uint32_t Vbatt = 0;
    for (int i = 0; i < BATTERY_LEVEL_SAMPLING; i++)
    {
        Vbatt += analogReadMilliVolts(BATPIN);
        delay(10);
    }
    float newBattLevel = 2 * Vbatt / BATTERY_LEVEL_SAMPLING / 1000.0;

    if (newBattLevel < lastBattLevel ||
        (newBattLevel - lastBattLevel) >= battChangeThreshold ||
        newBattLevel > battUpperLim)
    {
        lastBattLevel = newBattLevel;
    }

    return lastBattLevel;
}

byte BatteryManager::getBatteryPercent()
{
    float battLevel = getBatteryLevel();
    byte percent = ((battLevel - battLow) / (battHigh - battLow)) * 100;
    return constrain(percent, 1, 100);
}

String BatteryManager::getBatteryString()
{
    float battLevel = getBatteryLevel();
    return (battLevel >= 4.0) ? "USB" : String(getBatteryPercent()) + "%";
}
