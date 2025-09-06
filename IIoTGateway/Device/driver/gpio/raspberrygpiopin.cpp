#include "raspberrygpiopin.h"
#include "gpiopinfactory.h"

#include <wiringPi.h>

GPIO_BEGIN_NAMESPACE

int RasbpberryGPIOPin::m_id = GPIOPinFactory::registerGPIOPin(QStringLiteral("raspberry"), RasbpberryGPIOPin::create);

RasbpberryGPIOPin::RasbpberryGPIOPin()
    : m_pin(0),
    m_state(GPIOState::Low)
{
}

void RasbpberryGPIOPin::setPin(uint8_t pin)
{
    m_pin = pin;
}

void RasbpberryGPIOPin::setMode(GPIOMode mode)
{
    switch (mode)
    {
        case GPIOMode::Input:
            pinMode(m_pin, INPUT);
            break;
        case GPIOMode::Output:
            pinMode(m_pin, OUTPUT);
            break;
        case GPIOMode::InputPullUp:
            pinMode(m_pin, INPUT);
            pullUpDnControl(m_pin, PUD_UP);
            break;
        case GPIOMode::InputPullDown:
            pinMode(m_pin, INPUT);
            pullUpDnControl(m_pin, PUD_DOWN);
            break;
        case GPIOMode::OutputOpenDrain:
            pinMode(m_pin, OUTPUT);
            break;
        default:
            break;
    }
}

void RasbpberryGPIOPin::write(GPIOState state)
{
    digitalWrite(m_pin, static_cast<int>(state));
    m_state = state;
}

void RasbpberryGPIOPin::toggle()
{
    m_state = (m_state == GPIOState::High) ? GPIOState::Low : GPIOState::High;
    digitalWrite(m_pin, static_cast<int>(m_state));
}

GPIOState RasbpberryGPIOPin::read()
{
    return static_cast<GPIOState>(digitalRead(m_pin));
}

GPIOPin *RasbpberryGPIOPin::create()
{
    wiringPiSetup();
    return static_cast<GPIOPin*>(new RasbpberryGPIOPin());
}

GPIO_END_NAMESPACE
