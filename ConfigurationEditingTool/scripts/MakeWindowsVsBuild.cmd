set generator="Visual Studio 16 2019"

call :CreateAndCd "..\bin"
call :CreateAndCd %~1
cmake ../../proj/cmake -DUSE_CURL_LIB=FALSE %~3 -G %generator% 
cmake --build . --config %~2 -j 8
EXIT /B 0

:CreateAndCd
IF NOT EXIST "%~1" ( 
	mkdir "%~1"
) 
cd %~1
EXIT /B 0
