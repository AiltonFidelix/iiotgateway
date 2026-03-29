#ifndef FAKEGPIOPIN_H
#define FAKEGPIOPIN_H

#include <QString>

#include "gpio_global.h"
#include "gpiopin.h"

GPIO_BEGIN_NAMESPACE

class FakeGPIOPin : public GPIOPin
{
    static bool m_registered;

    uint8_t m_pin;
    GPIOMode m_mode;
    GPIOState m_state;

    void printState(const QString &method);

public:
    FakeGPIOPin();

    void setPin(uint8_t pin) override;
    void setMode(GPIOMode mode) override;
    void write(GPIOState state) override;
    void toggle() override;
    GPIOState read() override;

    static GPIOPin *create();
};

GPIO_END_NAMESPACE

#endif // FAKEGPIOPIN_H
