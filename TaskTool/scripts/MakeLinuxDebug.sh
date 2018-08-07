#!/bin/bash

source BuildTools.sh

prepareForBuild "LinuxDebug"
cmake ../../proj/cmake
build
