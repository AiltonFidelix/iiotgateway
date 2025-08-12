#include <gtest/gtest.h>

#include <QJsonDocument>

#include "testutils.h"

#include "network/networkmanager.h"
#include "network/networkmanagerfactory.h"

using device::network::NetworkManager;
using device::network::NetworkManagerFactory;

class TestNetworkManager: public testing::Test
{
};

TEST_F(TestNetworkManager, TestCreation)
{
    const QByteArray expectedData = TestUtils::readJsonFile(QStringLiteral(":/cases/network/networkmanager.json"));
    const auto expectedJson = QJsonDocument::fromJson(expectedData);

    NetworkManager *manager = NetworkManagerFactory::getNetworkManager(QStringLiteral("host"));

    ASSERT_TRUE(manager != nullptr);
    ASSERT_TRUE(manager->load());

    const QByteArray actualData = manager->settings();
    const auto actualJson = QJsonDocument::fromJson(actualData);

    ASSERT_EQ(expectedJson.toJson(QJsonDocument::Compact), actualJson.toJson(QJsonDocument::Compact));
}

