#include <WiFi.h>
#include "properties.h"

float Properties::previousVoltage = 0.0f;
float Properties::previousBatteryPercentage = 0.0f;
constexpr float Properties::alpha;

Properties::Properties() = default;

bool Properties::GetSoilSensorEvent(iot_sensors_event_t &val) {
    uint16_t soil = analogRead(SOIL_PIN);
    val.soil = map(soil, 0, 4095, 100, 0);

    return true;
}

bool Properties::GetSaltSensorEvent(iot_sensors_event_t &val) {
    uint8_t samples = 120;
    uint32_t humidity = 0;
    uint16_t array[120];
    for (int i = 0; i < samples; i++) {
        array[i] = analogRead(SALT_PIN);
        delay(2);
    }
    std::sort(array, array + samples);
    for (int i = 1; i < samples - 1; i++) {
        humidity += array[i];
    }
    humidity /= samples - 2;
    val.salt = humidity;
    return true;
}

bool Properties::GetVoltageSensorEvent(iot_sensors_event_t &val) {
    int vRef = 1100;
    uint16_t volt = analogRead(BAT_ADC);
    auto currentVoltage = static_cast<float>((static_cast<double>(volt) / 4095.0) * 6.6 * vRef);

    val.voltage = alpha * currentVoltage + (1.0f - alpha) * previousVoltage;
    previousVoltage = val.voltage;

    return true;
}

bool Properties::GetBatteryLevelEvent(iot_sensors_event_t &val) {
    float rawBatteryPercentage  = (val.voltage * 100.0f) / MAX_VOLTAGE;
    if (rawBatteryPercentage  < 0.0f) rawBatteryPercentage  = 0.0f;
    if (rawBatteryPercentage  > 100.0f) rawBatteryPercentage  = 100.0f;

    val.batteryPercentage = alpha * rawBatteryPercentage + (1.0f - alpha) * previousBatteryPercentage;
    previousBatteryPercentage = val.batteryPercentage;

    return true;
}

bool Properties::GetBHT1750SensorEvent(Devices &device, iot_sensors_event_t &val) {
    val.light = device.lightMeter.readLightLevel();
    if (isnan(val.light)) {
        val.light = 0.0;
    }

    return true;
}

bool Properties::GetBME280SensorEvent(Devices &device, iot_sensors_event_t &val) {
    val.temperature_in = device.bme.readTemperature();
    val.humidity_in = device.bme.readHumidity();
    val.pressure = device.bme.readPressure() / 100.0F;
    val.altitude = device.bme.readAltitude(1013.25F);

    return true;
}

bool Properties::GetSHT3xSensorEvent(Devices &device, iot_sensors_event_t &val) {
    float t = device.sht31.readTemperature();
    float h = device.sht31.readHumidity();

    Serial.println("\nSensor SHT3x: ");
    if (!isnan(t)) {  // check if 'is not a number'.
        Serial.print("Temp *C = ");
        Serial.print(t);
        Serial.print("\t\t");
    } else {
        Serial.println("Failed to read temperature");
    }
    if (!isnan(h)) {  // check if 'is not a number'.
        Serial.print("Hum. % = ");
        Serial.println(h);
    } else {
        Serial.println("Failed to read humidity");
    }

    val.temperature_out = t;
    val.humidity_out = h;

    return true;
}

String Properties::GenerateJSONData(iot_sensors_event_t &val) {
    JsonDocument doc;
    doc["voltage"] = val.voltage;
    doc["battery"] = val.batteryPercentage;
    doc["soil"] = val.soil;
    doc["salt"] = val.salt;
    doc["light"] = val.light;
    doc["temperature_in"] = val.temperature_in;
    doc["humidity_in"] = val.humidity_in;
    doc["temperature_out"] = val.temperature_out;
    doc["humidity_out"] = val.humidity_out;
    doc["pressure"] = val.pressure;
    doc["altitude"] = val.altitude;

    String data;
    serializeJson(doc, data);

    return data;
}

String Properties::GenerateJSONMetaData() {
    JsonDocument doc;
    doc["manufacturer"] = MANUFACTURER;
    doc["firmware_version"] = FIRMWARE_VERSION;
    doc["model_number"] = MODEL_NUMBER;

    String data;
    serializeJson(doc, data);

    return data;
}

String Properties::GenerateJSONWifiData() {
    JsonDocument doc;
    doc["ssid"] = WiFi.SSID();
    doc["channel"] = WiFi.channel();
    doc["dns"] = WiFi.dnsIP().toString();
    doc["ip"] = WiFi.localIP();
    doc["subnet"] = WiFi.subnetMask().toString();
    doc["mac"] = WiFi.macAddress();

    String data;
    serializeJson(doc, data);

    return data;
}
