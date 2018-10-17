#!/bin/bash

TASKSCRIPTS="$PWD/../TaskTool/scripts"

source "$TASKSCRIPTS/BuildTools.sh"

TASKMANAGER_VERSION=1.0
PACKAGE_VERSION=1
PACKAGE_NAME=taskmanager_$TASKMANAGER_VERSION-$PACKAGE_VERSION

TASKMANAGER_ROOT=$PWD/..
BIN_FOLDER=$PACKAGE_NAME/usr/local/bin
CONTROL_FOLDER=$PACKAGE_NAME/DEBIAN
TASKTOOL_BIN_FOLDER=$TASKMANAGER_ROOT/TaskTool/bin
CONFTOOL_BIN_FOLDER=$TASKMANAGER_ROOT/ConfigurationEditingTool/bin

buildTaskTool()
{
	CURRENTFOLDER=$PWD
	cd $TASKSCRIPTS
	./MakeLinuxRelease.sh
	cd $CURRENTFOLDER
}

buildConfigurationEditor()
{
	cd $CONFTOOL_BIN_FOLDER
	createFolderIfNotPresent Release
	cd Release
	cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=Release
	make -j8
}

createPackageStructure()
{
	cd $TASKMANAGER_ROOT/bin
	removeFolderIfPresent $PACKAGE_NAME
	mkdir $PACKAGE_NAME
	mkdir $PACKAGE_NAME/usr
	mkdir $PACKAGE_NAME/usr/local
	mkdir $BIN_FOLDER
	mkdir $CONTROL_FOLDER
}

copyFilesToStructure()
{
	cp $TASKTOOL_BIN_FOLDER/LinuxRelease/TaskTool $BIN_FOLDER/taskmanagerTool
	cp $CONFTOOL_BIN_FOLDER/Release/ConfigurationEditingTool $BIN_FOLDER/taskmanagerEditor
}

createDebianControlFile()
{
	CONTROL_FILE=$CONTROL_FOLDER/control
	
	echo "Package: taskmanager" >> $CONTROL_FILE
	echo "Version: $TASKMANAGER_VERSION-$PACKAGE_VERSION" >> $CONTROL_FILE
	echo "Section: base" >> $CONTROL_FILE
	echo "Priority: extra" >> $CONTROL_FILE
	echo "Architecture: i386" >> $CONTROL_FILE
	#echo "Depends: libsomethingorrather (>= 1.2.13), anotherDependency (>= 1.2.6)" >> $CONTROL_FILE
	echo "Maintainer: Mickael C. Guimaraes <mickael.dacruz@gmail.com>" >> $CONTROL_FILE
	echo "Description: Task Manager" >> $CONTROL_FILE
	echo " A Task Manager that can run tasks when you're not there" >> $CONTROL_FILE
	echo " and create a report about what happened :-)" >> $CONTROL_FILE
}

createPackage()
{
	dpkg-deb --build $PACKAGE_NAME
}

buildTaskTool
buildConfigurationEditor
createPackageStructure
copyFilesToStructure
createDebianControlFile
createPackage
