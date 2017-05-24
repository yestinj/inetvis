#!/bin/bash

echo "Uninstalling inetvis..."

BIN_NAME=inetvis
VER="<ver>"
ICON_PATH=/usr/share/icons/hicolor/48x48/apps
APPS_PATH=/usr/share/applications
USR_BIN=/usr/local/bin

echo "Removing ${USR_BIN} symlink..."
sudo rm ${USR_BIN}/${BIN_NAME}
echo "Removing /opt/${BIN_NAME}-${VER} directory..."
sudo rm -rf /opt/${BIN_NAME}-${VER}
echo "Removing /opt symlink..."
sudo rm /opt/${BIN_NAME}

echo "Removing icon..."
sudo rm ${ICON_PATH}/${BIN_NAME}.png
echo "Removing desktop file..."
sudo rm ${APPS_PATH}/${BIN_NAME}.desktop

echo "Done!"
