#!/bin/sh

TARGET_TYPE="Release"
PROG_ROOT="/home/mickael/Prog/"
TARGET_DIRECTORY=$PROG_ROOT"TaskManager/bin/"$TARGET_TYPE

TOOLS_BUILD_PATH=$PROG_ROOT"Tools/bin/"$TARGET_TYPE"/"
TOOLSLIB_BIN_PATH=$TOOLS_BUILD_PATH"ToolsLib/"
TOOLSLIB_PATH=$PROG_ROOT"Tools/ToolsLib/"

NETWORKTOOLSLIB_BIN_PATH=$TOOLS_BUILD_PATH"NetworkToolsLib/"
NETWORKTOOLSLIB_PATH=$PROG_ROOT"Tools/NetworkToolsLib/"

TASKMANAGER_BUILD_PATH=$PROG_ROOT"TaskManager/bin/"$TARGET_TYPE

PARSERSLIB_BIN_PATH_ROOT=$PROG_ROOT"TaskManager/bin/"$TARGET_TYPE"/ReportParsers/"
PARSERSLIB_BIN_PATH=$PARSERSLIB_BIN_PATH_ROOT"ParsersLib/"
PARSERSLIB_PATH=$PROG_ROOT"TaskManager/ReportParsers/ParsersLib/"

TASKLIB_BIN_PATH=$PROG_ROOT"TaskManager/bin/"$TARGET_TYPE"/TaskLib"
TASKLIB_PATH=$PROG_ROOT"TaskManager/TaskLib/"

TASKTOOL_BIN_PATH=$PROG_ROOT"TaskManager/bin/"$TARGET_TYPE"/TaskTool"
TASKTOOL_PATH=$PROG_ROOT"TaskManager/TaskTool/"

PARSERTOOL_BIN_PATH=$PROG_ROOT"TaskManager/bin/"$TARGET_TYPE"/ReportParsers/CommandLineTool/"
PARSERTOOL_PATH=$PROG_ROOT"TaskManager/ReportParsers/CommandLineTool/"

NETWORKTESTER_BIN_PATH=$TOOLS_BUILD_PATH"NetworkTester/"
NETWORKTESTER_PATH=$PROG_ROOT"Tools/NetworkTester/"

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
	qmake -o $MAKEFILE "CONFIG+=release" $LIBPATH/$NAME".pro"
	make -f $MAKEFILE
}

copyToDestination()
{
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
}


createFolderIfNotPresent $TOOLS_BUILD_PATH
createFolderIfNotPresent $TASKMANAGER_BUILD_PATH

buildModule "ToolsLib" $TOOLSLIB_PATH $TOOLSLIB_BIN_PATH

#buildModule "NetworkToolsLib" $NETWORKTOOLSLIB_PATH $NETWORKTOOLSLIB_BIN_PATH

createFolderIfNotPresent $PARSERSLIB_BIN_PATH_ROOT
buildModule "ParsersLib" $PARSERSLIB_PATH $PARSERSLIB_BIN_PATH

buildModule "TaskLib" $TASKLIB_PATH $TASKLIB_BIN_PATH

buildModule "TaskTool" $TASKTOOL_PATH $TASKTOOL_BIN_PATH 

buildModule "CommandLineTool" $PARSERTOOL_PATH $PARSERTOOL_BIN_PATH

buildModule "NetworkTester" $NETWORKTESTER_PATH $NETWORKTESTER_BIN_PATH



