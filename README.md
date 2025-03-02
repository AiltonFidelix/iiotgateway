# IIotGateway

This is a fake description.

### Docker commands

```bash
docker exec -it mosquitto mosquitto_sub -h localhost -t gateway/topic
docker exec -it mosquitto mosquitto_pub -h localhost -t gateway/topic -m "Hello from Docker!"
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