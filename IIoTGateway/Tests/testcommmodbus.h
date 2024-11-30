#ifndef TESTCOMMMODBUS_H
#define TESTCOMMMODBUS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "modbus/commmodbusrtu.h"
#include "mockmodbusclient.h"

// Test Fixture
class CommModbusTest : public testing::Test
{
protected:
    MockModbusClient *mockModbusClient;
    CommModbusRTU commModbus;

    void SetUp() override;
};

#endif // TESTCOMMMODBUS_H
