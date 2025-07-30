# IIotGateway

This is a fake description.

### Docker commands

```bash
docker exec -it mosquitto mosquitto_sub -h localhost -t gateway/topic
docker exec -it mosquitto mosquitto_pub -h localhost -t gateway/topic -m "Hello from Docker!"
```

### Mosquitto settings

mosquitto.conf

```
allow_anonymous true
listener 1883 0.0.0.0
```

### Request JSON

Reading example:

```bash
{
    "devices": [
        {
            "address": 240,
            "type": "HoldingRegisters",
            "startRegister": 0,
            "numberOfEntries": 25
        }
    ]
}
```

Writing example:

```bash
{
    "devices": [
        {
            "address": 240,
            "type": "HoldingRegisters",
            "startRegister": 1,
            "numberOfEntries": 1,
            "values": [1]
        }
    ]
}
```

Env variables:

```bash
EDGE_PROTOCOL=MODBUS_RTU
CLOUD_PROTOCOL=MQTT
MODBUS_RTU_PORT=/dev/ttyUSB0
MODBUS_RTU_PARITY=none
MODBUS_RTU_BAUDRATE=115200
MODBUS_RTU_DATABITS=8
MODBUS_RTU_STOPBITS=1
MODBUS_RTU_POLLING=enabled
MQTT_PROTOCOL=mqtt
MQTT_HOST=localhost
MQTT_SUBSCRIBE=enabled
MQTT_PORT=1883
MQTT_CLIENT_ID=iiotgateway
MQTT_PUB_TOPIC=iiotgateway/modbusrtu
MQTT_SUB_TOPIC=iiotgateway/mqtt
MQTT_QOS=0
DB_TYPE=QSQLITE
DB_NAME=IIoTGateway.db
```

Dashboard login:

```
username: admin
password gtwAdmin
```