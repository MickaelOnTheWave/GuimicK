#!/bin/bash

source BuildTools.sh

prepareForBuild "Synology"
cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=RELEASE -DUSE_OLD_COMPILER=TRUE -DCMAKE_TOOLCHAIN_FILE=/home/mickael/Prog/TaskManager/TaskTool/scripts/synology.cmake
build
