set buildname="ClientVersion"
set buildtype="Release"
set flags="-DCLIENT_VERSION"

call MakeWindowsVsBuild.cmd %buildname% %buildtype% %flags%
