#include "hostgpiopin.h"
#include "gpiopinfactory.h"
#include <QDebug>

GPIO_BEGIN_NAMESPACE

int HostGPIOPin::m_id = GPIOPinFactory::registerGPIOPin(QStringLiteral("host"), HostGPIOPin::create);

HostGPIOPin::HostGPIOPin()
    : m_pin(0),
    m_mode(GPIOMode::Output),
    m_state(GPIOState::Low)
{
}

void HostGPIOPin::printState(const QString &method)
{
    qDebug() << "HostGPIO" << m_pin << method << ((m_state == GPIOState::High) ? "GPIOState::Low" : "GPIOState::High");
}

bool HostGPIOPin::configure()
{
    return true;
}

void HostGPIOPin::setPin(uint8_t pin)
{
    m_pin = pin;
}

void HostGPIOPin::setMode(GPIOMode mode)
{
    m_mode = mode;
}

void HostGPIOPin::write(GPIOState state)
{
    m_state = state;
    printState(QStringLiteral("write"));
}

void HostGPIOPin::toggle()
{
    m_state = (m_state == GPIOState::High) ? GPIOState::Low : GPIOState::High;
    printState(QStringLiteral("toggle"));
}

GPIOState HostGPIOPin::read()
{
    return m_state;
}

GPIOPin *HostGPIOPin::create()
{
    return static_cast<GPIOPin*>(new HostGPIOPin());
}

GPIO_END_NAMESPACE
