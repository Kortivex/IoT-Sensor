#include <Arduino.h>
#include <algorithm>
#include "configuration.h"
#include "wifi/wifi.h"
#include "device/device.h"
#include "dashboard/dashboard.h"
#include "motor/motor.h"
#include "properties/properties.h"
#include "api/api.h"

Devices device;
Motors motor;

Dashboard dashboard(80);
API api(API_HOST, API_KEY);

uint64_t timestamp = 0;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    // Init Device Checks.
    device.CheckSensorPower();

    Wire.begin(I2C_SDA, I2C_SCL);    // For BH1750 & BME280 Sensors.
    Wire1.begin(I2C1_SDA, I2C1_SCL); // For SHT30 Sensor.

    device.Probe(Wire);
    device.Probe(Wire1);

    device.CheckBH1750();
    device.CheckBME280();
    device.CheckSHT3X();

    // Init Motor.
    motor.Run();

    // Generate BH1750 Cards.
    if (device.has_lightSensor) {
        dashboard.GenerateBH1750Cards();
    }

    // Generate BME280 Cards.
    if (device.has_bmeSensor) {
        dashboard.GenerateBME280Cards();
    }

    // Generate SHT3x Cards.
    if (device.has_sht3xSensor) {
        dashboard.GenerateSHT3xCards();
    }

    // Generate Motor Cards.
    dashboard.GenerateMotorCards();
    dashboard.motorButton->attachCallback([&](bool value) {
        motor.MotorButtonTriggered(value);

        dashboard.motorButton->update(value);
        dashboard.espDash.sendUpdates();
    });

    // Start Wi-Fi connection.
    setupWiFi();

    // Start HTTP Server.
    dashboard.server.begin();
}

void loop() {
    device.button.loop();

    if (millis() - timestamp > PROPERTIES_FREQUENCY) {
        timestamp = millis();

        iot_sensors_event_t val = {0};

        Properties::GetSoilSensorEvent(val);
        dashboard.soilValue->update(val.soil);

        if (AUTO_WATERING) {
            if (val.soil < 26) {
                Serial.println("Start Adding Water...");
                motor.WateringCallback(dashboard, true);
            }
            if (val.soil >= 40) {
                Serial.println("Stop Adding Water...");
                motor.WateringCallback(dashboard, false);
            }
        }

        Properties::GetSaltSensorEvent(val);
        dashboard.saltValue->update(val.salt);

        Properties::GetVoltageSensorEvent(val);
        dashboard.batteryValue->update(val.voltage);

        if (device.has_lightSensor) {
            Properties::GetBHT1750SensorEvent(device, val);
            dashboard.illumination->update(val.light);
        }

        if (device.has_bmeSensor) {
            Properties::GetBME280SensorEvent(device, val);
            dashboard.bmeTemperature->update(val.temperature_in);
            dashboard.bmeHumidity->update(val.humidity_in);
            dashboard.bmeAltitude->update(val.altitude);
            dashboard.bmePressure->update(val.pressure);
        }

        if (device.has_sht3xSensor) {
            Properties::GetSHT3xSensorEvent(device, val);
            dashboard.sht3xTemperature->update(val.temperature_out);
            dashboard.sht3xHumidity->update(val.humidity_out);
        }

        String properties = Properties::GenerateJSONData(val);
        api.PostSensorData(DEVICE_ID, properties);

        String wifi = Properties::GenerateJSONWifiData();
        api.PostSensorConnectionData(DEVICE_ID, wifi);

        dashboard.espDash.sendUpdates();
    }
}
