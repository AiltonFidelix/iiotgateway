#!/bin/bash

ARCH=$1
ARCHS=("aarch64")

APP_VERSION="1.0.0"
APP_NAME="IIoTGateway"
APP_FOLDER="$APP_NAME-deploy"

QT_VERSION="6.8.3"
QT_DEPENDENCIES=("libQt6Core" "libQt6HttpServer" "libQt6Sql" "libQt6Network" "libQt6WebSockets" "libQt6SerialBus" "libQt6SerialPort")
QT_LIB_PATH=$HOME/Qt/$QT_VERSION/gcc_64/lib
QT_PLUGINS_PATH=$HOME/Qt/$QT_VERSION/gcc_64/plugins

LIB_PATH=$APP_FOLDER/usr/lib

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
mkdir -p $LIB_PATH

# Copy scripts and application files
cp control $APP_FOLDER/DEBIAN
cp build-$ARCH/Main/$APP_NAME $APP_FOLDER/usr/bin/$APP_NAME
cp build-$ARCH/Application/libApplication.so $LIB_PATH
cp build-$ARCH/Communication/libCommunication.so $LIB_PATH
cp build-$ARCH/Device/libDevice.so $LIB_PATH

if [ $ARCH == "aarch64" ]; then
    QT_LIB_PATH="/Qt/$QT_VERSION/arm64/lib"
fi

docker run -d --name=$DOCKER_CONTAINER $DOCKER_IMAGE

# Copy Qt shared libraries
for qtlib in ${QT_DEPENDENCIES[@]}
do
    docker cp $DOCKER_CONTAINER:$QT_LIB_PATH/$qtlib.so.$QT_VERSION ./$LIB_PATH
done

# docker cp $DOCKER_CONTAINER:$QT_LIB_PATH/libicui18n.so.73 ./$LIB_PATH
# docker cp $DOCKER_CONTAINER:$QT_LIB_PATH/libicuuc.so.73 ./$LIB_PATH
# docker cp $DOCKER_CONTAINER:$QT_LIB_PATH/libicudata.so.73 ./$LIB_PATH

docker container rm $DOCKER_CONTAINER

# Generate the .deb installer
dpkg --build $APP_FOLDER

# Clean dir
rm -rf $APP_FOLDER

# Rename .deb
mv $APP_FOLDER.deb $APP_NAME-$APP_VERSION-$ARCH.deb
