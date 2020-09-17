#!/bin/bash

source BuildTools.sh

prepareForBuild "LinuxGccDebug"
runCmakeForLinux "DEBUG" $1
build
