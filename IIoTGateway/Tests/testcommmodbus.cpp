#include "testcommmodbus.h"

void CommModbusTest::SetUp()
{
    // Create a mock QModbusClient
    mockModbusClient = new MockModbusClient();
    // Inject the mock into the CommModbus instance
    commModbus.setModbusClient(mockModbusClient);
}

TEST_F(CommModbusTest, TestConnectComm) 
{
    // Set up the mock behavior
    EXPECT_CALL(*mockModbusClient, state()).WillOnce(testing::Return(QModbusDevice::UnconnectedState));
    EXPECT_CALL(*mockModbusClient, setConnectionParameter(testing::_, testing::_)).Times(5);
    EXPECT_CALL(*mockModbusClient, setTimeout(testing::_)).Times(1);
    EXPECT_CALL(*mockModbusClient, setNumberOfRetries(testing::_)).Times(1);
    EXPECT_CALL(*mockModbusClient, connectDevice()).WillOnce(testing::Return(false));
    EXPECT_CALL(*mockModbusClient, errorString()).WillOnce(testing::Return("Error from GTEST"));

    // Call the method being tested
    commModbus.connectComm();
}

TEST_F(CommModbusTest, TestIsConnected)
{
    // Set up mock to return connected state
    EXPECT_CALL(*mockModbusClient, state()).WillOnce(testing::Return(QModbusDevice::ConnectedState));

    // Call the method being tested
    EXPECT_TRUE(commModbus.isconnected());
}

TEST_F(CommModbusTest, TestDisconnectComm)
{
    // Set up the mock behavior
    EXPECT_CALL(*mockModbusClient, disconnectDevice()).Times(1);

    // Call the method being tested
    commModbus.disconnectComm();
}
