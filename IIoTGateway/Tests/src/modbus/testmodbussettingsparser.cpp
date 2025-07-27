#include <gtest/gtest.h>
#include <QJsonDocument>

#include "testutils.h"

#include "modbus/modbussettingsparser.h"

using commmodbus::ModbusSettingsParser;

using TestCases = std::tuple<QByteArray, ModbusSettingsParser*>;

class TestModbusSettingsParser : public testing::TestWithParam<TestCases>
{
public:
    static std::vector<TestCases> LoadTestCases();
};

TEST_P(TestModbusSettingsParser, TestSettingsParser)
{
    const auto param = GetParam();

    const auto filePath = std::get<QByteArray>(param);
    auto expectedParser = std::get<ModbusSettingsParser*>(param);

    ASSERT_TRUE(expectedParser != nullptr);

    const QByteArray data = TestUtils::readJsonFile(filePath);
    const QJsonObject object = QJsonDocument::fromJson(data).object();

    ModbusSettingsParser actualParser(object);

    ASSERT_EQ(expectedParser->host(), actualParser.host());
    ASSERT_EQ(expectedParser->port(), actualParser.port());
    ASSERT_EQ(expectedParser->baudrate(), actualParser.baudrate());
    ASSERT_EQ(expectedParser->parity(), actualParser.parity());

    ASSERT_EQ(expectedParser->databits(), actualParser.databits());
    ASSERT_EQ(expectedParser->stopbits(), actualParser.stopbits());
    ASSERT_EQ(expectedParser->retries(), actualParser.retries());
    ASSERT_EQ(expectedParser->timeout(), actualParser.timeout());
    ASSERT_EQ(expectedParser->pollingTimeout(), actualParser.pollingTimeout());

    ASSERT_EQ(expectedParser->pollingEnabled(), actualParser.pollingEnabled());

    delete expectedParser;
}

std::vector<TestCases> TestModbusSettingsParser::LoadTestCases()
{
    std::vector<TestCases> testCases{};

    {
        // Test without settings
        auto parser = new ModbusSettingsParser();
        testCases.push_back(std::make_tuple("", parser));
    }

    {
        // Test modbus RTU settings
        QJsonObject settings{};

        settings.insert("port", "default");
        settings.insert("baudrate", "115200");
        settings.insert("parity", "None");
        settings.insert("dataBits", 8);
        settings.insert("stopBits", 1);
        settings.insert("retries", 10);
        settings.insert("timeout", 2000);
        settings.insert("pollingInterval", 5000);
        settings.insert("pollingEnabled", true);

        auto parser = new ModbusSettingsParser(settings);

        testCases.push_back(std::make_tuple(":/cases/settings/modbusrtu.json", parser));
    }

    {
        // Test modbus TCP settings
        QJsonObject settings{};

        settings.insert("host", "192.68.50.94");
        settings.insert("port", 502);
        settings.insert("retries", 5);
        settings.insert("timeout", 500);
        settings.insert("pollingInterval", 1000);
        settings.insert("pollingEnabled", false);

        auto parser = new ModbusSettingsParser(settings);

        testCases.push_back(std::make_tuple(":/cases/settings/modbustcp.json", parser));
    }

    return testCases;
}

INSTANTIATE_TEST_SUITE_P(TestSettingsParser, TestModbusSettingsParser, ::testing::ValuesIn(TestModbusSettingsParser::LoadTestCases()));

