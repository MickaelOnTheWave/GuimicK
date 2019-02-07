#!/bin/bash

source BuildTools.sh

prepareForBuild "LinuxGccRelease"
runCmakeForLinux "RELEASE"
build
