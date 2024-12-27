#pragma once

// Time constants
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 60

// Battery constants
#define BATPIN A0
#define BATTERY_LEVEL_SAMPLING 4
const float battChangeThreshold = 0.15;
const float battUpperLim = 3.3;
const float battHigh = 3.4;
const float battLow = 2.9;

// WiFi settings
const char *WIFI_SSID = "SonyBraviaX400";
const char *WIFI_PASSWORD = "79756622761";
const char *NTP_SERVER = "asia.pool.ntp.org";
const long NTP_OFFSET = 19800; // India offset

// Display constants
#define COLORED 0
#define UNCOLORED 1
