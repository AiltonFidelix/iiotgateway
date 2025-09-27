#!/bin/bash

APP_VERSION="1.0.0"
APP_NAME="IIoTDashboard"
APP_FOLDER="$APP_NAME-deploy"
BUILD_PATH=build/WebAssembly_Qt_6_8_3_single_threaded-Debug

rm -rf $APP_FOLDER
rm *.deb

# Create the necessary paths to the application deploy
mkdir $APP_FOLDER
mkdir -p $APP_FOLDER/DEBIAN
mkdir -p $APP_FOLDER/etc/nginx/sites-available
mkdir -p $APP_FOLDER/var/www/iiotdashboard


cp control $APP_FOLDER/DEBIAN
cp postinst $APP_FOLDER/DEBIAN
cp nginx/iiotdashboard $APP_FOLDER/etc/nginx/sites-available
cp index.html $APP_FOLDER/var/www/iiotdashboard
cp favicon.ico $APP_FOLDER/var/www/iiotdashboard
cp $BUILD_PATH/$APP_NAME.js $APP_FOLDER/var/www/iiotdashboard
cp $BUILD_PATH/$APP_NAME.wasm $APP_FOLDER/var/www/iiotdashboard
cp $BUILD_PATH/qtloader.js $APP_FOLDER/var/www/iiotdashboard

chmod +x $APP_FOLDER/DEBIAN/postinst

# Generate the .deb installer
dpkg --build $APP_FOLDER

# Clean dir
rm -rf $APP_FOLDER

# Rename .deb
mv $APP_FOLDER.deb $APP_NAME-$APP_VERSION.deb
