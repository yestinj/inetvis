#!/bin/bash

BIN_NAME=inetvis
VER="<ver>"

ICON_PATH=/usr/share/icons/hicolor/48x48/apps/
APPS_PATH=/usr/share/applications/
USR_BIN=/usr/local/bin

echo "Creating directories..."
sudo mkdir /opt/${BIN_NAME}-${VER}
sudo ln -s /opt/${BIN_NAME}-${VER} /opt/${BIN_NAME}

echo "Copying files..."
sudo cp -r bin/ /opt/${BIN_NAME}-${VER}
sudo cp resources/${BIN_NAME}.png ${ICON_PATH}
sudo cp -r doc/ /opt/${BIN_NAME}-${VER}
sudo cp -r lic/ /opt/${BIN_NAME}-${VER}

echo "Copying menu entry..."
sudo cp resources/${BIN_NAME}.desktop ${APPS_PATH} 

echo "Creating symlink for inetvis..."
sudo ln -s /opt/${BIN_NAME}/bin/${BIN_NAME} ${USR_BIN} 

echo "Setting capabilities on binary to allow monitoring interfaces"
sudo setcap cap_net_raw,cap_net_admin=eip /opt/${BIN_NAME}/bin/${BIN_NAME}

echo "Done"
