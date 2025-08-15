#include "gpiopinfactory.h"

GPIO_BEGIN_NAMESPACE

GPIOPinFactory::GPIOPinList *GPIOPinFactory::m_gpioPinList = nullptr;

GPIOPin *GPIOPinFactory::getGPIOPin(const QString &platform)
{
    if (m_gpioPinList->contains(platform))
    {
        return m_gpioPinList->value(platform)();
    }

    return nullptr;
}

int GPIOPinFactory::registerGPIOPin(const QString &platform, Creator creator)
{
    if (m_gpioPinList == nullptr)
    {
        m_gpioPinList = new GPIOPinList();
    }

    m_gpioPinList->insert(platform, creator);


    return m_gpioPinList->size();
}

GPIO_END_NAMESPACE
