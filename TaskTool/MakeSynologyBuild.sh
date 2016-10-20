#!/bin/sh

BUILD_DIRECTORY="synology_build/"
TOOLSLIB_PATH="../../../Tools/ToolsLib/"

if [ -d "$BUILD_DIRECTORY" ]; then
  rm -rf $BUILD_DIRECTORY
fi

mkdir $BUILD_DIRECTORY
cd $BUILD_DIRECTORY

# Building Tools lib
qmake -spec linux-arm-synology -o toolsMakefile ./$TOOLSLIB_PATH/ToolsLib.pro
make -f toolsMakefile

# Building BackupAgent
qmake -spec linux-arm-synology -o execMakefile "CONFIG+=synology" ../BackupAgent.pro
make -f execMakefile
