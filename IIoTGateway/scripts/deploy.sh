#!/bin/bash

ARCH=$1
ARCHS=("aarch64")

APP_VERSION="1.0.0"
APP_NAME="IIoTGateway"
APP_FOLDER="$APP_NAME-deploy"

QT_VERSION="6.8.3"
QT_LIBRARIES=("libQt6Core" "libQt6HttpServer" "libQt6Sql" "libQt6Network" "libQt6WebSockets" "libQt6SerialBus" "libQt6SerialPort")
QT_LIBRARIES_PATH="/Qt/$QT_VERSION/arm64/lib"

EXECUTABLE_INSTALL_PATH=$APP_FOLDER/opt/iiotgateway
LIBRARIES_INSTALL_PATH=$APP_FOLDER/opt/iiotgateway/lib

DOCKER_IMAGE="qt-$ARCH:$QT_VERSION"
DOCKER_CONTAINER="qt-$QT_VERSION-$ARCH"

options() {
    echo "Usage ./deploy.sh [OPTION]"
    echo "  Options: ${ARCHS[@]}"
    exit 0
}

copySharedLibraryFromDocker() {
    path=$1
    file=$2
    docker run --rm $DOCKER_IMAGE bash -c "cd $path && tar czf - $file" > libs.tar.gz
    tar xzf libs.tar.gz -C ./$LIBRARIES_INSTALL_PATH
    rm libs.tar.gz
}

if [ $# -eq 0 ]; then
    options
elif [[ ! ${ARCHS[@]} =~ $ARCH ]]; then
    options
fi

bash scripts/build.sh $ARCH -c

rm -rf $APP_FOLDER

# Create the necessary paths to the application deploy
mkdir $APP_FOLDER
mkdir -p $APP_FOLDER/DEBIAN
mkdir -p $APP_FOLDER/opt/iiotgateway
mkdir -p $APP_FOLDER/opt/iiotgateway/plugins/sqldrivers
mkdir -p $APP_FOLDER/usr/lib/systemd/system
mkdir -p $LIBRARIES_INSTALL_PATH

# Copy scripts and application files
cp debian/control $APP_FOLDER/DEBIAN
cp debian/preinst $APP_FOLDER/DEBIAN
cp debian/postinst $APP_FOLDER/DEBIAN
cp service/iiotgateway.service $APP_FOLDER/usr/lib/systemd/system
cp service/rungateway $EXECUTABLE_INSTALL_PATH
cp build-$ARCH/Main/$APP_NAME $EXECUTABLE_INSTALL_PATH/iiotgateway
cp build-$ARCH/Application/libApplication.so* $LIBRARIES_INSTALL_PATH
cp build-$ARCH/Communication/libCommunication.so* $LIBRARIES_INSTALL_PATH
cp build-$ARCH/Device/libDevice.so* $LIBRARIES_INSTALL_PATH

chmod +x $EXECUTABLE_INSTALL_PATH/rungateway

# Copy Qt shared libraries
for qtlib in ${QT_LIBRARIES[@]}
do
    copySharedLibraryFromDocker "$QT_LIBRARIES_PATH" "$qtlib.so*"
done

# Copy Qt sqlite plugin
copySharedLibraryFromDocker "/Qt/$QT_VERSION/arm64/plugins/sqldrivers" "libqsqlite.so"
mv $LIBRARIES_INSTALL_PATH/libqsqlite.so $EXECUTABLE_INSTALL_PATH/plugins/sqldrivers

# Copy other shared libraries
copySharedLibraryFromDocker "/qt/arm64/sysroot/usr/lib" "libwiringPi.so*"
copySharedLibraryFromDocker "/qt/arm64/sysroot/usr/local/lib" "libpaho-mqttpp3.so*"
copySharedLibraryFromDocker "/qt/arm64/sysroot/usr/local/lib" "libpaho-mqtt3as.so*"
copySharedLibraryFromDocker "/qt/arm64/sysroot/lib/aarch64-linux-gnu" "libicu*.so*"

# Generate the .deb installer
dpkg --build $APP_FOLDER

# Clean dir
rm -rf $APP_FOLDER

# Rename .deb
mv $APP_FOLDER.deb $APP_NAME-$APP_VERSION-$ARCH.deb
