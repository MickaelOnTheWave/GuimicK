#!/bin/sh

createFolderIfNotPresent()
{
	FOLDER=$1	
	if [ ! -d "$FOLDER" ]; then
	mkdir $FOLDER
	fi
}

removeFolderIfPresent()
{
	FOLDER=$1	
	if [ -d "$FOLDER" ]; then
	rm -Rf $FOLDER
	fi
}

cdAndCreateIfNotPresent()
{
    createFolderIfNotPresent $1
	cd $1
}

prepareForBuild()
{
	cdAndCreateIfNotPresent ../bin
	cdAndCreateIfNotPresent $1
}

build()
{
	make -j8
}
