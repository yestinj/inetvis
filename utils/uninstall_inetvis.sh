#!/bin/bash

echo "Uninstalling inetvis..."

BIN_NAME=inetvis
VER="<ver>"
ICON_PATH=/usr/share/icons/hicolor/48x48/apps
APPS_PATH=/usr/share/applications
USR_BIN=/usr/local/bin


if [ -f "${APPS_PATH}/${BIN_NAME}.desktop" ]; then
    echo "Removing desktop file..."
    sudo rm ${APPS_PATH}/${BIN_NAME}.desktop
else
    echo "Not removing desktop file, it doesn't exist"
fi

if [ -f "${ICON_PATH}/${BIN_NAME}.png" ]; then
    echo "Removing icon..."
    sudo rm ${ICON_PATH}/${BIN_NAME}.png
else
    echo "Not removing icon, it doesn't exist"
fi

if [ -L "${USR_BIN}/${BIN_NAME}" ]; then
    echo "Removing ${USR_BIN} symlink..."
    sudo rm ${USR_BIN}/${BIN_NAME}
else
    echo "Not removing symlink ${USR_BIN}/${BIN_NAME}, it doesn't exist"
fi

if [ -L "/opt/${BIN_NAME}" ]; then
    echo "Removing /opt symlink..."
    sudo rm /opt/${BIN_NAME}
else
    echo "Not removing /opt/${BIN_NAME}, doesn't exist"
fi

if [ -d "/opt/${BIN_NAME}-${VER}" ]; then
    echo "Removing /opt/${BIN_NAME}-${VER} directory..."
    sudo rm -rf /opt/${BIN_NAME}-${VER}
else 
    echo "Not removing /opt/${BIN_NAME}-${VER}, doesn't exist"
fi

echo "Done!"
