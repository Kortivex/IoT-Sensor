#include "properties.h"
#include "device/device.h"

Properties::Properties() = default;

bool Properties::GetSoilSensorEvent(iot_sensors_event_t &val) {
    uint16_t soil = analogRead(SOIL_PIN);
    val.soil = map(soil, 0, 4095, 100, 0);

    return true;
}

bool Properties::GetSaltSensorEvent(iot_sensors_event_t &val) {
    uint8_t samples = 120;
    uint32_t humi = 0;
    uint16_t array[120];
    for (int i = 0; i < samples; i++) {
        array[i] = analogRead(SALT_PIN);
        delay(2);
    }
    std::sort(array, array + samples);
    for (int i = 1; i < samples - 1; i++) {
        humi += array[i];
    }
    humi /= samples - 2;
    val.salt = humi;
    return true;
}

bool Properties::GetVoltageSensorEvent(iot_sensors_event_t &val) {
    int vref = 1100;
    uint16_t volt = analogRead(BAT_ADC);
    val.voltage = ((float) volt / 4095.0) * 6.6 * (vref);

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
    val.temperature = device.bme.readTemperature();
    val.humidity = device.bme.readHumidity();
    val.pressure = device.bme.readPressure() / 100.0F;
    val.altitude = device.bme.readAltitude(1013.25F);

    return true;
}

bool Properties::GetSHT3xSensorEvent(Devices &device, iot_sensors_event_t &val) {
    float t = device.sht31.readTemperature();
    float h = device.sht31.readHumidity();

    Serial.println("Sensor SHT3x: ");
    if (!isnan(t)) {  // check if 'is not a number'
        Serial.print("Temp *C = ");
        Serial.print(t);
        Serial.print("\t\t");
    } else {
        Serial.println("Failed to read temperature");
    }
    if (!isnan(h)) {  // check if 'is not a number'
        Serial.print("Hum. % = ");
        Serial.println(h);
    } else {
        Serial.println("Failed to read humidity");
    }

    val.temperature = t;
    val.humidity = h;

    return true;
}
