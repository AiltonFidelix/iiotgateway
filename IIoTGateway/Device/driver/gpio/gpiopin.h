#ifndef GPIOPIN_H
#define GPIOPIN_H

#include "gpio_global.h"
#include "gpiomode.h"
#include "gpiostate.h"

GPIO_BEGIN_NAMESPACE

class GPIOPin
{
public:
    virtual ~GPIOPin() = default;

    virtual bool configure() = 0;

    virtual void setPin(uint8_t pin) = 0;
    virtual void setMode(GPIOMode mode) = 0;
    virtual void write(GPIOState state) = 0;
    virtual void toggle() = 0;

    virtual GPIOState read() = 0;
};

GPIO_END_NAMESPACE

#endif // COMMINTERFACE_H
