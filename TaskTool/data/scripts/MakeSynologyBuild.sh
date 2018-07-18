#!/bin/bash

source BuildTools.sh

ROOT="/home/mickael/Prog/"
TARGET_DIRECTORY=$ROOT"TaskManager/bin/SynologyPackage/"

qmakeBuildCommand()
{
	NAME=$1
	LIBPATH=$2
	MAKEFILE=$NAME"Makefile"
	
	qmake -spec linux-arm-synology -o $MAKEFILE $QMAKE_CONFIG $LIBPATH/$NAME".pro"
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


buildAll "Synology" "CONFIG+=synology" $ROOT
copyToDestination

