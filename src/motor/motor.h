#include <Adafruit_NeoPixel.h>
#include "dashboard/dashboard.h"

#define MOTOR_PIN               (19)
#define RGB_PIN                 (18)

#define RED_COLOR                Adafruit_NeoPixel::Color(255, 0, 0)
#define GREEN_COLOR              Adafruit_NeoPixel::Color(0, 255, 0)
#define BLUE_COLOR               Adafruit_NeoPixel::Color(0, 0, 255)
#define WHITE_COLOR              Adafruit_NeoPixel::Color(0, 0, 0)

#define DELAY_COLOR              (1000)

class Motors {
public:
    Adafruit_NeoPixel *pixels = nullptr;

    Motors();

    void Run() const;

    void InitRGBSequence() const;

    void MotorButtonTriggered(bool value) const;

    void WateringCallback(Dashboard &dashboard, bool value) const;
};