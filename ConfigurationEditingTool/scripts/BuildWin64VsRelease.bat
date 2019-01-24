
set buildname="Win64VsRelease"

cd ../bin
IF NOT EXIST %buildname% (
	mkdir %buildname%
)
cd ../bin/%buildname%

cmake ../../proj/cmake -DCMAKE_BUILD_TYPE=RELEASE -G "Visual Studio 15 2017 Win64"
cmake --build . -j 8
