#ifndef GPIOPINFACTORY_H
#define GPIOPINFACTORY_H

#include <functional>
#include <QByteArray>
#include <QHash>

#include "gpio_global.h"
#include "gpiopin.h"

GPIO_BEGIN_NAMESPACE

class GPIOPinFactory
{
    using Creator = std::function<GPIOPin*()>;
    using GPIOPinList = QHash<QString, Creator>;

    static GPIOPinList *m_gpioPinList;

public:
    GPIOPinFactory() = delete;

    static GPIOPin *getGPIOPin(const QString &platform);
    static int registerGPIOPin(const QString &platform, Creator creator);
};

GPIO_END_NAMESPACE

#endif // GPIOPINFACTORY_H
