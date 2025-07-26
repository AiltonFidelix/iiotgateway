#ifndef TESTCOMMMODBUSSETTINGSPARSER_H
#define TESTCOMMMODBUSSETTINGSPARSER_H

#include <gtest/gtest.h>
#include "modbus/commmodbusrequestparser.h"

using commmodbus::Request;

class TestModbusRequestParser : public testing::TestWithParam<std::tuple<QByteArray, Request*>>
{
public:
    static std::vector<std::tuple<QByteArray, Request*>> LoadTestCases();
};

#endif // TESTCOMMMODBUSSETTINGSPARSER_H
