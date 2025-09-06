#ifndef HOSTGPIOPIN_H
#define HOSTGPIOPIN_H

#include <QString>

#include "gpio_global.h"
#include "gpiopin.h"

GPIO_BEGIN_NAMESPACE

class HostGPIOPin : public GPIOPin
{
    static int m_id;

    uint8_t m_pin;
    GPIOMode m_mode;
    GPIOState m_state;

    void printState(const QString &method);

public:
    HostGPIOPin();

    void setPin(uint8_t pin) override;
    void setMode(GPIOMode mode) override;
    void write(GPIOState state) override;
    void toggle() override;
    GPIOState read() override;

    static GPIOPin *create();
};

GPIO_END_NAMESPACE

#endif // HOSTGPIOPIN_H
