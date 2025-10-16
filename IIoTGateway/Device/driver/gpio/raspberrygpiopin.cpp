#include "raspberrygpiopin.h"
#include "gpiopinfactory.h"

#include <wiringPi.h>

#include <QDebug>

GPIO_BEGIN_NAMESPACE

int RasbpberryGPIOPin::m_id = RasbpberryGPIOPin::registerGPIO();

int RasbpberryGPIOPin::registerGPIO()
{
    qDebug() << "Setting up wiring pi...";
    wiringPiSetupGpio();
    return GPIOPinFactory::registerGPIOPin(QStringLiteral("raspberry"), RasbpberryGPIOPin::create);
}

RasbpberryGPIOPin::RasbpberryGPIOPin()
    : m_pin(0),
      m_state(GPIOState::Low)
{
    qDebug() << "Creating GPIO...";
}

RasbpberryGPIOPin::~RasbpberryGPIOPin()
{
    write(GPIOState::Low);
}

void RasbpberryGPIOPin::setPin(uint8_t pin)
{
    qDebug() << "Setting GPIO pin:" << pin;
    m_pin = pin;
}

void RasbpberryGPIOPin::setMode(GPIOMode mode)
{
    switch (mode)
    {
        case GPIOMode::Input:
            pinMode(m_pin, INPUT);
            qDebug() << "Setting GPIO mode: INPUT";
            break;
        case GPIOMode::Output:
            pinMode(m_pin, OUTPUT);
            qDebug() << "Setting GPIO mode: OUTPUT";
            break;
        case GPIOMode::InputPullUp:
            pinMode(m_pin, INPUT);
            pullUpDnControl(m_pin, PUD_UP);
            qDebug() << "Setting GPIO mode: INPUT PUD_UP";
            break;
        case GPIOMode::InputPullDown:
            pinMode(m_pin, INPUT);
            pullUpDnControl(m_pin, PUD_DOWN);
            qDebug() << "Setting GPIO mode: INPUT PUD_DOWN";
            break;
        case GPIOMode::OutputOpenDrain:
            pinMode(m_pin, OUTPUT);
            qDebug() << "Setting GPIO mode: OUTPUT";
            break;
        default:
            break;
    }
}

void RasbpberryGPIOPin::write(GPIOState state)
{
    digitalWrite(m_pin, static_cast<int>(state));
    m_state = state;
    qDebug() << "GPIOPin:" << m_pin << ((m_state == GPIOState::High) ? "GPIOState::Low" : "GPIOState::High");
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
