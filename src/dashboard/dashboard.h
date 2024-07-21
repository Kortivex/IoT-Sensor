#pragma once

#include <ESPDash.h>

#define DASH_BME280_TEMPERATURE_STRING  "BME Temperature"
#define DASH_BME280_PRESSURE_STRING     "BME Pressure"
#define DASH_BME280_ALTITUDE_STRING     "BME Altitude"
#define DASH_BME280_HUMIDITY_STRING     "BME Humidity"
#define DASH_BH1750_LUX_STRING          "BH1750"
#define DASH_SOIL_VALUE_STRING          "Soil Value"
#define DASH_SALT_VALUE_STRING          "Salt Value"
#define DASH_BATTERY_STRING             "Battery"
#define DASH_BATTERY_LEVEL_STRING       "Battery Level (EMA Filter)"
#define DASH_SHT3X_TEMPERATURE_STRING   "SHT3X Temperature"
#define DASH_SHT3X_HUMIDITY_STRING      "SHT3X Humidity"
#define DASH_MOTOR_CTRL_STRING          "Water pump"

class Dashboard {
public:
    AsyncWebServer server;
    ESPDash espDash;

    Card *saltValue = nullptr;
    Card *batteryValue = nullptr;
    Card *batteryPercent = nullptr;
    Card *soilValue = nullptr;

    Card *illumination = nullptr;
    Card *bmeTemperature = nullptr;
    Card *bmeHumidity = nullptr;
    Card *bmeAltitude = nullptr;
    Card *bmePressure = nullptr;
    Card *sht3xTemperature = nullptr;
    Card *sht3xHumidity = nullptr;

    Card *motorButton = nullptr;

    explicit Dashboard(int port = 80);

    void GenerateBH1750Cards();

    void GenerateBME280Cards();

    void GenerateSHT3xCards();

    void GenerateMotorCards();
};