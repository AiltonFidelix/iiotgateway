#include "testmqttsettingsparser.h"
#include "testutils.h"

#include <QJsonDocument>
#include <MQTTAsync.h>

void
TestMQTTSettingsParser::SetUp()
{
    const auto bytearray = TestUtils::readJsonFile(":/settings/mqtt.json");
    const auto object = QJsonDocument::fromJson(bytearray).object();

    m_parser = new MQTTSettingsParser(object);
}

void
TestMQTTSettingsParser::TearDown()
{
    if (m_parser)
    {
        delete m_parser;
    }
}

TEST_F(TestMQTTSettingsParser, TestAutoReconnect)
{
    const auto autoReconnect = m_parser->autoReconnect();
    ASSERT_TRUE(autoReconnect);
}

TEST_F(TestMQTTSettingsParser, TestCleanStart)
{
    const auto cleanStart = m_parser->cleanStart();
    ASSERT_TRUE(cleanStart);
}

TEST_F(TestMQTTSettingsParser, TestClientId)
{
    const auto clientId = m_parser->clientId();
    ASSERT_EQ("iiotgateway", clientId);
}

TEST_F(TestMQTTSettingsParser, TestConnectionTimeout)
{
    const auto connectionTimeout = m_parser->connectionTimeout();
    ASSERT_EQ(10, connectionTimeout);
}

TEST_F(TestMQTTSettingsParser, TestHost)
{
    const auto host = m_parser->host();
    ASSERT_EQ("localhost", host);
}

TEST_F(TestMQTTSettingsParser, TestKeepAlive)
{
    const auto keepalive = m_parser->keepAlive();
    ASSERT_EQ(60, keepalive);
}

TEST_F(TestMQTTSettingsParser, TestPassword)
{
    const auto password = m_parser->password();
    ASSERT_EQ("testerpass", password);
}

TEST_F(TestMQTTSettingsParser, TestPort)
{
    const auto port = m_parser->port();
    ASSERT_EQ(1883, port);
}

TEST_F(TestMQTTSettingsParser, TestProtocol)
{
    const auto protocol = m_parser->protocol();
    ASSERT_EQ("mqtt", protocol);
}

TEST_F(TestMQTTSettingsParser, TestUsername)
{
    const auto username = m_parser->username();
    ASSERT_EQ("testeruser", username);
}

TEST_F(TestMQTTSettingsParser, TestVersion)
{
    const auto version = m_parser->version();
    ASSERT_EQ(MQTTVERSION_3_1, version);
}

TEST_F(TestMQTTSettingsParser, TestPublish)
{
    const auto publish = m_parser->publish();
    const auto topic = m_parser->publishTopic();
    const auto qos = m_parser->publishQos();

    ASSERT_TRUE(publish);
    ASSERT_EQ("iiotgateway/edge", topic);
    ASSERT_EQ(0, qos);
}

TEST_F(TestMQTTSettingsParser, TestSubscribe)
{
    const auto subscribe = m_parser->subscribe();
    const auto topic = m_parser->subscribeTopic();
    const auto qos = m_parser->subscribeQos();

    ASSERT_TRUE(subscribe);
    ASSERT_EQ("iiotgateway/cloud", topic);
    ASSERT_EQ(0, qos);
}
