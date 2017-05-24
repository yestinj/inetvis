#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: ${0} <version>"
    exit 1
fi

VER=$1
DOC_DIR="../doc"
SRC_DIR="../src"
LIC_DIR="../lic"
BIN_NAME="inetvis"
IMAGE_DIR="images"

TMP_DIR="inetvis-${VER}"
BIN_DIR="bin"
RES_DIR="resources"

echo "Preparing archive for ${TMP_DIR}..."

echo "Creating working directory structure..."
mkdir ${TMP_DIR}
mkdir ${TMP_DIR}/${BIN_DIR}
mkdir ${TMP_DIR}/${RES_DIR}

echo "Copying binary..."
cp ${SRC_DIR}/${BIN_NAME} ${TMP_DIR}/${BIN_DIR}

echo "Copying docs..."
cp -r ${DOC_DIR} ${TMP_DIR}

echo "Copying licenses..."
cp -r ${LIC_DIR} ${TMP_DIR}

echo "Copying resources..."
cp ${BIN_NAME}.desktop ${TMP_DIR}/${RES_DIR}
cp ${SRC_DIR}/${IMAGE_DIR}/${BIN_NAME}.png ${TMP_DIR}/${RES_DIR}

echo "Copying scripts and readme..."
sed "s/<ver>/${VER}/g" install_${BIN_NAME}.sh > ${TMP_DIR}/install_${BIN_NAME}.sh
chmod a+x ${TMP_DIR}/install_${BIN_NAME}.sh
sed "s/<ver>/${VER}/g" uninstall_${BIN_NAME}.sh > ${TMP_DIR}/uninstall_${BIN_NAME}.sh
chmod a+x ${TMP_DIR}/uninstall_${BIN_NAME}.sh
cp ../README.md ${TMP_DIR}

echo "Creating archive ${TMP_DIR}.tgz..."
tar -zcf ${TMP_DIR}.tgz ${TMP_DIR}
echo "Removing directory ${TMP_DIR}"
rm -rf ${TMP_DIR}
echo "Done"
