#include <gtest/gtest.h>

#include <QJsonDocument>
#include <memory>

#include "mqtt/commmqttsettingsparser.hpp"
#include "testutils.hpp"

using comm::commmqtt::CommMQTTSettingsParser;

using TestCases = std::tuple<QString, CommMQTTSettingsParser *>;

class TestCommMQTTSettingsParser : public testing::TestWithParam<TestCases> {
public:
    static std::vector<TestCases> LoadTestCases();
};

TEST_P(TestCommMQTTSettingsParser, TestSettingsParser) {
    const auto param = GetParam();

    const auto filePath = std::get<QString>(param);
    std::unique_ptr<CommMQTTSettingsParser> expectedParser{std::get<CommMQTTSettingsParser *>(param)};

    ASSERT_TRUE(expectedParser != nullptr);

    const QByteArray data = TestUtils::readJsonFile(filePath);
    const QJsonObject settings = QJsonDocument::fromJson(data).object();

    CommMQTTSettingsParser actualParser(settings);

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
}

std::vector<TestCases> TestCommMQTTSettingsParser::LoadTestCases() {
    std::vector<TestCases> testCases{};

    {
        // Test without settings
        auto parser = new CommMQTTSettingsParser();
        testCases.push_back(std::make_tuple(QStringLiteral(""), parser));
    }

    {
        // Test with settings
        QJsonObject settings{};

        settings.insert(QStringLiteral("protocol"), QStringLiteral("mqtt://"));
        settings.insert(QStringLiteral("host"), QStringLiteral("localhost"));
        settings.insert(QStringLiteral("username"), QStringLiteral("testeruser"));
        settings.insert(QStringLiteral("password"), QStringLiteral("testerpass"));
        settings.insert(QStringLiteral("clientId"), QStringLiteral("iiotgateway"));
        settings.insert(QStringLiteral("publishTopic"), QStringLiteral("iiotgateway/edge"));
        settings.insert(QStringLiteral("subscribeTopic"), QStringLiteral("iiotgateway/cloud"));

        settings.insert(QStringLiteral("connectionTimeout"), 10);
        settings.insert(QStringLiteral("keepAlive"), 60);
        settings.insert(QStringLiteral("port"), 1883);
        settings.insert(QStringLiteral("version"), QStringLiteral("3.1"));
        settings.insert(QStringLiteral("publishQos"), 0);
        settings.insert(QStringLiteral("subscribeQos"), 0);

        settings.insert(QStringLiteral("autoReconnect"), true);
        settings.insert(QStringLiteral("cleanStart"), true);
        settings.insert(QStringLiteral("publish"), true);
        settings.insert(QStringLiteral("subscribe"), true);

        auto parser = new CommMQTTSettingsParser(settings);

        testCases.push_back(std::make_tuple(QStringLiteral(":/cases/settings/mqtt.json"), parser));
    }

    return testCases;
}

INSTANTIATE_TEST_SUITE_P(TestSettingsParser, TestCommMQTTSettingsParser, ::testing::ValuesIn(TestCommMQTTSettingsParser::LoadTestCases()));
