#include "testcommmodbussettingsparser.h"
#include "testutils.h"

#include <QJsonDocument>
#include <QJsonObject>

void
TestModbusSettingsParser::SetUp()
{
    const auto bytearray = TestUtils::readJsonFile(":/settings/modbusrtu.json");
    const auto object = QJsonDocument::fromJson(bytearray).object();

#warning // TODO: implement test
}

void
TestModbusSettingsParser::TearDown()
{
}
