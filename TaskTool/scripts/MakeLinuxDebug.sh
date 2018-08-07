#!/bin/bash

source BuildTools.sh

prepareForBuild "LinuxDebug"
runCmakeForLinux "DEBUG"
build
