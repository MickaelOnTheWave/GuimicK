#!/bin/sh

TARGET_DIRECTORY="/home/mickael/Prog/TaskManager/bin/SynologyPackage/"

TOOLS_BUILD_PATH="/home/mickael/Prog/Tools/bin/Synology/"
TOOLSLIB_BIN_PATH=$TOOLS_BUILD_PATH"ToolsLib/"
TOOLSLIB_PATH="/home/mickael/Prog/Tools/ToolsLib/"

NETWORKTOOLSLIB_BIN_PATH=$TOOLS_BUILD_PATH"NetworkToolsLib/"
NETWORKTOOLSLIB_PATH="/home/mickael/Prog/Tools/NetworkToolsLib/"

TASKMANAGER_BUILD_PATH="/home/mickael/Prog/TaskManager/bin/Synology/"

PARSERSLIB_BIN_PATH1="/home/mickael/Prog/TaskManager/bin/Synology/ReportParsers/"
PARSERSLIB_BIN_PATH=$PARSERSLIB_BIN_PATH1"ParsersLib/"
PARSERSLIB_PATH="/home/mickael/Prog/TaskManager/ReportParsers/ParsersLib/"

TASKLIB_BIN_PATH="/home/mickael/Prog/TaskManager/bin/Synology/TaskLib"
TASKLIB_PATH="/home/mickael/Prog/TaskManager/TaskLib/"

TASKTOOL_BIN_PATH="/home/mickael/Prog/TaskManager/bin/Synology/TaskTool"
TASKTOOL_PATH="/home/mickael/Prog/TaskManager/TaskTool/"

PARSERTOOL_BIN_PATH="/home/mickael/Prog/TaskManager/bin/Synology/ReportParsers/CommandLineTool/"
PARSERTOOL_PATH="/home/mickael/Prog/TaskManager/ReportParsers/CommandLineTool/"

NETWORKTESTER_BIN_PATH=$TOOLS_BUILD_PATH"NetworkTester/"
NETWORKTESTER_PATH="/home/mickael/Prog/Tools/NetworkTester/"


createFolderIfNotPresent()
{
	FOLDER=$1	
	if [ ! -d "$FOLDER" ]; then
	mkdir $FOLDER
	fi
}

buildModule()
{
	NAME=$1
	LIBPATH=$2
	BINPATH=$3
	MAKEFILE=$NAME"Makefile"
	
	createFolderIfNotPresent $BINPATH
	cd $BINPATH
	qmake -spec linux-arm-synology -o $MAKEFILE "CONFIG+=synology" $LIBPATH/$NAME".pro"
	make -f $MAKEFILE
}

createFolderIfNotPresent $TOOLS_BUILD_PATH
createFolderIfNotPresent $TASKMANAGER_BUILD_PATH

buildModule "ToolsLib" $TOOLSLIB_PATH $TOOLSLIB_BIN_PATH

#buildModule "NetworkToolsLib" $NETWORKTOOLSLIB_PATH $NETWORKTOOLSLIB_BIN_PATH

createFolderIfNotPresent $PARSERSLIB_BIN_PATH1
buildModule "ParsersLib" $PARSERSLIB_PATH $PARSERSLIB_BIN_PATH

buildModule "TaskLib" $TASKLIB_PATH $TASKLIB_BIN_PATH

buildModule "TaskTool" $TASKTOOL_PATH $TASKTOOL_BIN_PATH 

buildModule "CommandLineTool" $PARSERTOOL_PATH $PARSERTOOL_BIN_PATH

buildModule "NetworkTester" $NETWORKTESTER_PATH $NETWORKTESTER_BIN_PATH

# Copy everything into synology folder, ready for transfer
if [ -d "$TARGET_DIRECTORY" ]; then
  rm -rf $TARGET_DIRECTORY
fi

mkdir $TARGET_DIRECTORY

cp $TOOLSLIB_BIN_PATH/libToolsLib.so $TARGET_DIRECTORY/
#cp $NETWORKTOOLSLIB_BIN_PATH/libNetworkToolsLib.so $TARGET_DIRECTORY/
cp $PARSERSLIB_BIN_PATH/libParsersLib.so $TARGET_DIRECTORY/
cp $TASKLIB_BIN_PATH/libTaskLib.so $TARGET_DIRECTORY/
cp $TASKTOOL_BIN_PATH/TaskTool $TARGET_DIRECTORY/
cp $PARSERTOOL_BIN_PATH/CommandLineTool $TARGET_DIRECTORY/
cp $NETWORKTESTER_BIN_PATH/NetworkTester $TARGET_DIRECTORY/

