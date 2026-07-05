#include "mockmqttclient.hpp"
#include "mqtt/commmqtt.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <mqtt/token.h>

using comm::commmqtt::CommMQTT;

class TestCommMQTT: public testing::Test
{
protected:
    MockMQTTClient *m_mockMQTTClient;
    CommMQTT *m_commMQTT;

    void SetUp() override;
    void TearDown() override;
};

void TestCommMQTT::SetUp()
{
    m_mockMQTTClient = new MockMQTTClient();
    m_commMQTT = new CommMQTT();

    m_commMQTT->setMQTTClient(m_mockMQTTClient);
}

void TestCommMQTT::TearDown()
{
    if (m_commMQTT)
    {
        delete m_commMQTT;
    }
}

TEST_F(TestCommMQTT, TestConnectionSuccess)
{
    EXPECT_CALL(*m_mockMQTTClient, connect(testing::_, testing::_, testing::_)).WillOnce(testing::Return(std::shared_ptr<mqtt::token>()));
    m_commMQTT->connectComm();
}

TEST_F(TestCommMQTT, TestIsConnected)
{
    EXPECT_CALL(*m_mockMQTTClient, is_connected()).WillOnce(testing::Return(true));

    EXPECT_TRUE(m_commMQTT->isconnected());
}


TEST_F(TestCommMQTT, TestDisconnect)
{
    EXPECT_CALL(*m_mockMQTTClient, is_connected()).WillOnce(testing::Return(true));
    EXPECT_CALL(*m_mockMQTTClient, disconnect()).WillOnce(testing::Return(std::shared_ptr<mqtt::token>()));

    m_commMQTT->disconnectComm();
}
