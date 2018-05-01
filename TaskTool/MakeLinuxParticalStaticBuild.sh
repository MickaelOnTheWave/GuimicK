#!/bin/sh

TARGET_DIRECTORY="/home/mickael/Prog/TaskManager/bin/PartialStatic/"
FINAL_LINK_PATH=$TARGET_DIRECTORY"FinalLink"

TOOLSLIB_PATH="/home/mickael/Prog/Tools/ToolsLib/"

TASKMANAGER_BUILD_PATH="/home/mickael/Prog/TaskManager/bin/Synology/"

PARSERSLIB_PATH="/home/mickael/Prog/TaskManager/ReportParsers/ParsersLib/"

TASKLIB_PATH="/home/mickael/Prog/TaskManager/TaskLib/"

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

buildTaskModule()
{
	NAME="TaskLib"
	LIBPATH=$TASKLIB_PATH
	BINPATH=$TARGET_DIRECTORY$NAME
	MAKEFILE=$NAME"Makefile"
	
	createFolderIfNotPresent $TARGET_DIRECTORY
	createFolderIfNotPresent $BINPATH
	cd $BINPATH
	
	TOOLSBINPATH=$TARGET_DIRECTORY"ToolsLib"
	PARSERSBINPATH=$TARGET_DIRECTORY"ParsersLib"
	qmake -o $MAKEFILE "LIBS += -L$TOOLSBINPATH -L$PARSERSBINPATH" $LIBPATH/$NAME".pro"
	make -f $MAKEFILE
}

extractModule()
{
	NAME=$1
	PATH=$TARGET_DIRECTORY$NAME"/static"
	/bin/cp $PATH/*.o ./
}

linkEverything()
{
	createFolderIfNotPresent $FINAL_LINK_PATH
	cd $FINAL_LINK_PATH
	extractModule "ToolsLib"
	extractModule "ParsersLib"
	extractModule "TaskLib"
	/usr/bin/ar -c libTaskManager.a *.o	
}

buildModule "ToolsLib" $TOOLSLIB_PATH

buildModule "ParsersLib" $PARSERSLIB_PATH

buildTaskModule

linkEverything
