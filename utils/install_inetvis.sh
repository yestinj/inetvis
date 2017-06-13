#!/bin/bash

BIN_NAME=inetvis
VER="<ver>"

ICON_PATH=/usr/share/icons/hicolor/48x48/apps/
APPS_PATH=/usr/share/applications/
USR_BIN=/usr/local/bin

echo "Creating directories..."

OPT_DIR_VER=/opt/${BIN_NAME}-${VER}
if [[ ! -d "${OPT_DIR_VER}" ]]; then
    sudo mkdir ${OPT_DIR_VER}
else
    echo "Not creating ${OPT_DIR_VER} as it already exists"
fi

OPT_DIR_SYM=/opt/${BIN_NAME}
if [[ ! -L "${OPT_DIR_SYM}" ]]; then
    sudo ln -s /opt/${BIN_NAME}-${VER} /opt/${BIN_NAME}
else
    echo "Symlink for ${OPT_DIR_SYM} already exists, updating it to ${OPT_DIR_VER}."
    sudo rm /opt/${BIN_NAME}
    sudo ln -s /opt/${BIN_NAME}-${VER} /opt/${BIN_NAME}
fi

echo "Copying files..."
sudo cp -r bin/ /opt/${BIN_NAME}-${VER}
sudo cp resources/${BIN_NAME}.png ${ICON_PATH}
sudo cp -r doc/ /opt/${BIN_NAME}-${VER}
sudo cp -r lic/ /opt/${BIN_NAME}-${VER}

echo "Copying menu entry..."
sudo cp resources/${BIN_NAME}.desktop ${APPS_PATH} 

echo "Creating symlink for inetvis..."

if [[ ! -L "${USR_BIN}/${BIN_NAME}" ]]; then
    sudo ln -s /opt/${BIN_NAME}/bin/${BIN_NAME} ${USR_BIN} 
else
    echo "Symlink at ${USR_BIN}/${BIN_NAME} already exists."
fi

echo "Setting capabilities on binary to allow monitoring interfaces"
sudo setcap cap_net_raw,cap_net_admin=eip /opt/${BIN_NAME}/bin/${BIN_NAME}

echo "Done"
