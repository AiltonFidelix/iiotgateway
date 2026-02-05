# IIotGateway

IIoTGateway is a bridge application that allows reading and writing data via Modbus RTU and transmitting this information using the MQTT protocol, enabling the integration of Modbus devices with modern monitoring and control systems. 

### Requirements

- C++ 17
- Qt >= 6.8.3
- googletest v1.16.0
- paho.mqtt.cpp v1.5.1

### Environment variables

| Variable | Description | Value | Supported |
| --- | --- | --- | --- |
| DB_TYPE | Database driver | QSQLITE | QSQLITE |
| DB_NAME | Database name | IIoTGateway.db | anyname.db |
| PLATFORM | Platform used | raspberry | host, raspberry |

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

The example above demonstrates how to write the value 1 on the register 1 from a Modbus device configured with address 240.

### Dashboard

Dashboard login:

```sh
username: admin
password gtwAdmin
```