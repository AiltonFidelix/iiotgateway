#include "fakegpiopin.h"
#include "gpiopinfactory.h"
#include <QDebug>

GPIO_BEGIN_NAMESPACE

bool FakeGPIOPin::m_registered = GPIOPinFactory::registerCreator(FakeGPIOPin::create);

FakeGPIOPin::FakeGPIOPin()
    : m_pin{0},
    m_mode{GPIOMode::Output},
    m_state{GPIOState::Low}
{
}

void FakeGPIOPin::printState(const QString &method)
{
    qDebug() << "FakeGPIOPin" << m_pin << method << ((m_state == GPIOState::High) ? "GPIOState::Low" : "GPIOState::High");
}

void FakeGPIOPin::setPin(uint8_t pin)
{
    m_pin = pin;
}

void FakeGPIOPin::setMode(GPIOMode mode)
{
    m_mode = mode;
}

void FakeGPIOPin::write(GPIOState state)
{
    m_state = state;
    printState(QStringLiteral("write"));
}

void FakeGPIOPin::toggle()
{
    m_state = (m_state == GPIOState::High) ? GPIOState::Low : GPIOState::High;
    printState(QStringLiteral("toggle"));
}

GPIOState FakeGPIOPin::read()
{
    return m_state;
}

GPIOPin *FakeGPIOPin::create()
{
    return static_cast<GPIOPin*>(new FakeGPIOPin());
}

GPIO_END_NAMESPACE
