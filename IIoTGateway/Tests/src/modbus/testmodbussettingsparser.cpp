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

        settings.insert("baudrate", "115200");
        settings.insert("dataBits", 8);
        settings.insert("parity", "None");
        settings.insert("pollingEnabled", false);
        settings.insert("pollingTimeout", 1000);
        settings.insert("port", "default");
        settings.insert("retries", 5);
        settings.insert("stopBits", 1);
        settings.insert("timeout", 1000);

        auto parser = new ModbusSettingsParser(settings);

        testCases.push_back(std::make_tuple(":/cases/settings/modbusrtu.json", parser));
    }

    {
        // Test modbus TCP settings
        QJsonObject settings{};

        auto parser = new ModbusSettingsParser(settings);

        testCases.push_back(std::make_tuple(":/cases/settings/modbustcp.json", parser));
    }

    return testCases;
}

INSTANTIATE_TEST_SUITE_P(TestSettingsParser, TestModbusSettingsParser, ::testing::ValuesIn(TestModbusSettingsParser::LoadTestCases()));

