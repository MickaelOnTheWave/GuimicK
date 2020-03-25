set generator="Visual Studio 16 2019"
set /p version=<"%ROOT%\data\globalVersion.txt"


call ../../scripts/CreateAndCd.cmd "..\bin"
call ../../scripts/CreateAndCd.cmd %~1
cmake ../../proj/cmake "-DVERSION=%version%" %~3 -G %generator% 
cmake --build . --config %~2 -j 8
EXIT /B 0
