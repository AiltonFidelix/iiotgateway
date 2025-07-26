#ifndef TESTMQTTSETTINGSPARSER_H
#define TESTMQTTSETTINGSPARSER_H

#include <gtest/gtest.h>
#include "mqtt/commmqttsettingsparser.h"

using commmqtt::MQTTSettingsParser;

class TestMQTTSettingsParser : public testing::TestWithParam<std::tuple<QByteArray, MQTTSettingsParser*>>
{
public:
    static std::vector<std::tuple<QByteArray, MQTTSettingsParser*>> LoadTestCases();
};

#endif // TESTMQTTSETTINGSPARSER_H
