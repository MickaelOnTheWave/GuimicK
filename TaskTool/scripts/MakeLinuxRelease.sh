#!/bin/bash

source BuildTools.sh

prepareForBuild "LinuxRelease"
cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=RELEASE
build
