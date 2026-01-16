# IIotGateway

IIoTGateway is a bridge application that allows reading and writing data via Modbus RTU and transmitting this information using the MQTT protocol, enabling the integration of Modbus devices with modern monitoring and control systems. 


### Request JSON

Reading example:

```json
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

The example above demonstrates how to read registers 0–25 from a Modbus device configured with address 240.

Writing example:

```json
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

The example above demonstrates how to write on the register 1 from a Modbus device configured with address 240.

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

Env variables:

```bash
DB_TYPE=QSQLITE
DB_NAME=IIoTGateway.db
PLATFORM=host | raspberry
```

Dashboard login:

```
username: admin
password gtwAdmin
```

journalctl -u iiotgateway.service 