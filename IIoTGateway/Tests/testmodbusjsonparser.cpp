#include "testmodbusjsonparser.h"
#include "testutils.h"

void
TestModbusJsonParserReadOne::SetUp()
{
    auto data = TestUtils::readJsonFile(":/requests/readone.json");

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
    const auto readRequest = m_parser->request();
    const auto type = m_parser->type();

    ASSERT_FALSE(readRequest.isEmpty());
    ASSERT_EQ(ModbusJsonParser::Read, type);

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
    auto data = TestUtils::readJsonFile(":/requests/readmultiple.json");

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

    const auto readRequest = m_parser->request();
    const auto type = m_parser->type();

    ASSERT_FALSE(readRequest.isEmpty());
    ASSERT_EQ(ModbusJsonParser::Read, type);

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
TestModbusJsonParserWriteOne::SetUp()
{
    auto data = TestUtils::readJsonFile(":/requests/writeone.json");

    m_parser = new ModbusJsonParser(data);
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
    const auto writeRequest = m_parser->request();
    const auto type = m_parser->type();

    ASSERT_FALSE(writeRequest.isEmpty());
    ASSERT_EQ(ModbusJsonParser::Write, type);

    auto addresses = writeRequest.keys();
    ASSERT_EQ(1, addresses.count());

    auto address = addresses.first();
    ASSERT_EQ(55, address);

    auto unit = writeRequest.value(address).at(0);

    ASSERT_TRUE(unit.isValid());
    ASSERT_EQ(QModbusDataUnit::HoldingRegisters, unit.registerType());
    ASSERT_EQ(3, unit.startAddress());

    QList<quint16> expected = { 88, 2, 65533, 4, 5, 0, 7, 8, 255, 10 };
    auto values = unit.values();

    bool result = std::equal(values.begin(), values.end(), expected.begin());

    ASSERT_TRUE(result);
}

void
TestModbusJsonParserWriteMultiple::SetUp()
{
    auto data = TestUtils::readJsonFile(":/requests/writemultiple.json");

    m_parser = new ModbusJsonParser(data);
}

void
TestModbusJsonParserWriteMultiple::TearDown()
{
    if (m_parser)
    {
        delete m_parser;
    }
}

TEST_F(TestModbusJsonParserWriteMultiple, TestWriteRequest)
{
    auto testUnit = [] (auto type, auto start, auto &expected, auto &unit) -> void
    {
        ASSERT_TRUE(unit.isValid());
        ASSERT_EQ(type, unit.registerType());
        ASSERT_EQ(start, unit.startAddress());

        auto values = unit.values();

        ASSERT_EQ(expected.count(), values.count());

        bool result = std::equal(values.begin(), values.end(), expected.begin());

        ASSERT_TRUE(result);
    };

    const auto writeRequest = m_parser->request();
    const auto type = m_parser->type();

    ASSERT_FALSE(writeRequest.isEmpty());
    ASSERT_EQ(ModbusJsonParser::Write, type);

    auto addresses = ModbusJsonParser::sortedAddress(writeRequest);
    ASSERT_EQ(2, addresses.count());

    QList<quint8> expectedAddresses = { 55, 60 };

    ASSERT_EQ(expectedAddresses.count(), addresses.count());

    bool result = std::equal(addresses.begin(), addresses.end(), expectedAddresses.begin());

    ASSERT_TRUE(result);

    auto unit1 = writeRequest.value(addresses.at(0));

    ASSERT_EQ(1, unit1.count());

    QList<quint16> expected = { 88, 2, 65533, 4, 5, 0, 7, 8, 255, 10 };

    testUnit(QModbusDataUnit::HoldingRegisters, 3, expected, unit1.at(0));

    auto unit2 = writeRequest.value(addresses.at(1));

    ASSERT_EQ(2, unit2.count());

    expected = { 88, 2, 65533, 4, 55, 0, 7, 8, 255, 1080 };

    testUnit(QModbusDataUnit::HoldingRegisters, 0, expected, unit2.at(0));

    expected = { 68, 72, 780, 2556, 101, 99, 0, 23 };

    testUnit(QModbusDataUnit::HoldingRegisters, 10, expected, unit2.at(1));
}

void
TestModbusJsonParserEmpty::SetUp()
{
    m_parser = new ModbusJsonParser(QByteArray());
}

void
TestModbusJsonParserEmpty::TearDown()
{
    if (m_parser)
    {
        delete m_parser;
    }
}

TEST_F(TestModbusJsonParserEmpty, TestRequestEmpty)
{
    auto readRequest = m_parser->request();
    auto type = m_parser->type();

    ASSERT_EQ(ModbusJsonParser::Unknown, type);
    ASSERT_TRUE(readRequest.isEmpty());
}
