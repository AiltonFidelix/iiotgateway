#!/bin/bash

shopt -s nullglob
INSTALLERS=( IIoTGateway-*-aarch64.deb IIoTDashboard-*.deb )

echo "Installing dependencies..."

apt-get update && apt-get --fix-broken install

apt-get install -y \
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
