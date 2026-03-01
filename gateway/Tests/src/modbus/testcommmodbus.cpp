#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QModbusReply>
#include <QJsonArray>
#include <QJsonObject>

#include "testutils.h"

#include "modbus/commmodbusrtu.h"
#include "mockmodbusclient.h"

using testing::_;
using testing::DoAll;
using testing::Return;
using testing::Invoke;
using testing::Test;

using comm::commmodbus::CommModbus;
using comm::commmodbus::CommModbusRTU;

class TestCommModbus: public Test
{
protected:
    MockModbusClient *m_mockModbusClient;
    CommModbusRTU *m_commModbus;

    void SetUp() override;
    void TearDown() override;
};

void TestCommModbus::SetUp()
{
    m_mockModbusClient = new MockModbusClient();
    m_commModbus = new CommModbusRTU();

    m_commModbus->setModbusClient(m_mockModbusClient);
}

void TestCommModbus::TearDown()
{
    if (m_commModbus)
    {
        delete m_commModbus;
    }
}

TEST_F(TestCommModbus, TestConnectionSuccess)
{
    EXPECT_CALL(*m_mockModbusClient, state()).WillOnce(Return(QModbusDevice::UnconnectedState));
    EXPECT_CALL(*m_mockModbusClient, setConnectionParameter(_, _)).Times(5);
    EXPECT_CALL(*m_mockModbusClient, setTimeout(_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, setNumberOfRetries(_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, connectDevice()).WillOnce(Return(true));

    m_commModbus->connectComm();
}

TEST_F(TestCommModbus, TestConnectionFailed)
{
    EXPECT_CALL(*m_mockModbusClient, state()).WillOnce(Return(QModbusDevice::UnconnectedState));
    EXPECT_CALL(*m_mockModbusClient, setConnectionParameter(_, _)).Times(5);
    EXPECT_CALL(*m_mockModbusClient, setTimeout(_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, setNumberOfRetries(_)).Times(1);
    EXPECT_CALL(*m_mockModbusClient, connectDevice()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockModbusClient, errorString()).WillOnce(Return("Error from GTEST"));

    m_commModbus->connectComm();
}

TEST_F(TestCommModbus, TestIsConnected)
{
    EXPECT_CALL(*m_mockModbusClient, state()).WillOnce(Return(QModbusDevice::ConnectedState));

    EXPECT_TRUE(m_commModbus->isconnected());
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

    reply->setResult(QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, expectedValues));

    EXPECT_CALL(*m_mockModbusClient, sendReadRequest(_, _)).WillOnce(Return(reply));

    const QByteArray request = TestUtils::readJsonFile(QStringLiteral(":/cases/requests/readone.json"));

    QTimer::singleShot(1, emitFinished);

    QByteArray result{};

    auto handleResult = [&result](const QByteArray &data) -> void
    {
        result = data;
    };

    QObject::connect(m_commModbus, &CommModbus::outgoing, handleResult);

    m_commModbus->incoming(request);

    const QJsonObject jsonResult = QJsonDocument::fromJson(result).object();

    const QJsonArray devices = jsonResult.value(QStringLiteral("devices")).toArray();

    ASSERT_EQ(1, devices.count());

    const QJsonObject firstDevice = devices.first().toObject();

    ASSERT_EQ(expectedAddress, firstDevice.value(QStringLiteral("address")).toInt());

    const QJsonArray registers = firstDevice.value(QStringLiteral("registers")).toArray();

    ASSERT_EQ(maxRegisters, registers.count());

    for (const auto &reg : registers)
    {
        const QJsonObject obj = reg.toObject();
        const int regAddress = obj.value(QStringLiteral("register")).toInt();
        const int regValue = obj.value(QStringLiteral("value")).toInt();

        ASSERT_EQ(expectedValues.at(regAddress), regValue);
    }
}

TEST_F(TestCommModbus, TestDisconnect)
{
    EXPECT_CALL(*m_mockModbusClient, disconnectDevice()).Times(1);

    m_commModbus->disconnectComm();
}
