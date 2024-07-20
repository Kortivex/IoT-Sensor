#pragma once


#include "device/device.h"

typedef struct {
    uint32_t timestamp;     /** time is in milliseconds */
    float temperature;      /** temperature is in degrees centigrade (Celsius) */
    float light;            /** light in SI lux units */
    float pressure;         /** pressure in hectopascal (hPa) */
    float humidity;         /** humidity in percent */
    float altitude;         /** altitude in m */
    float voltage;          /** voltage in volts (V) */
    uint8_t soil;           /** Percentage of soil */
    uint8_t salt;           /** Percentage of salt */
} iot_sensors_event_t;

class Properties {
public:
    Properties();

    static bool GetSoilSensorEvent(iot_sensors_event_t &val);

    static bool GetSaltSensorEvent(iot_sensors_event_t &val);

    static bool GetVoltageSensorEvent(iot_sensors_event_t &val);

    static bool GetBHT1750SensorEvent(Devices &device, iot_sensors_event_t &val);

    static bool GetBME280SensorEvent(Devices &device, iot_sensors_event_t &val);

    static bool GetSHT3xSensorEvent(Devices &device, iot_sensors_event_t &val);
};