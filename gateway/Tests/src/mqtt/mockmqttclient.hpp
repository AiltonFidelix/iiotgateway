#ifndef MOCKMQTTCLIENT_H
#define MOCKMQTTCLIENT_H

#include <gmock/gmock.h>
#include <mqtt/iasync_client.h>

using namespace mqtt;

using qos_collection = std::vector<int>;

class TestableMQTTClientInterface : public mqtt::iasync_client {
protected:
    void remove_token(token *tok) override = 0;
};

class MockMQTTClient final : public TestableMQTTClientInterface {
public:
    MockMQTTClient() = default;

    // Connect
    MOCK_METHOD(token_ptr, connect, (), (override));
    MOCK_METHOD(token_ptr, connect, (connect_options), (override));
    MOCK_METHOD(token_ptr, connect, (connect_options, void *, iaction_listener &), (override));
    MOCK_METHOD(token_ptr, connect, (void *, iaction_listener &), (override));
    MOCK_METHOD(token_ptr, reconnect, (), (override));

    // Disconnect
    MOCK_METHOD(token_ptr, disconnect, (), (override));
    MOCK_METHOD(token_ptr, disconnect, (disconnect_options), (override));
    MOCK_METHOD(token_ptr, disconnect, (int), (override));
    MOCK_METHOD(token_ptr, disconnect, (int, void *, iaction_listener &), (override));
    MOCK_METHOD(token_ptr, disconnect, (void *, iaction_listener &), (override));

    // Publish
    MOCK_METHOD(delivery_token_ptr, publish, (string_ref, const void *, size_t, int, bool, const properties &), (override));
    MOCK_METHOD(delivery_token_ptr, publish, (string_ref, const void *, size_t), (override));
    MOCK_METHOD(delivery_token_ptr, publish, (string_ref, const void *, size_t, int, bool, void *, iaction_listener &), (override));
    MOCK_METHOD(delivery_token_ptr, publish, (string_ref, binary_ref, int, bool, const properties &), (override));
    MOCK_METHOD(delivery_token_ptr, publish, (string_ref, binary_ref), (override));
    MOCK_METHOD(delivery_token_ptr, publish, (const_message_ptr), (override));
    MOCK_METHOD(delivery_token_ptr, publish, (const_message_ptr, void *, iaction_listener &), (override));

    // Subscribe
    MOCK_METHOD(token_ptr, subscribe, (const std::string &, int, const subscribe_options &, const properties &), (override));
    MOCK_METHOD(token_ptr, subscribe, (const std::string &, int, void *, iaction_listener &, const subscribe_options &, const properties &), (override));
    MOCK_METHOD(token_ptr, subscribe, (const_string_collection_ptr, const qos_collection &, const std::vector<subscribe_options> &, const properties &), (override));
    MOCK_METHOD(token_ptr, subscribe, (const_string_collection_ptr, const qos_collection &, void *, iaction_listener &, const std::vector<subscribe_options> &, const properties &), (override));

    // Unsubscribe
    MOCK_METHOD(token_ptr, unsubscribe, (const std::string &, const properties &), (override));
    MOCK_METHOD(token_ptr, unsubscribe, (const_string_collection_ptr, const properties &), (override));
    MOCK_METHOD(token_ptr, unsubscribe, (const_string_collection_ptr, void *, iaction_listener &, const properties &), (override));
    MOCK_METHOD(token_ptr, unsubscribe, (const std::string &, void *, iaction_listener &, const properties &), (override));

    // Message consumption
    MOCK_METHOD(void, start_consuming, (), (override));
    MOCK_METHOD(void, stop_consuming, (), (override));
    MOCK_METHOD(const_message_ptr, consume_message, (), (override));
    MOCK_METHOD(bool, try_consume_message, (const_message_ptr *), (override));

    // Callback
    MOCK_METHOD(void, set_callback, (callback &), (override));
    MOCK_METHOD(void, disable_callbacks, (), (override));

    // Info methods
    MOCK_METHOD(delivery_token_ptr, get_pending_delivery_token, (int), (const, override));
    MOCK_METHOD(std::vector<delivery_token_ptr>, get_pending_delivery_tokens, (), (const, override));
    MOCK_METHOD(std::string, get_client_id, (), (const, override));
    MOCK_METHOD(std::string, get_server_uri, (), (const, override));
    MOCK_METHOD(bool, is_connected, (), (const, override));

    MOCK_METHOD(void, remove_token, (token *), (override));
};

#endif  // MOCKMQTTCLIENT_H
