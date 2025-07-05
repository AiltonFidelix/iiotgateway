#ifndef TESTCOMMMODBUS_H
#define TESTCOMMMODBUS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "modbus/commmodbusrtu.h"
#include "mockmodbusclient.h"

using namespace commmodbus;

class TestCommModbus: public testing::Test
{
protected:
    MockModbusClient *m_mockModbusClient;
    CommModbusRTU *m_commModbus;

    void SetUp() override;
    void TearDown() override;
};

#endif // TESTCOMMMODBUS_H
