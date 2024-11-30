#include "testcommmodbus.h"

void
TestCommModbus::SetUp()
{
    // Create a mock QModbusClient
    m_mockModbusClient = new MockModbusClient();
    // Inject the mock into the CommModbus instance
    m_commModbus.setModbusClient(m_mockModbusClient);
}

TEST_F(TestCommModbus, TestConnectComm)
{
    // Set up the mock behavior
    EXPECT_CALL(*m_mockModbusClient, state()).WillOnce(testing::Return(QModbusDevice::UnconnectedState));
    EXPECT_CALL(*m_mockModbusClient, setConnectionParameter(testing::_, testing::_)).Times(5);
    EXPECT_CALL(*m_mockModbusClient, setTimeout(testing::_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, setNumberOfRetries(testing::_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, connectDevice()).WillOnce(testing::Return(false));
    EXPECT_CALL(*m_mockModbusClient, errorString()).WillOnce(testing::Return("Error from GTEST"));

    // Call the method being tested
    m_commModbus.connectComm();
}

TEST_F(TestCommModbus, TestIsConnected)
{
    // Set up mock to return connected state
    EXPECT_CALL(*m_mockModbusClient, state()).WillOnce(testing::Return(QModbusDevice::ConnectedState));

    // Call the method being tested
    EXPECT_TRUE(m_commModbus.isconnected());
}

TEST_F(TestCommModbus, TestDisconnectComm)
{
    // Set up the mock behavior
    EXPECT_CALL(*m_mockModbusClient, disconnectDevice()).Times(1);

    // Call the method being tested
    m_commModbus.disconnectComm();
}
