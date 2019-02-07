#!/bin/bash

source ../../scripts/BuildTools.sh

prepareForBuild "LinuxGccRelease"
cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=RELEASE
build
