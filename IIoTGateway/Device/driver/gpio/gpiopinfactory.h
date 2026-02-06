#ifndef GPIOPINFACTORY_H
#define GPIOPINFACTORY_H

#include <functional>

#include "gpio_global.h"
#include "gpiopin.h"

GPIO_BEGIN_NAMESPACE

class GPIOPinFactory
{
    using Creator = std::function<GPIOPin*()>;
    static Creator m_creator;

public:
    GPIOPinFactory() = delete;

    static GPIOPin *create();
    static bool registerCreator(Creator creator);
};

GPIO_END_NAMESPACE

#endif // GPIOPINFACTORY_H
