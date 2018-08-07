#!/bin/bash

source BuildTools.sh

prepareForBuild "Synology"
cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=RELEASE
build
