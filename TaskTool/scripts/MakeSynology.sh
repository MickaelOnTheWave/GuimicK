#!/bin/bash

source BuildTools.sh

version=`cat ../../data/globalVersion.txt`

SCRIPTDIR=$PWD
prepareForBuild "Synology"
cmake ../../proj/cmake -DVERSION=$version -DCMAKE_BUILD_TYPE=RELEASE -DUSE_OLD_COMPILER=TRUE -DCMAKE_TOOLCHAIN_FILE=$SCRIPTDIR/synology.cmake
build
