#include <SPI.h>
#include "src/EPD_3in52.h"
#include "src/imagedata.h"
#include "src/epdpaint.h"

#include <Wire.h>
#include "RTClib.h"

#include <WiFi.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <Preferences.h>
#include <BH1750.h>

// powersave wifi off
#include <esp_wifi.h>

#include "Config.h"
#include "DisplayManager.h"
#include "TimeManager.h"
#include "BatteryManager.h"

Preferences pref; // preference library object

RTC_DS3231 rtc;          // ds3231 object
BH1750 lightMeter(0x23); // Initalize light sensor
int nightFlag = 0;       // remembers last state of clock i.e. sleeping or not

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
const int TIME_TO_SLEEP = 60;  // in seconds

#define BATPIN A0                // battery voltage divider connection pin (1M Ohm with 104 Capacitor)
#define BATTERY_LEVEL_SAMPLING 4 // number of times to average the reading

// battery related settings
const float battChangeThreshold = 0.15;
const float battUpperLim = 3.3;
const float battHigh = 3.4;
const float battLow = 2.9;

const char *ssid = "SonyBraviaX400";
const char *password = "79756622761";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 19800); // 19800 is offset of India, asia.pool.ntp.org is close to India
char daysOfTheWeek[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define COLORED 0
#define UNCOLORED 1

UBYTE image[68000];
Epd epd;

bool errFlag = false;
DisplayManager *display;
TimeManager *timeManager;
BatteryManager batteryManager;

void enableWiFi()
{
  WiFi.disconnect(false); // Reconnect the network
  WiFi.mode(WIFI_STA);    // Switch WiFi off

  Serial1.println("START WIFI");
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed");
    break;
  }
  Serial1.println("IP address: ");
  Serial1.println(WiFi.localIP());
}

void disableWiFi()
{
  WiFi.disconnect(true); // Disconnect from the network
  WiFi.mode(WIFI_OFF);   // Switch WiFi off
}

bool initializeSystem()
{
  pinMode(BATPIN, INPUT);
  disableWiFi();

  if (!rtc.begin() || !lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE))
  {
    return false;
  }

  pref.begin("database", false);
  if (!pref.isKey("nightFlag"))
  {
    pref.putBool("nightFlag", false);
  }

  return true;
}

float getLightLevel()
{
  while (!lightMeter.measurementReady(true))
  {
    yield();
  }
  return lightMeter.readLightLevel();
}

void handleNightMode(float lux)
{
  bool tempNightFlag = nightFlag;

  if (lux == 0 && !nightFlag)
  {
    nightFlag = 1;
    display->showMessage("SLEEPING");
  }
  else
  {
    nightFlag = 0;
  }

  if (tempNightFlag != nightFlag)
  {
    pref.putBool("nightFlag", nightFlag);
  }
}

void updateDisplayAndTime()
{
  if (timeManager->needsUpdate())
  {
    enableWiFi();
    timeManager->updateTime();
    disableWiFi();
  }

  DateTime now = timeManager->getCurrentTime();
  float battLevel = batteryManager.getBatteryLevel();

  display->showTime(
      daysOfTheWeek[now.dayOfTheWeek()],
      timeManager->getFormattedTime(),
      timeManager->getFormattedDate(),
      String(battLevel) + "V",
      batteryManager.getBatteryString(),
      batteryManager.getBatteryPercent(),
      String(int(rtc.getTemperature())) + "C");
}

void prepareSleep()
{
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.flush();
  delay(50);
  esp_deep_sleep_start();
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // Set clock speed to be the fastest for better communication (fast mode)

  display = new DisplayManager(epd, image);
  timeManager = new TimeManager(rtc, timeClient);

  // Initialize system
  if (!initializeSystem())
  {
    display->showMessage("Init Error");
    return;
  }

  // Main logic
  float lux = getLightLevel();
  handleNightMode(lux);

  if (lux > 0)
  {
    updateDisplayAndTime();
  }

  // Sleep
  prepareSleep();
}

void loop()
{
  // This will never run
}
