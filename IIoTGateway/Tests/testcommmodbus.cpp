#include "testcommmodbus.h"

void
TestCommModbus::SetUp()
{
    m_mockModbusClient = new MockModbusClient();

    m_commModbus.setModbusClient(m_mockModbusClient);
}

TEST_F(TestCommModbus, TestConnectiongSuccess)
{
    EXPECT_CALL(*m_mockModbusClient, state()).WillOnce(testing::Return(QModbusDevice::UnconnectedState));
    EXPECT_CALL(*m_mockModbusClient, setConnectionParameter(testing::_, testing::_)).Times(5);
    EXPECT_CALL(*m_mockModbusClient, setTimeout(testing::_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, setNumberOfRetries(testing::_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, connectDevice()).WillOnce(testing::Return(true));

    m_commModbus.connectComm();
}

TEST_F(TestCommModbus, TestConnectiongFailed)
{
    EXPECT_CALL(*m_mockModbusClient, state()).WillOnce(testing::Return(QModbusDevice::UnconnectedState));
    EXPECT_CALL(*m_mockModbusClient, setConnectionParameter(testing::_, testing::_)).Times(5);
    EXPECT_CALL(*m_mockModbusClient, setTimeout(testing::_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, setNumberOfRetries(testing::_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, connectDevice()).WillOnce(testing::Return(false));
    EXPECT_CALL(*m_mockModbusClient, errorString()).WillOnce(testing::Return("Error from GTEST"));

    m_commModbus.connectComm();
}

TEST_F(TestCommModbus, TestIsConnected)
{
    EXPECT_CALL(*m_mockModbusClient, state()).WillOnce(testing::Return(QModbusDevice::ConnectedState));

    EXPECT_TRUE(m_commModbus.isconnected());
}

TEST_F(TestCommModbus, TestDisconnect)
{
    EXPECT_CALL(*m_mockModbusClient, disconnectDevice()).Times(1);

    m_commModbus.disconnectComm();
}
