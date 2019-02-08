
set buildname="Win64VsRelease"

IF NOT EXIST ..\bin (
	mkdir ..\bin
)
cd ..\bin

IF NOT EXIST %buildname% (
	mkdir %buildname%
)
cd %buildname%

cmake ../../proj/cmake -G "Visual Studio 15 2017 Win64"
cmake --build . --config Release -j 8
