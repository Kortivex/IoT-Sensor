#include "motor.h"


Motors::Motors() {
    pixels = new Adafruit_NeoPixel(1, RGB_PIN, NEO_GRB + NEO_KHZ800);
}

void Motors::Run() {
    // IO18 is initialized as RGB pixel pin, and IO19 is initialized as motor drive pin.
    Serial.println("Initialize IO18 as RGB pixel pin, and IO19 as motor drive pin.");

    this->InitRGBSequence();

    // IO19 is initialized as motor drive pin.
    pinMode(MOTOR_PIN, OUTPUT);
    digitalWrite(MOTOR_PIN, LOW);
}

void Motors::InitRGBSequence() {
    if (this->pixels != nullptr) {
        // IO18 is initialized as RGB pixel pin.
        this->pixels->begin();
        this->pixels->setBrightness(50);

        this->pixels->setPixelColor(0, this->pixels->RED_COLOR);
        this->pixels->show();

        delay(DELAY_COLOR);

        this->pixels->setPixelColor(0, this->pixels->GREEN_COLOR);
        this->pixels->show();

        delay(DELAY_COLOR);

        this->pixels->setPixelColor(0, this->pixels->BLUE_COLOR);
        this->pixels->show();

        delay(DELAY_COLOR);

        this->pixels->setPixelColor(0, this->pixels->WHITE_COLOR);
        this->pixels->show();
    }
}

void Motors::MotorButtonTriggered(bool value) {
    Serial.println("Motor Button Triggered: " + String((value) ? "true" : "false"));
    digitalWrite(MOTOR_PIN, value);

    if (this->pixels != nullptr) {
        this->pixels->setPixelColor(0, value ? GREEN_COLOR : WHITE_COLOR);
        this->pixels->show();
    }
}

void Motors::WateringCallback(Dashboard &dashboard, bool value) {
    Serial.println("Motor Button Triggered: " + String((value) ? "true" : "false"));
    digitalWrite(MOTOR_PIN, value);

    if (this->pixels != nullptr) {
        this->pixels->setPixelColor(0, value ? GREEN_COLOR : WHITE_COLOR);
        this->pixels->show();
    }
    dashboard.motorButton->update(value);
    dashboard.espDash.sendUpdates();
}
