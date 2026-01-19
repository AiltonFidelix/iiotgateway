# TODO

### Structure

- Split the projet into to two repositories `iiotdasboard` and `iiotgateway`
- Created docker images for each one, being able to use with a docker compose
- Adjust deploy script to be able to deploy as docker image or as .deb installer 
- Move build/deploy scripts to a specific folder
- Create an option in cmake to install gtest and mqtt paho

### Protocols

- Add Modbus TCP
- Add HTTP server/client

### Code

- Refactor reboot code, creating an interface/factory according the platform