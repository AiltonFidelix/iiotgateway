#!/bin/bash

shopt -s nullglob
INSTALLERS=( IIoTGateway-*-aarch64.deb IIoTDashboard-*.deb )

echo "Installing dependencies..."

apt-get update && apt-get --fix-broken install

apt-get install -y \
    python3-smbus \
    i2c-tools \
    nginx \
    sqlite3 \
    libqt6sql6-sqlite

echo "Looking for installers..."

if [ ${#INSTALLERS[@]} -gt 0 ]; then
    for installer in "${INSTALLERS[@]}"; do
        echo "$installer found, installing..."
        dpkg -i $installer
    done
else
    echo "No installers found."
fi

# --- Manual configurations ---
# 1 - sudo raspi-config -> Interface Options -> Serial Port Enable hardware serial
# 2 - sudo raspi-config -> Interface Options -> I2C -> Enable hardware I2C
# 3 - sudo nano /boot/firmware/config.txt -> Add dtoverlay=i2c-rtc,ds1307
