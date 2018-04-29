#!/bin/sh

TARGET_DIRECTORY="/home/mickael/Prog/TaskManager/bin/PartialStatic/"
FINAL_LINK_PATH=$TARGET_DIRECTORY"FinalLink"

TOOLS_BUILD_PATH="/home/mickael/Prog/Tools/bin/Synology/"
TOOLSLIB_BIN_PATH=$TOOLS_BUILD_PATH"ToolsLib/"
TOOLSLIB_PATH="/home/mickael/Prog/Tools/ToolsLib/"

TASKMANAGER_BUILD_PATH="/home/mickael/Prog/TaskManager/bin/Synology/"

PARSERSLIB_BIN_PATH1="/home/mickael/Prog/TaskManager/bin/Synology/ReportParsers/"
PARSERSLIB_BIN_PATH=$PARSERSLIB_BIN_PATH1"ParsersLib/"
PARSERSLIB_PATH="/home/mickael/Prog/TaskManager/ReportParsers/ParsersLib/"

TASKLIB_BIN_PATH="/home/mickael/Prog/TaskManager/bin/Synology/TaskLib"
TASKLIB_PATH="/home/mickael/Prog/TaskManager/TaskLib/"

TASKTOOL_BIN_PATH="/home/mickael/Prog/TaskManager/bin/Synology/TaskTool"
TASKTOOL_PATH="/home/mickael/Prog/TaskManager/TaskTool/"


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
	BINPATH=$TARGET_DIRECTORY$NAME
	MAKEFILE=$NAME"Makefile"
	
	createFolderIfNotPresent $TARGET_DIRECTORY
	createFolderIfNotPresent $BINPATH
	cd $BINPATH
	qmake -o $MAKEFILE $LIBPATH/$NAME".pro"
	make -f $MAKEFILE
}

extractModule()
{
	NAME=$1
	PATH=$TARGET_DIRECTORY$NAME"/static"
	cp $PATH"/*.o ./"
}

linkEverything()
{
	createFolderIfNotPresent $FINAL_LINK_PATH
	cd $FINAL_LINK_PATH
	#extractModule "ToolsLib"
	#extractModule "ParsersLib"
	#extractModule "TaskLib"
	ar -c libTaskManager.a *.o	
}

buildModule "ToolsLib" $TOOLSLIB_PATH

buildModule "ParsersLib" $PARSERSLIB_PATH

buildModule "TaskLib" $TASKLIB_PATH

linkEverything
