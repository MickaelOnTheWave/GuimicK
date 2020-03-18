set buildname="ClientVersion"
set buildtype="Release"
set flags="-DCLIENT_VERSION=TRUE"

call ../../scripts/MakeWindowsVsBuild.cmd %buildname% %buildtype% %flags%
