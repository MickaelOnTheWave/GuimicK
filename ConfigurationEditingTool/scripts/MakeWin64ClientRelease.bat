set buildname="ClientVersion"
set buildtype="Release"
set flags="-DCLIENT_VERSION=TRUE"

call MakeWindowsVsBuild.cmd %buildname% %buildtype% %flags%
