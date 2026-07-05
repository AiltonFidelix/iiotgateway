#include "modbus/commmodbussettingsparser.hpp"
#include "testutils.hpp"

#include <gtest/gtest.h>

#include <QJsonArray>
#include <QJsonDocument>

#include <memory>

using testing::ValuesIn;
using testing::TestWithParam;

using comm::commmodbus::CommModbusSettingsParser;
using comm::commmodbus::Request;
using comm::commmodbus::Units;

using TestCases = std::tuple<QString, CommModbusSettingsParser*>;

class TestCommModbusSettingsParser : public TestWithParam<TestCases>
{
public:
    static std::vector<TestCases> LoadTestCases();
};

TEST_P(TestCommModbusSettingsParser, TestSettingsParser)
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

    const auto filePath = std::get<QString>(param);
    std::unique_ptr<CommModbusSettingsParser> expectedParser{std::get<CommModbusSettingsParser *>(param)};

    ASSERT_TRUE(expectedParser != nullptr);

    const QByteArray data = TestUtils::readJsonFile(filePath);
    const QJsonObject settings = QJsonDocument::fromJson(data).object();

    CommModbusSettingsParser actualParser(settings);

    ASSERT_EQ(expectedParser->host(), actualParser.host());
    ASSERT_EQ(expectedParser->port(), actualParser.port());
    ASSERT_EQ(expectedParser->baudrate(), actualParser.baudrate());
    ASSERT_EQ(expectedParser->parity(), actualParser.parity());

    ASSERT_EQ(expectedParser->databits(), actualParser.databits());
    ASSERT_EQ(expectedParser->stopbits(), actualParser.stopbits());
    ASSERT_EQ(expectedParser->retries(), actualParser.retries());
    ASSERT_EQ(expectedParser->timeout(), actualParser.timeout());
    ASSERT_EQ(expectedParser->pollingInterval(), actualParser.pollingInterval());

    ASSERT_EQ(expectedParser->pollingEnabled(), actualParser.pollingEnabled());

    const Request expectedRequest = expectedParser->requests();
    const Request actualRequest = actualParser.requests();

    ASSERT_EQ(expectedRequest.count(), actualRequest.count());

    const auto keys = actualRequest.keys();

    for (const quint8 &key : keys)
    {
        ASSERT_TRUE(expectedRequest.contains(key));

        const Units expectedUnits = expectedRequest.value(key);
        const Units actualUnits = actualRequest.value(key);

        ASSERT_EQ(expectedUnits.count(), actualUnits.count());

        const bool result = std::equal(expectedUnits.cbegin(), expectedUnits.cend(), actualUnits.cbegin(), testUnit);

        ASSERT_TRUE(result) << "Units are not equal";
    }
}

std::vector<TestCases> TestCommModbusSettingsParser::LoadTestCases()
{
    std::vector<TestCases> testCases{};

    {
        // Test without settings
        auto parser = new CommModbusSettingsParser();
        testCases.push_back(std::make_tuple("", parser));
    }

    {
        // Test modbus RTU settings
        QJsonObject settings{};

        settings.insert(QStringLiteral("port"), QStringLiteral("default"));
        settings.insert(QStringLiteral("baudrate"), QStringLiteral("115200"));
        settings.insert(QStringLiteral("parity"), QStringLiteral("None"));
        settings.insert(QStringLiteral("dataBits"), 8);
        settings.insert(QStringLiteral("stopBits"), 1);
        settings.insert(QStringLiteral("retries"), 10);
        settings.insert(QStringLiteral("timeout"), 2000);
        settings.insert(QStringLiteral("pollingInterval"), 5000);
        settings.insert(QStringLiteral("pollingEnabled"), true);

        QJsonObject request{};

        request.insert(QStringLiteral("address"), 240);
        request.insert(QStringLiteral("startRegister"), 0);
        request.insert(QStringLiteral("numberOfEntries"), 10);
        request.insert(QStringLiteral("registerType"), QStringLiteral("HoldingRegisters"));

        const QJsonArray array{request};

        settings.insert(QStringLiteral("requests"), array);

        auto parser = new CommModbusSettingsParser(settings);

        testCases.push_back(std::make_tuple(QStringLiteral(":/cases/settings/modbusrtu.json"), parser));
    }

    {
        // Test modbus TCP settings
        QJsonObject settings{};

        settings.insert(QStringLiteral("host"), QStringLiteral("192.68.50.94"));
        settings.insert(QStringLiteral("port"), 502);
        settings.insert(QStringLiteral("retries"), 5);
        settings.insert(QStringLiteral("timeout"), 500);
        settings.insert(QStringLiteral("pollingInterval"), 1000);
        settings.insert(QStringLiteral("pollingEnabled"), false);

        auto parser = new CommModbusSettingsParser(settings);

        testCases.push_back(std::make_tuple(QStringLiteral(":/cases/settings/modbustcp.json"), parser));
    }

    return testCases;
}

INSTANTIATE_TEST_SUITE_P(TestSettingsParser, TestCommModbusSettingsParser, ValuesIn(TestCommModbusSettingsParser::LoadTestCases()));

