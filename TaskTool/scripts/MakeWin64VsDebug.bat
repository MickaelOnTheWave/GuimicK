set buildname="Win64Vs"

call :CreateAndCd "..\bin"
call :CreateAndCd %buildname%

cmake ../../proj/cmake -DUSE_CURL_LIB=FALSE -G "Visual Studio 15 2017 Win64"
cmake --build . --config Debug -j 8

:CreateAndCd
IF NOT EXIST "%~1" ( 
	mkdir "%~1"
) 
cd %~1
EXIT /B 0