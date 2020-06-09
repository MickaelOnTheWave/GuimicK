#!/bin/bash

TASKSCRIPTS=$PWD"/../TaskTool/scripts"

source "BuildTools.sh"

SCRIPTDIR=$PWD
TASKMANAGERROOT=$SCRIPTDIR"/.."
TOOLSROOT="/home/mickael/Prog/Tools"

BuildSynologyTarget()
{
	cd $1
	prepareForBuild "Synology"
	cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=RELEASE -DUSE_OLD_COMPILER=TRUE -DCMAKE_TOOLCHAIN_FILE=$TASKSCRIPTS/synology.cmake
	build
}

BuildSynologyTaskTool()
{
    version=`cat "$TASKMANAGERROOT/data/globalVersion.txt"`
    cd $TASKMANAGERROOT"/TaskTool/bin"
	prepareForBuild "Synology"
	cmake ../../proj/cmake -DVERSION=$version -DCMAKE_BUILD_TYPE=RELEASE -DUSE_OLD_COMPILER=TRUE -DCMAKE_TOOLCHAIN_FILE=$TASKSCRIPTS/synology.cmake
	build
}


BuildSynologyTaskTool
BuildSynologyTarget $TASKMANAGERROOT"/ReportParsers/CommandLineTool/bin"
BuildSynologyTarget $TOOLSROOT"/NetworkTester/bin"
cd $SCRIPTDIR



