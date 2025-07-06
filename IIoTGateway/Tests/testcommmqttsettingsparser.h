#ifndef TESTMQTTSETTINGSPARSER_H
#define TESTMQTTSETTINGSPARSER_H

#include <gtest/gtest.h>
#include "mqtt/commmqttsettingsparser.h"

using commmqtt::MQTTSettingsParser;

class TestMQTTSettingsParser : public testing::Test
{
protected:
    MQTTSettingsParser *m_parser;

    void SetUp() override;
    void TearDown() override;
};

#endif // TESTMQTTSETTINGSPARSER_H
