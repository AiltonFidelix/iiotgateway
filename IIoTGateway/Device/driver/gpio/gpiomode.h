#ifndef GPIOMODE_H
#define GPIOMODE_H

#include <cstdint>

#include "gpio_global.h"

GPIO_BEGIN_NAMESPACE

enum class GPIOMode : uint8_t {
    Input = 0,
    Output,
    InputPullUp,
    InputPullDown,
    OutputOpenDrain
};

GPIO_END_NAMESPACE

#endif // GPIOMODE_H
