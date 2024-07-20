#include "dashboard.h"

Dashboard::Dashboard(int port) : server(port), espDash(&server) {
    this->saltValue = new Card(&espDash, GENERIC_CARD, DASH_SALT_VALUE_STRING, "%");
    this->batteryValue = new Card(&espDash, GENERIC_CARD, DASH_BATTERY_STRING, "mV");
    this->soilValue = new Card(&espDash, GENERIC_CARD, DASH_SOIL_VALUE_STRING, "%");
}

void Dashboard::GenerateBH1750Cards() {
    this->illumination = new Card(&espDash, GENERIC_CARD, DASH_BH1750_LUX_STRING, "lx");
}

void Dashboard::GenerateBME280Cards() {
    this->bmeTemperature = new Card(&espDash, TEMPERATURE_CARD, DASH_BME280_TEMPERATURE_STRING, "°C");
    this->bmeHumidity = new Card(&espDash, HUMIDITY_CARD, DASH_BME280_HUMIDITY_STRING, "%");
    this->bmeAltitude = new Card(&espDash, GENERIC_CARD, DASH_BME280_ALTITUDE_STRING, "m");
    this->bmePressure = new Card(&espDash, GENERIC_CARD, DASH_BME280_PRESSURE_STRING, "hPa");
}

void Dashboard::GenerateSHT3xCards() {
    this->sht3xTemperature = new Card(&espDash, TEMPERATURE_CARD, DASH_SHT3X_TEMPERATURE_STRING, "°C");
    this->sht3xHumidity = new Card(&espDash, HUMIDITY_CARD, DASH_SHT3X_HUMIDITY_STRING, "%");
}

void Dashboard::GenerateMotorCards() {
    this->motorButton = new Card(&espDash, BUTTON_CARD, DASH_MOTOR_CTRL_STRING);
}