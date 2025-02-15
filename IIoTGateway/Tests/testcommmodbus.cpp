#include "testcommmodbus.h"

#include <QObject>
#include <QFile>
#include <QModbusReply>

#warning // TODO: move TestUtils
QByteArray
readJsonFile2(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        return QByteArray();
    }

    auto data = file.readAll();

    file.close();

    return data;
}

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
    GTEST_SKIP_("Implement TestReadRequest");

    auto reply = new QModbusReply(QModbusReply::Common, 240);

    auto emitReply = [&reply]() -> void
    {
        emit reply->finished();
    };

    auto unit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters);

    unit.setStartAddress(0);
    unit.setValueCount(10);
    unit.setValues({ 1, 50, 58, 100, 6528, 0, 0, 0, 0, 0 });

    reply->setResult(unit);

    EXPECT_CALL(*m_mockModbusClient, sendReadRequest(testing::_, testing::_)).WillOnce(testing::Return(reply));

    auto data = readJsonFile2(":/json/readone.json");

    QTimer::singleShot(10, emitReply);

    QByteArray result;

    auto handleResult = [&result](QByteArray data) -> void
    {
        result = data;
    };

    QObject::connect(&m_commModbus, &CommModbus::outgoing, handleResult);

    m_commModbus.incoming(data);
}

TEST_F(TestCommModbus, TestDisconnect)
{
    EXPECT_CALL(*m_mockModbusClient, disconnectDevice()).Times(1);

    m_commModbus.disconnectComm();
}
