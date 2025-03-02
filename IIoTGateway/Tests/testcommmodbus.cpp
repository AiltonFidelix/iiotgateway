#include "testcommmodbus.h"
#include "testutils.h"

#include <QModbusReply>
#include <QJsonArray>
#include <QJsonObject>

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

TEST_F(TestCommModbus, TestReadRequest)
{
    const quint8 expectedAddress = 240;
    const quint8 maxRegisters = 10;
    const QList<quint16> expectedValues{ 1, 50, 58, 100, 6528, 2048, 0, 65535, 35, 88 };

    auto reply = new QModbusReply(QModbusReply::Common, expectedAddress);

    auto emitFinished = [&reply]() -> void
    {
        emit reply->finished();
    };

    auto unit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, maxRegisters);
    unit.setValues(expectedValues);

    reply->setResult(unit);

    EXPECT_CALL(*m_mockModbusClient, sendReadRequest(testing::_, testing::_)).WillOnce(testing::Return(reply));

    auto request = TestUtils::readJsonFile(":/requests/readone.json");

    QTimer::singleShot(1, emitFinished);

    QByteArray result;

    auto handleResult = [&result](QByteArray data) -> void
    {
        result = data;
    };

    QObject::connect(&m_commModbus, &CommModbus::outgoing, handleResult);

    m_commModbus.incoming(request);

    auto jsonResult = QJsonDocument::fromJson(result).object();

    auto devices = jsonResult.value("devices").toArray();

    ASSERT_EQ(1, devices.count());

    auto firstDevice = devices.first().toObject();

    ASSERT_EQ(expectedAddress, firstDevice.value("address").toInt());

    auto registers = firstDevice.value("registers").toArray();

    ASSERT_EQ(maxRegisters, registers.count());

    for (const auto reg : registers)
    {
        auto obj = reg.toObject();
        auto regAddress = obj.value("register").toInt();
        auto regValue = obj.value("value").toInt();

        ASSERT_EQ(expectedValues.at(regAddress), regValue);
    }
}

TEST_F(TestCommModbus, TestDisconnect)
{
    EXPECT_CALL(*m_mockModbusClient, disconnectDevice()).Times(1);

    m_commModbus.disconnectComm();
}
