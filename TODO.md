# TODO

### Structure

- Split applications into different repositories
- Verify cmake adapters options and factories on C++ side
- Move the configuration http server to a separated application
- Move the led blink status to a separated application
- Start/stop gateway and led from other applications using systemctl

### Distro

- Install applications as packages, following buildroot approach
- Remove buildroot as submodule and add installation in init-environment script

### Gateway

- Close connections before reboot
- Status led keeps blinking faster after fail
- Add Modbus TCP as edge protocol
- Add HTTP server/client as cloud protocol

### Dashboard

- Create a control class for each page
- Improve error handlers
- Finish network settings page
- Add an option to change user password
- Display date/time and add an option to update
