#!/bin/bash

source BuildTools.sh

SCRIPTDIR=$PWD

prepareForBuild "Synology"
cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=RELEASE -DUSE_OLD_COMPILER=TRUE -DCMAKE_TOOLCHAIN_FILE=$SCRIPTDIR/synology.cmake
build
