#include "testcommmqttsettingsparser.h"
#include "testutils.h"

#include <MQTTAsync.h>

TEST_P(TestMQTTSettingsParser, TestSettingsParser)
{
    const auto param = GetParam();

    const QByteArray filePath = std::get<0>(param);
    MQTTSettingsParser *expectedParser = std::get<1>(param);

    ASSERT_TRUE(expectedParser != nullptr);

    const QByteArray data = TestUtils::readJsonFile(filePath);
    const QJsonObject object = QJsonDocument::fromJson(data).object();

    MQTTSettingsParser actualParser(object);

    ASSERT_EQ(expectedParser->autoReconnect(), actualParser.autoReconnect());
    ASSERT_EQ(expectedParser->cleanStart(), actualParser.cleanStart());
    ASSERT_EQ(expectedParser->publish(), actualParser.publish());
    ASSERT_EQ(expectedParser->subscribe(), actualParser.subscribe());

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

    delete expectedParser;
}

std::vector<std::tuple<QByteArray, MQTTSettingsParser*>> TestMQTTSettingsParser::LoadTestCases()
{
    std::vector<std::tuple<QByteArray, MQTTSettingsParser*>> cases{};

    {
        // Test without settings
        auto parser = new MQTTSettingsParser();
        cases.push_back(std::make_tuple("", parser));
    }

    {
        // Test with settings
        QJsonObject settings{};

        settings.insert("autoReconnect", true);
        settings.insert("cleanStart", true);
        settings.insert("publish", true);
        settings.insert("subscribe", true);

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

        auto parser = new MQTTSettingsParser(settings);

        cases.push_back(std::make_tuple(":/settings/mqtt.json", parser));
    }

    return cases;
}

INSTANTIATE_TEST_SUITE_P(TestSettingsParser, TestMQTTSettingsParser, ::testing::ValuesIn(TestMQTTSettingsParser::LoadTestCases()));
