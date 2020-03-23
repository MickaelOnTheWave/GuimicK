set generator="Visual Studio 16 2019"

call CreateAndCd.cmd "..\bin"
call CreateAndCd.cmd %~1
cmake ../../proj/cmake %~3 -G %generator% 
cmake --build . --config %~2 -j 8
EXIT /B 0
