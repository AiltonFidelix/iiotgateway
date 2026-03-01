#include "gpiopinfactory.h"

GPIO_BEGIN_NAMESPACE

GPIOPinFactory::Creator GPIOPinFactory::m_creator = nullptr;

GPIOPin *GPIOPinFactory::create()
{
    if (m_creator)
    {
        return m_creator();
    }

    return nullptr;
}

bool GPIOPinFactory::registerCreator(Creator creator)
{
    m_creator = creator;
    return true;
}


GPIO_END_NAMESPACE
