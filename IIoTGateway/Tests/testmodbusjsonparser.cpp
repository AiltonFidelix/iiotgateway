#include "testmodbusjsonparser.h"

void
TestModbusJsonParserRead::SetUp()
{
    QByteArray data = "{"
                      " \"devices\": ["
                      "     {"
                      "         \"address\": 240,"
                      "         \"type\": \"HoldingRegisters\","
                      "         \"registers\": [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ]"
                      "     }"
                      " ]"
                      "}";

    m_parser = new ModbusJsonParser(data);
}

void
TestModbusJsonParserRead::TearDown()
{
    if (m_parser)
    {
        delete m_parser;
    }
}

TEST_F(TestModbusJsonParserRead, TestReadRequest)
{
    ModbusJsonParser::Request readRequest = m_parser->readRequest();

    ASSERT_FALSE(readRequest.isEmpty());
    ASSERT_EQ(1, readRequest.uniqueKeys().count());
}

void
TestModbusJsonParserReadEmpty::SetUp()
{
    m_parser = new ModbusJsonParser(QByteArray());
}

void
TestModbusJsonParserReadEmpty::TearDown()
{
    if (m_parser)
    {
        delete m_parser;
    }
}

TEST_F(TestModbusJsonParserReadEmpty, TestReadRequestEmpty)
{
    ModbusJsonParser::Request readRequest = m_parser->readRequest();

    ASSERT_TRUE(readRequest.isEmpty());
}
