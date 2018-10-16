
cd ../bin
IF NOT EXIST WindowsRelease (
	mkdir WindowsRelease
)
cd ../bin/WindowsRelease

cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=RELEASE -DUSE_CURL_LIB=FALSE -G "MinGW Makefiles"


REM -DCMAKE_TOOLCHAIN_FILE=../../scripts/winNative.cmake
