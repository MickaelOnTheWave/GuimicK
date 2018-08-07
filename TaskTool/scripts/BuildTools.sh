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

runCmakeForLinux()
{
	MODE=$1
	cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=$MODE -DUSE_CURL_LIB=TRUE
}

build()
{
	make -j8
}
