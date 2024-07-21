#include "device.h"

Devices::Devices() : lightMeter(OB_BH1750_ADDRESS), sht31(&Wire1), bme() {
    this->init_device = true;

    this->has_powerSensor = false;
    this->has_lightSensor = false;
    this->has_bmeSensor = false;
    this->has_sht3xSensor = false;

    // Use the button to enter deep sleep (GPIO35).
    this->button = Button2(USER_BUTTON);
    this->button.setLongClickHandler(Sleep);
}

void Devices::Probe(TwoWire &t) {
    if (this->init_device) {
        uint8_t err, addr;
        int nDevices = 0;
        for (addr = 1; addr < 127; addr++) {
            t.beginTransmission(addr);
            err = t.endTransmission();
            if (err == 0) {
                switch (addr) {
                    case OB_BH1750_ADDRESS:
                        this->has_lightSensor = true;
                        Serial.println("BH1750 - Light Sensor found!");
                        break;
                    case OB_BME280_ADDRESS:
                        this->has_bmeSensor = true;
                        Serial.println("BME280 Temperature and Humidity Sensor found!");
                        break;
                    case OB_SHT3X_ADDRESS:
                        this->has_sht3xSensor = true;
                        Serial.println("SHT3X Temperature and Humidity Sensor found!");
                        break;
                    default:
                        Serial.print("I2C device found at address 0x!");
                        if (addr < 16)
                            Serial.print("0");
                        Serial.print(addr, HEX);
                        Serial.println(" !");
                        break;
                }
                nDevices++;
            } else if (err == 4) {
                Serial.print("Unknown error at address 0x!!");
                if (addr < 16)
                    Serial.print("0");
                Serial.println(addr, HEX);
            }
        }
    }
}

/* *
   * Warning:
   *   The Sensor Power Control Pin, use external port and onboard sensor, IO4 must be set high.
* */
void Devices::CheckSensorPower() {
    if (this->init_device) {

        pinMode(POWER_CTRL, OUTPUT);
        digitalWrite(POWER_CTRL, HIGH);

        delay(100);

        this->has_powerSensor = true;
        Serial.println("Power Sensor init succeeded!");
    }
}

void Devices::CheckBH1750() {
    if (this->init_device) {
        if (this->has_lightSensor) {
            if (!this->lightMeter.begin()) {
                this->has_lightSensor = false;
                Serial.println("Warning: Failed to find BH1750 light sensor!!");
            } else {
                Serial.println("BH1750 light sensor init succeeded, using BH1750!");
            }
        }
    }
}

void Devices::CheckBME280() {
    if (this->init_device) {
        if (this->has_bmeSensor) {
            if (!this->bme.begin()) {
                Serial.println("Warning: Failed to find BMP280 temperature and humidity sensor!!");
            } else {
                Serial.println("BMP280 temperature and humidity sensor init succeeded, using BMP280!");
                this->has_bmeSensor = true;
            }
        }
    }
}

void Devices::CheckSHT3X() {
    if (this->init_device) {
        if (this->has_sht3xSensor) {
            if (!this->sht31.begin(OB_SHT3X_ADDRESS)) {   // Set to 0x45 for alternate i2c addr
                Serial.println("Warning: Failed to find SHT3x temperature and humidity sensor!!");
            } else {
                this->has_sht3xSensor = true;
                Serial.println("SHT3X Temperature and Humidity Sensor init succeeded, using SHT3X!");
            }
        }
    }
}

void Devices::Sleep(Button2 &b) {
    Serial.println("Enter in Deep Sleep Mode...");

    esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
    delay(1000);
    esp_deep_sleep_start();
}
