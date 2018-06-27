#!/bin/sh

setupGlobalVars()
{
	TARGET_TYPE=$1
	PROG_ROOT=$2

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
}

createFolderIfNotPresent()
{
	FOLDER=$1	
	if [ ! -d "$FOLDER" ]; then
	mkdir $FOLDER
	fi
}

qmakeBuildCommand()
{
	NAME=$1
	LIBPATH=$2
	MAKEFILE=$NAME"Makefile"
	BUILDVAR="BUILD_TYPE=$TARGET_TYPE"
	
	qmake -o $MAKEFILE $QMAKE_CONFIG $LIBPATH/$NAME".pro" $BUILDVAR
}

buildModule()
{
	NAME=$1
	LIBPATH=$2
	BINPATH=$3
	MAKEFILE=$NAME"Makefile"
	
	createFolderIfNotPresent $BINPATH
	cd $BINPATH
	qmakeBuildCommand $NAME $LIBPATH
	make -f $MAKEFILE
}

buildAll()
{
	TARGET_TYPE=$1
	QMAKE_CONFIG=$2
	PROG_ROOT=$3
	
	setupGlobalVars $TARGET_TYPE $PROG_ROOT
	
	createFolderIfNotPresent $TOOLS_BUILD_PATH
	createFolderIfNotPresent $TASKMANAGER_BUILD_PATH
	buildModule "ToolsLib" $TOOLSLIB_PATH $TOOLSLIB_BIN_PATH

	createFolderIfNotPresent $PARSERSLIB_BIN_PATH_ROOT
	buildModule "ParsersLib" $PARSERSLIB_PATH $PARSERSLIB_BIN_PATH
	buildModule "TaskLib" $TASKLIB_PATH $TASKLIB_BIN_PATH
	buildModule "TaskTool" $TASKTOOL_PATH $TASKTOOL_BIN_PATH 
	buildModule "CommandLineTool" $PARSERTOOL_PATH $PARSERTOOL_BIN_PATH
	buildModule "NetworkTester" $NETWORKTESTER_PATH $NETWORKTESTER_BIN_PATH
}

buildAllWithNetworkLib()
{
	buildAll $1 $2 $3
	buildModule "NetworkToolsLib" $NETWORKTOOLSLIB_PATH $NETWORKTOOLSLIB_BIN_PATH
}




