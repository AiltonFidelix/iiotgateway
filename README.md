# IIotGateway

IIoTGateway is a protocol bridge platform that allows reading and writing data via Modbus RTU and transmitting this information using the MQTT protocol, enabling the integration of Modbus devices with modern monitoring and control systems. 

The platform combines a protocol gateway, a web-based management interface, and a custom Buildroot-based Linux distribution.

## Features

- Modbus RTU read and write support
- MQTT-based data publishing and control
- Web-based dashboard for configuration and monitoring
- Embedded Linux distribution built with buildroot
- Targeted for Raspberry Pi hardware

## Requirements

These requirements apply only to the **development environment**, not to the target Raspberry Pi system.

### Toolchain

- GCC ≥ 11.4.0
- CMake ≥ 3.20
- Python ≥ 3.10.12

### Dependencies

- Emscripten 3.1.56
- Qt 6.8.3
    - Desktop
    - WebAssembly (single-threaded)
    - Modules:
        - Qt HTTP Server
        - Qt Serial Bus
        - Qt Serial Port

### Libraries

- googletest v1.16.0 (via CMake FetchContent)
- paho.mqtt.cpp v1.5.1 (via CMake FetchContent)

#TODO Provide `docker` images for cross-compilation and WebAssembly builds.

## Project Structure

```sh
.
├── dashboard   # Web-based UI for configuration and monitoring
├── distro      # Buildroot-based embedded Linux distribution
└── gateway     # Core service (Modbus ↔ MQTT bridge
```

- **dashboard:** Browser-based interface for system configuration and operation
- **gateway:** Core application responsible for protocol translation and device communication
- **distro:** Custom Linux image for Raspberry Pi, bundling the gateway and dashboard

Architecture diagram:

![diagram](/docs/diagram.png)

## Documentation

- [Modbus Request Structure](./docs/MODBUS.md)
- [Dashboard Default Settings](./docs/DASHBOARD.md)

## Building

#TODO Build example using `python` script.