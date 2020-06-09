#!/bin/sh

source ../../scripts/BuildTools.sh

runCmakeForLinux()
{
	MODE=$1
	version=`cat ../../data/globalVersion.txt`
	cmake ../../proj/cmake -DVERSION=$version -DCMAKE_BUILD_TYPE=$MODE -DUSE_CURL_LIB=TRUE
}
