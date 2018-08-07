#!/bin/sh

createFolderIfNotPresent()
{
	FOLDER=$1	
	if [ ! -d "$FOLDER" ]; then
	mkdir $FOLDER
	fi
}

prepareForBuild()
{
	BUILDNAME=$1
	cd ../bin
	createFolderIfNotPresent $BUILDNAME
	cd $BUILDNAME
}

build()
{
	make -j8
}
