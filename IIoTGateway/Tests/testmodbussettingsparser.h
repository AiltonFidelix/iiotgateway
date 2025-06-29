#ifndef TESTMODBUSSETTINGSPARSER_H
#define TESTMODBUSSETTINGSPARSER_H

#include <gtest/gtest.h>

class TestModbusSettingsParser : public testing::Test
{
protected:
    // MQTTSettingsParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

#endif // TESTMODBUSSETTINGSPARSER_H
