#!/bin/bash

source BuildTools.sh

SCRIPTDIR=$PWD
prepareForBuild "WindowsRelease"
cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=RELEASE -DUSE_CURL_LIB=FALSE -DCMAKE_TOOLCHAIN_FILE=$SCRIPTDIR/win64.cmake
build