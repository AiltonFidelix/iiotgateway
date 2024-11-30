#ifndef TESTCOMMMODBUS_H
#define TESTCOMMMODBUS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "modbus/commmodbusrtu.h"
#include "mockmodbusclient.h"

class TestCommModbus: public testing::Test
{
protected:
    MockModbusClient *m_mockModbusClient;
    CommModbusRTU m_commModbus;

    void SetUp() override;
};

#endif // TESTCOMMMODBUS_H
