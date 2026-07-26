# IIoTGateway

**IIoTGateway** is a protocol bridge platform that enables reading from and writing to `Modbus RTU` devices through the `MQTT` protocol, allowing Modbus networks to be seamlessly integrated into modern monitoring and control systems.

The platform is delivered as a custom Linux distribution built with `Buildroot` for `Raspberry Pi` devices, where all required applications are pre-integrated and ready to run. It combines the protocol gateway, a web-based management interface, and the underlying operating system into a single solution, making deployment straightforward and consistent.

---

## Features

- Modbus RTU read and write support
- MQTT-based data publishing and control
- Web-based dashboard for configuration and monitoring
- Embedded Linux distribution built with buildroot
- Targeted for Raspberry Pi hardware

---

## Requirements

These requirements apply only to the **development environment**, not to the target Raspberry Pi system.

### Toolchain

- GCC ≥ 14 (Needs support for C++23 standard)
- CMake ≥ 3.20
- Python ≥ 3.10.12
- Go ≥ 1.26

### Dependencies

- Qt 6.8.3
    - Desktop
    - WebAssembly (single-threaded)
    - Modules:
        - Qt HTTP Server
        - Qt Serial Bus
        - Qt Serial Port

**Note:** For development Qt needs to be installed manually.

Others dependencies/libraries can be installed using the `init-environment` script.

```sh
python3 init-environment.py
```

The script will install the following items:

- googletest v1.16.0
- paho.mqtt.cpp v1.5.1
- emscripten 3.1.56
- buildroot 2025.02.x

**Note:** This script will check the required tools and ask you to install if not find.

---

## Project Structure

```sh
.
├── dashboard   # Web-based UI for configuration and monitoring
├── distro      # Buildroot-based embedded Linux distribution
├── gateway     # Core service (Modbus <-> MQTT bridge)
└── manager     # Backend manager for settings and gateway control (In developement)
```

- **dashboard:** Browser-based interface for system configuration and operation
- **manager:** Backend for the dashboard and system control
- **gateway:** Core application responsible for protocol translation and device communication
- **distro:** Custom Linux image for Raspberry Pi, bundling the gateway and dashboard

Architecture diagram:

![diagram](/docs/images/architecture_diagram.png)

**Note:** `LedStatus` and `Manager` applications are still being migrating from the `Gateway` (This is the intended architecture). 

---

## Documentation

- [Modbus Request Structure](./docs/MODBUS.md)
- [Dashboard Settings](./docs/DASHBOARD.md)

---

## Building

The default build targets a **Raspberry Pi 3**.

To run a default build of the **IIoTGateway** platform:

```sh
python3 build.py --default
```

This will execute the full build using the default configuration for the Raspberry Pi 3.

Additional command-line options are available to customize the build process (e.g., cleaning artifacts, adjusting parallel jobs, or skipping components). Use `--help` to see all available options:

```sh
python3 build.py --help
```

## Note

This is an evolving project, some bugs and undefined behavior may be found yet, as well as some difference in performance depend on the number of registers you are reading/writing. If you want to use this project, be aware of these issues.

Suggestions, new ideas, and contributions are welcome.