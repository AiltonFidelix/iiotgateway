#ifndef RASPBERRYGPIOPIN_H
#define RASPBERRYGPIOPIN_H

#include <QString>

#include "gpio_global.h"
#include "gpiopin.h"

GPIO_BEGIN_NAMESPACE

class RasbpberryGPIOPin : public GPIOPin
{
    static bool m_registered;

    uint8_t m_pin;
    GPIOState m_state;

    static bool registerGPIO();

public:
    RasbpberryGPIOPin();
    ~RasbpberryGPIOPin();

    void setPin(uint8_t pin) override;
    void setMode(GPIOMode mode) override;
    void write(GPIOState state) override;
    void toggle() override;
    GPIOState read() override;

    static GPIOPin *create();
};

GPIO_END_NAMESPACE

#endif // RASPBERRYGPIOPIN_H
