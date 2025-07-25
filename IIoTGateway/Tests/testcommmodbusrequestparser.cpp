#include "testcommmodbusrequestparser.h"
#include "testutils.h"

using commmodbus::CommModbusRequestParser;
using commmodbus::RequestType;
using commmodbus::Units;

TEST_P(TestModbusRequestParser, TestRequestParser)
{
    auto testUnit = [] (const QModbusDataUnit &expected, const QModbusDataUnit &actual) -> bool
    {
        bool result = (expected.registerType() == actual.registerType());
        result &= (expected.startAddress() == actual.startAddress());

        const auto expectedValues = expected.values();
        const auto actualValues = actual.values();

        result &= (expectedValues.count() == actualValues.count());

        result &= std::equal(actualValues.cbegin(), actualValues.cend(), expectedValues.cbegin(), [](const int a, const int b) {
            return (a == b);
        });

        return result;
    };

    const auto param = GetParam();

    const QByteArray filePath = std::get<0>(param);
    Request *expectedRequest = std::get<1>(param);

    ASSERT_TRUE(expectedRequest != nullptr);

    const QByteArray data = TestUtils::readJsonFile(filePath);

    CommModbusRequestParser parser(data);

    const Request actualRequest = parser.request();

    ASSERT_EQ(expectedRequest->count(), actualRequest.count());

    const auto keys = actualRequest.keys();

    for (const quint8 &key : keys)
    {
        ASSERT_TRUE(expectedRequest->contains(key));

        const Units expectedUnits = expectedRequest->value(key);
        const Units actualUnits = actualRequest.value(key);

        ASSERT_EQ(expectedUnits.count(), actualUnits.count());

        const bool result = std::equal(expectedUnits.cbegin(), expectedUnits.cend(), actualUnits.cbegin(), testUnit);

        ASSERT_TRUE(result) << "Units are not equal";
    }

    delete expectedRequest;
}

std::vector<std::tuple<QByteArray, Request*>> TestModbusRequestParser::LoadTestCases()
{
    std::vector<std::tuple<QByteArray, Request*>> cases{};

    {
        // Test empty
        auto request = new Request();
        cases.push_back(std::make_tuple("", request));
    }

    {
        // Test read one
        auto request = new Request();

        request->insert(240, Units({QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, 10)}));

        cases.push_back(std::make_tuple(":/requests/readone.json", request));
    }

    {
        // Test read multiple
        auto request = new Request();

        request->insert(1, Units({QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, 10),
                                  QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 10, 10)}));

        request->insert(2, Units({QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, 10),
                                  QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 10, 10),
                                  QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 20, 10),
                                  QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 30, 10)}));

        cases.push_back(std::make_tuple(":/requests/readmultiple.json", request));
    }

    {
        // Test write one
        auto request = new Request();

        request->insert(55, Units({QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 3, { 88, 2, 65533, 4, 5, 0, 7, 8, 255, 10 })}));

        cases.push_back(std::make_tuple(":/requests/writeone.json", request));
    }

    {
        // Test write multiple
        auto request = new Request();

        request->insert(55, Units({QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 3, { 88, 2, 65533, 4, 5, 0, 7, 8, 255, 10 })}));

        request->insert(60, Units({QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, { 88, 2, 65533, 4, 55, 0, 7, 8, 255, 1080 }),
                                   QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 10, { 68, 72, 780, 2556, 101, 99, 0, 23 })}));

        cases.push_back(std::make_tuple(":/requests/writemultiple.json", request));
    }

    return cases;
}

INSTANTIATE_TEST_SUITE_P(TestRequestParser, TestModbusRequestParser, ::testing::ValuesIn(TestModbusRequestParser::LoadTestCases()));
