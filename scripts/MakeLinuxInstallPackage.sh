#!/bin/bash

source "BuildTools.sh"

TASKMANAGER_ROOT=$PWD"/.."
version=`cat "$TASKMANAGER_ROOT/data/globalVersion.txt"`

TASKMANAGER_VERSION=$version
PACKAGE_VERSION=1
PACKAGE_NAME=taskmanager_$TASKMANAGER_VERSION-$PACKAGE_VERSION

BIN_FOLDER=$PACKAGE_NAME/usr/local/bin
CONTROL_FOLDER=$PACKAGE_NAME/DEBIAN
TASKTOOL_BIN_FOLDER=$TASKMANAGER_ROOT/TaskTool/bin
CONFTOOL_BIN_FOLDER=$TASKMANAGER_ROOT/ConfigurationEditingTool/bin

buildSubProject()
{
	CURRENTFOLDER=$PWD
	PROJECTSCRIPTS="$PWD/../$1/scripts"
	cd $PROJECTSCRIPTS
	./MakeLinuxGccRelease.sh
	cd $CURRENTFOLDER
}

createPackageStructure()
{
    cdAndCreateIfNotPresent $TASKMANAGER_ROOT/bin
	removeFolderIfPresent $PACKAGE_NAME
	mkdir $PACKAGE_NAME
	mkdir $PACKAGE_NAME/usr
	mkdir $PACKAGE_NAME/usr/local
	mkdir $BIN_FOLDER
	mkdir $CONTROL_FOLDER
}

copyFilesToStructure()
{
	cp $TASKTOOL_BIN_FOLDER/LinuxGccRelease/TaskTool $BIN_FOLDER/taskmanagerTool
	cp $CONFTOOL_BIN_FOLDER/LinuxGccRelease/ConfigurationEditingTool $BIN_FOLDER/taskmanagerEditor
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

buildSubProject "TaskTool"
buildSubProject "ConfigurationEditingTool"
createPackageStructure
copyFilesToStructure
createDebianControlFile
createPackage
