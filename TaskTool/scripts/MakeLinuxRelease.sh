#!/bin/bash

source BuildTools.sh

prepareForBuild "LinuxRelease"
runCmakeForLinux "RELEASE"
build
