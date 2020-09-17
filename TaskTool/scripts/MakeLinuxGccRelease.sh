#!/bin/bash

source BuildTools.sh

prepareForBuild "LinuxGccRelease"
runCmakeForLinux "RELEASE" $1
build
