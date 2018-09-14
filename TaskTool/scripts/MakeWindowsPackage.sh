#!/bin/bash

source BuildTools.sh

SCRIPTDIR=$PWD

PACKAGE_FOLDER="WindowsPackage"
TASKMANAGER_ROOT=$PWD/../..
CONFTOOL_BIN_FOLDER=$TASKMANAGER_ROOT/ConfigurationEditingTool/bin

buildConfigurationEditor()
{
	cd $CONFTOOL_BIN_FOLDER
	createFolderIfNotPresent Release
	cd Release
	cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$SCRIPTDIR/win64.cmake
	make -j8
}

createPackageStructure()
{
	cd $TASKMANAGER_ROOT/TaskTool/bin
	removeFolderIfPresent $PACKAGE_FOLDER
	mkdir $PACKAGE_FOLDER
}

copyFilesToStructure()
{
	cp WindowsRelease/TaskTool.exe $PACKAGE_FOLDER/TaskmanagerTool.exe
	cp $CONFTOOL_BIN_FOLDER/WindowsRelease/ConfigurationEditingTool.exe $PACKAGE_FOLDER/TaskmanagerEditor.exe
}

./MakeWindowsRelease.sh
buildConfigurationEditor
createPackageStructure
copyFilesToStructure

