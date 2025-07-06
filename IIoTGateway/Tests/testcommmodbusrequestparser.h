#ifndef TESTCOMMMODBUSSETTINGSPARSER_H
#define TESTCOMMMODBUSSETTINGSPARSER_H

#include <gtest/gtest.h>
#include "modbus/commmodbusrequestparser.h"

using commmodbus::CommModbusRequestParser;

class TestModbusJsonParserReadOne : public testing::Test
{
protected:
    CommModbusRequestParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

class TestModbusJsonParserReadMultiple : public testing::Test
{
protected:
    CommModbusRequestParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

class TestModbusJsonParserWriteOne : public testing::Test
{
protected:
    CommModbusRequestParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

class TestModbusJsonParserWriteMultiple : public testing::Test
{
protected:
    CommModbusRequestParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

class TestModbusJsonParserEmpty : public testing::Test
{
protected:
    CommModbusRequestParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

#endif // TESTCOMMMODBUSSETTINGSPARSER_H
