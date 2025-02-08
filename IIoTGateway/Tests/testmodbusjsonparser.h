#ifndef TESTMODBUSJSONPARSER_H
#define TESTMODBUSJSONPARSER_H

#include <gtest/gtest.h>
#include "modbus/modbusjsonparser.h"

class TestModbusJsonParserReadOne: public testing::Test
{
protected:
    ModbusJsonParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

class TestModbusJsonParserReadMultiple: public testing::Test
{
protected:
    ModbusJsonParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

class TestModbusJsonParserReadEmpty: public testing::Test
{
protected:
    ModbusJsonParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

class TestModbusJsonParserWriteOne: public testing::Test
{
protected:
    ModbusJsonParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

class TestModbusJsonParserWriteMultiple: public testing::Test
{
protected:
    ModbusJsonParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

class TestModbusJsonParserWriteEmpty: public testing::Test
{
protected:
    ModbusJsonParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

#endif // TESTMODBUSJSONPARSER_H
