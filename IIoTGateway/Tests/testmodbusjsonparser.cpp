#include "testmodbusjsonparser.h"

void
TestModbusJsonParserReadOne::SetUp()
{
    QByteArray data = "{"
                      " \"devices\": ["
                      "     {"
                      "         \"address\": 240,"
                      "         \"type\": \"HoldingRegisters\","
                      "         \"startRegister\": 0,"
                      "         \"numberOfEntries\": 10"
                      "     }"
                      " ]"
                      "}";

    m_parser = new ModbusJsonParser(data);
}

void
TestModbusJsonParserReadOne::TearDown()
{
    if (m_parser)
    {
        delete m_parser;
    }
}

TEST_F(TestModbusJsonParserReadOne, TestReadRequest)
{
    ModbusJsonParser::Request readRequest = m_parser->readRequest();

    ASSERT_FALSE(readRequest.isEmpty());

    auto addresses = readRequest.keys();
    ASSERT_EQ(1, addresses.count());

    auto address = addresses.first();
    ASSERT_EQ(240, address);

    auto unit = readRequest.value(address).at(0);

    ASSERT_TRUE(unit.isValid());
    ASSERT_EQ(QModbusDataUnit::HoldingRegisters, unit.registerType());
    ASSERT_EQ(0, unit.startAddress());
}

void
TestModbusJsonParserReadMultiple::SetUp()
{
    QByteArray data = "{"
                      " \"devices\": ["
                      "     {"
                      "         \"address\": 1,"
                      "         \"type\": \"HoldingRegisters\","
                      "         \"startRegister\": 0,"
                      "         \"numberOfEntries\": 10"
                      "     },"
                      "     {"
                      "         \"address\": 1,"
                      "         \"type\": \"HoldingRegisters\","
                      "         \"startRegister\": 10,"
                      "         \"numberOfEntries\": 10"
                      "     },"
                      "     {"
                      "         \"address\": 2,"
                      "         \"type\": \"HoldingRegisters\","
                      "         \"startRegister\": 0,"
                      "         \"numberOfEntries\": 40"
                      "     }"
                      " ]"
                      "}";

    m_parser = new ModbusJsonParser(data);
}

void
TestModbusJsonParserReadMultiple::TearDown()
{
    if (m_parser)
    {
        delete m_parser;
    }
}

TEST_F(TestModbusJsonParserReadMultiple, TestReadRequest)
{
    auto testUnit = [](auto unit) -> void
    {
        ASSERT_TRUE(unit.isValid());
        ASSERT_EQ(QModbusDataUnit::HoldingRegisters, unit.registerType());
        ASSERT_EQ(10, unit.valueCount());
    };

    ModbusJsonParser::Request readRequest = m_parser->readRequest();

    ASSERT_FALSE(readRequest.isEmpty());

    auto addresses = readRequest.keys();
    ASSERT_EQ(2, addresses.count());

    auto units1 = readRequest.value(1);
    ASSERT_EQ(2, units1.count());

    for (const auto &unit : units1)
    {
        testUnit(unit);
    }

    auto units2 = readRequest.value(2);
    ASSERT_EQ(4, units2.count());

    for (const auto &unit : units2)
    {
        testUnit(unit);
    }
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

void
TestModbusJsonParserWriteOne::SetUp()
{
    m_parser = new ModbusJsonParser(QByteArray());
}

void
TestModbusJsonParserWriteOne::TearDown()
{
    if (m_parser)
    {
        delete m_parser;
    }
}

TEST_F(TestModbusJsonParserWriteOne, TestWriteRequest)
{
    auto writeRequest = m_parser->readRequest();

    GTEST_SKIP() << "Implement this test";
}

void
TestModbusJsonParserWriteEmpty::SetUp()
{
    m_parser = new ModbusJsonParser(QByteArray());
}

void
TestModbusJsonParserWriteEmpty::TearDown()
{
    if (m_parser)
    {
        delete m_parser;
    }
}

TEST_F(TestModbusJsonParserWriteEmpty, TestWriteRequestEmpty)
{
    auto writeRequest = m_parser->readRequest();

    ASSERT_TRUE(writeRequest.isEmpty());
}

