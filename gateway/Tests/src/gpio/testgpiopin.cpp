#include <gtest/gtest.h>

#include <memory>

#include "driver/gpio/gpiopin.h"
#include "driver/gpio/gpiopinfactory.h"

using device::driver::gpio::GPIOPin;
using device::driver::gpio::GPIOPinFactory;
using device::driver::gpio::GPIOState;

class TestGPIOPin: public testing::Test
{
};

TEST_F(TestGPIOPin, TestMethods)
{
    std::unique_ptr<GPIOPin> pin(GPIOPinFactory::create());

    ASSERT_TRUE(pin);
    ASSERT_EQ(GPIOState::Low, pin->read());

    pin->write(GPIOState::High);

    ASSERT_EQ(GPIOState::High, pin->read());

    pin->toggle();

    ASSERT_EQ(GPIOState::Low, pin->read());
}
