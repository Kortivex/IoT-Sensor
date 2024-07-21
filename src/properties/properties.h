#pragma once

#include <ArduinoJson.h>
#include "device/device.h"

typedef struct {
    uint32_t timestamp;         /** time is in milliseconds */
    float temperature_in;       /** temperature is in degrees centigrade (Celsius) */
    float temperature_out;      /** temperature is in degrees centigrade (Celsius) */
    float humidity_in;          /** humidity in percent */
    float humidity_out;         /** humidity in percent */
    float light;                /** light in SI lux units */
    float pressure;             /** pressure in hectopascal (hPa) */
    float altitude;             /** altitude in m */
    float voltage;              /** voltage in volts (V) */
    float batteryPercentage;    /** Percentage of battery load level */
    uint8_t soil;               /** Percentage of soil */
    uint8_t salt;               /** Percentage of salt */
} iot_sensors_event_t;

class Properties {
public:
    Properties();

    static bool GetSoilSensorEvent(iot_sensors_event_t &val);

    static bool GetSaltSensorEvent(iot_sensors_event_t &val);

    static bool GetVoltageSensorEvent(iot_sensors_event_t &val);

    static bool GetBatteryLevelEvent(iot_sensors_event_t &val);

    static bool GetBHT1750SensorEvent(Devices &device, iot_sensors_event_t &val);

    static bool GetBME280SensorEvent(Devices &device, iot_sensors_event_t &val);

    static bool GetSHT3xSensorEvent(Devices &device, iot_sensors_event_t &val);

    static String GenerateJSONData(iot_sensors_event_t &val);

    static String GenerateJSONMetaData();

    static String GenerateJSONWifiData();

private:
    static float previousVoltage;
    static float previousBatteryPercentage;
    constexpr static const float alpha = 0.1f;
};