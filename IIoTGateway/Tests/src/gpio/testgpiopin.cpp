#include <gtest/gtest.h>

#include "driver/gpio/gpiopin.h"
#include "driver/gpio/gpiopinfactory.h"

using device::driver::gpio::GPIOPin;
using device::driver::gpio::GPIOPinFactory;
using device::driver::gpio::GPIOState;

class TestGPIOPin: public testing::Test
{
};

TEST_F(TestGPIOPin, TestCreation)
{
    GPIOPin *pin = GPIOPinFactory::getGPIOPin(QStringLiteral("host"));

    ASSERT_TRUE(pin != nullptr);

    pin->write(GPIOState::High);

    ASSERT_EQ(GPIOState::High, pin->read());

    pin->toggle();

    ASSERT_EQ(GPIOState::Low, pin->read());

    delete pin;
}
