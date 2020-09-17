#!/bin/sh

source ../../scripts/BuildTools.sh

runCmakeForLinux()
{
	MODE=$1
	cmake ../../proj/cmake -DVERSION=$2 -DCMAKE_BUILD_TYPE=$MODE -DUSE_CURL_LIB=TRUE
}
