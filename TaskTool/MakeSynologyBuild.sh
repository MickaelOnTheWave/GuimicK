#!/bin/sh

BUILD_DIRECTORY="synology_build/"
TOOLSLIB_PATH="/home/mickael/Prog/Tools/ToolsLib/"
PARSERSLIB_PATH="/home/mickael/Prog/TaskManager/ReportParsers/ParsersLib/"
TASKLIB_PATH="/home/mickael/Prog/TaskManager/TaskLib/"

if [ -d "$BUILD_DIRECTORY" ]; then
  rm -rf $BUILD_DIRECTORY
fi

mkdir $BUILD_DIRECTORY
cd $BUILD_DIRECTORY

# Building Tools lib
qmake -spec linux-arm-synology -o toolsMakefile "CONFIG+=synology" $TOOLSLIB_PATH/ToolsLib.pro
make -f toolsMakefile

# Building Parsers lib
qmake -spec linux-arm-synology -o parsersMakefile "CONFIG+=synology" $PARSERSLIB_PATH/ParsersLib.pro
make -f parsersMakefile

# Building Task lib
qmake -spec linux-arm-synology -o taskMakefile "CONFIG+=synology" $TASKLIB_PATH/TaskLib.pro
make -f taskMakefile

# Building Task tool
qmake -spec linux-arm-synology -o execMakefile "CONFIG+=synology" ../TaskTool.pro
make -f execMakefile
