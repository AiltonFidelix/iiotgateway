#ifndef GPIOSTATE_H
#define GPIOSTATE_H

#include <cstdint>

#include "gpio_global.h"

GPIO_BEGIN_NAMESPACE

enum class GPIOState : uint8_t {
    Low = 0,
    High = 1
};

GPIO_END_NAMESPACE

#endif // GPIOSTATE_H
