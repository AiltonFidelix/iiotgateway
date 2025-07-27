#include <gtest/gtest.h>
#include <QJsonDocument>

#include "testutils.h"

#include "mqtt/mqttsettingsparser.h"

using commmqtt::MQTTSettingsParser;

using TestCases = std::tuple<QByteArray, MQTTSettingsParser*>;

class TestMQTTSettingsParser : public testing::TestWithParam<TestCases>
{
public:
    static std::vector<TestCases> LoadTestCases();
};

TEST_P(TestMQTTSettingsParser, TestSettingsParser)
{
    const auto param = GetParam();

    const auto filePath = std::get<QByteArray>(param);
    auto expectedParser = std::get<MQTTSettingsParser*>(param);

    ASSERT_TRUE(expectedParser != nullptr);

    const QByteArray data = TestUtils::readJsonFile(filePath);
    const QJsonObject object = QJsonDocument::fromJson(data).object();

    MQTTSettingsParser actualParser(object);

    ASSERT_EQ(expectedParser->protocol(), actualParser.protocol());
    ASSERT_EQ(expectedParser->host(), actualParser.host());
    ASSERT_EQ(expectedParser->username(), actualParser.username());
    ASSERT_EQ(expectedParser->password(), actualParser.password());
    ASSERT_EQ(expectedParser->clientId(), actualParser.clientId());
    ASSERT_EQ(expectedParser->publishTopic(), actualParser.publishTopic());
    ASSERT_EQ(expectedParser->subscribeTopic(), actualParser.subscribeTopic());

    ASSERT_EQ(expectedParser->connectionTimeout(), actualParser.connectionTimeout());
    ASSERT_EQ(expectedParser->keepAlive(), actualParser.keepAlive());
    ASSERT_EQ(expectedParser->port(), actualParser.port());
    ASSERT_EQ(expectedParser->version(), actualParser.version());
    ASSERT_EQ(expectedParser->publishQos(), actualParser.publishQos());
    ASSERT_EQ(expectedParser->subscribeQos(), actualParser.subscribeQos());

    ASSERT_EQ(expectedParser->autoReconnect(), actualParser.autoReconnect());
    ASSERT_EQ(expectedParser->cleanStart(), actualParser.cleanStart());
    ASSERT_EQ(expectedParser->publish(), actualParser.publish());
    ASSERT_EQ(expectedParser->subscribe(), actualParser.subscribe());

    delete expectedParser;
}

std::vector<TestCases> TestMQTTSettingsParser::LoadTestCases()
{
    std::vector<TestCases> testCases{};

    {
        // Test without settings
        auto parser = new MQTTSettingsParser();
        testCases.push_back(std::make_tuple("", parser));
    }

    {
        // Test with settings
        QJsonObject settings{};

        settings.insert("protocol", "mqtt://");
        settings.insert("host", "localhost");
        settings.insert("username", "testeruser");
        settings.insert("password", "testerpass");
        settings.insert("clientId", "iiotgateway");
        settings.insert("publishTopic", "iiotgateway/edge");
        settings.insert("subscribeTopic", "iiotgateway/cloud");

        settings.insert("connectionTimeout", 10);
        settings.insert("keepAlive", 60);
        settings.insert("port", 1883);
        settings.insert("version", "3.1");
        settings.insert("publishQos", 0);
        settings.insert("subscribeQos", 0);

        settings.insert("autoReconnect", true);
        settings.insert("cleanStart", true);
        settings.insert("publish", true);
        settings.insert("subscribe", true);

        auto parser = new MQTTSettingsParser(settings);

        testCases.push_back(std::make_tuple(":/cases/settings/mqtt.json", parser));
    }

    return testCases;
}

INSTANTIATE_TEST_SUITE_P(TestSettingsParser, TestMQTTSettingsParser, ::testing::ValuesIn(TestMQTTSettingsParser::LoadTestCases()));
