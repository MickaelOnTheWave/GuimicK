cd ../
set ROOT=%CD%

call scripts/CreateAndCd.cmd bin
call ../scripts/CreateAndCd.cmd WindowsInstallPackage

cd %ROOT%\TaskTool\scripts
CALL MakeWin64VsRelease.bat
cd %ROOT%\ConfigurationEditingTool\scripts
CALL MakeWin64ClientRelease.bat
cd %ROOT%\TaskToolLauncher\scripts
CALL MakeWin64VsRelease.bat

cd %ROOT%\bin\WindowsInstallPackage
copy %ROOT%\scripts\WindowsInstallerScript.nsi WindowsInstallerScript.nsi
copy %ROOT%\TaskTool\bin\Win64Vs\Release\TaskTool.exe TaskTool.exe
copy %ROOT%\ConfigurationEditingTool\bin\ClientVersion\Release\ConfigurationEditingTool.exe ConfigurationEditingTool.exe
copy %ROOT%\TaskToolLauncher\bin\Win64Vs\Release\TaskToolLauncher.exe TaskToolLauncher.exe
xcopy %ROOT%\data\toShip /Y

"C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\bin\windeployqt" ConfigurationEditingTool.exe

set /p version=<"%ROOT%\data\globalVersion.txt"

"c:\Program Files (x86)\NSIS\Bin\makensis.exe" -DVERSION=%version% %ROOT%\bin\WindowsInstallPackage\WindowsInstallerScript.nsi

pause
EXIT /B 0