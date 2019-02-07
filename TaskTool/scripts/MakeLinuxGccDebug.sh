#!/bin/bash

source BuildTools.sh

prepareForBuild "LinuxGccDebug"
runCmakeForLinux "DEBUG"
build
