#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Button2.h>
#include "BH1750.h"
#include "Adafruit_SHT31.h"
#include "Adafruit_BME280.h"

#define POWER_CTRL              (4)

#define I2C1_SDA                (21)
#define I2C1_SCL                (22)
#define I2C_SDA                 (25)
#define I2C_SCL                 (26)

#define OB_BH1750_ADDRESS       (0x23)
#define OB_SHT3X_ADDRESS        (0x44)
#define OB_BME280_ADDRESS       (0x77)

#define SOIL_PIN                (32)
#define BAT_ADC                 (33)
#define SALT_PIN                (34)

#define USER_BUTTON             (35)

class Devices {
public:
    bool has_powerSensor;
    bool has_lightSensor;
    bool has_bmeSensor;
    bool has_sht3xSensor;

    BH1750 lightMeter;     //0x23
    Adafruit_SHT31 sht31;
    Adafruit_BME280 bme;   //0x77

    Button2 button;

    Devices();

    void Probe(TwoWire &t);

    void CheckSensorPower();

    void CheckBH1750();

    void CheckBME280();

    void CheckSHT3X();

private:
    bool init_device;

    static void Sleep(Button2 &b);
};