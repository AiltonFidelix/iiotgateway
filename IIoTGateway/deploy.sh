#!/bin/bash

ARCH=$1
ARCHS=("aarch64")

APP_VERSION="1.0.0"
APP_NAME="IIoTGateway"
APP_FOLDER="$APP_NAME-deploy"

QT_MAJOR_VERSION="6"
QT_MINOR_VERSION="8.3"
QT_VERSION="$QT_MAJOR_VERSION.$QT_MINOR_VERSION"
QT_LIBRARIES=("libQt6Core" "libQt6HttpServer" "libQt6Sql" "libQt6Network" "libQt6WebSockets" "libQt6SerialBus" "libQt6SerialPort")
QT_LIBRARIES_PATH="/Qt/$QT_VERSION/arm64/lib"

LIBRARIES_INSTALL_PATH=$APP_FOLDER/usr/lib

DOCKER_IMAGE="qt-$ARCH:$QT_VERSION"
DOCKER_CONTAINER="qt-$QT_VERSION-$ARCH"

options() {
    echo "Usage ./deploy.sh [OPTION]"
    echo "  Options: ${ARCHS[@]}"
    exit 0
}

if [ $# -eq 0 ]; then
    options
elif [[ ! ${ARCHS[@]} =~ $ARCH ]]; then
    options
fi

bash build.sh $ARCH

rm -rf $APP_FOLDER

# Create the necessary paths to the application deploy
mkdir $APP_FOLDER
mkdir -p $APP_FOLDER/DEBIAN
mkdir -p $APP_FOLDER/usr/bin
mkdir -p $LIBRARIES_INSTALL_PATH

# Copy scripts and application files
cp control $APP_FOLDER/DEBIAN
cp build-$ARCH/Main/$APP_NAME $APP_FOLDER/usr/bin/$APP_NAME
cp build-$ARCH/Application/libApplication.so $LIBRARIES_INSTALL_PATH
cp build-$ARCH/Communication/libCommunication.so $LIBRARIES_INSTALL_PATH
cp build-$ARCH/Device/libDevice.so $LIBRARIES_INSTALL_PATH

docker run -d --name=$DOCKER_CONTAINER $DOCKER_IMAGE

# Copy Qt shared libraries
for qtlib in ${QT_LIBRARIES[@]}
do
    docker cp $DOCKER_CONTAINER:$QT_LIBRARIES_PATH/$qtlib.so.$QT_VERSION ./$LIBRARIES_INSTALL_PATH
    docker cp $DOCKER_CONTAINER:$QT_LIBRARIES_PATH/$qtlib.so.$QT_MAJOR_VERSION ./$LIBRARIES_INSTALL_PATH
done

# Copy other shared libraries
docker cp $DOCKER_CONTAINER:/qt/arm64/sysroot/usr/lib/libwiringPi.so ./$LIBRARIES_INSTALL_PATH
docker cp $DOCKER_CONTAINER:/qt/arm64/sysroot/usr/lib/libwiringPi.so.3.16 ./$LIBRARIES_INSTALL_PATH
docker cp $DOCKER_CONTAINER:/qt/arm64/sysroot/usr/local/lib/libpaho-mqttpp3.so.1 ./$LIBRARIES_INSTALL_PATH
docker cp $DOCKER_CONTAINER:/qt/arm64/sysroot/usr/local/lib/libpaho-mqttpp3.so.1.5.1 ./$LIBRARIES_INSTALL_PATH
docker cp $DOCKER_CONTAINER:/qt/arm64/sysroot/usr/local/lib/libpaho-mqtt3as.so.1 ./$LIBRARIES_INSTALL_PATH
docker cp $DOCKER_CONTAINER:/qt/arm64/sysroot/usr/local/lib/libpaho-mqtt3as.so.1.3.14 ./$LIBRARIES_INSTALL_PATH

docker container rm $DOCKER_CONTAINER

# Generate the .deb installer
dpkg --build $APP_FOLDER

# Clean dir
rm -rf $APP_FOLDER

# Rename .deb
mv $APP_FOLDER.deb $APP_NAME-$APP_VERSION-$ARCH.deb
