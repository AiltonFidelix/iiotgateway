#include "testmodbussettingsparser.h"
#include "testutils.h"

#include <QJsonDocument>
#include <QJsonObject>

void
TestModbusSettingsParser::SetUp()
{
    const auto bytearray = TestUtils::readJsonFile(":/settings/modbusrtu.json");
    const auto object = QJsonDocument::fromJson(bytearray).object();

#warning // TODO: implement test

    // m_parser = new MQTTSettingsParser(object);
}

void
TestModbusSettingsParser::TearDown()
{
    // if (m_parser)
    // {
    //     delete m_parser;
    // }
}
