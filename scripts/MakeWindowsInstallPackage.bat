cd ../
set ROOT=%CD%

call :CreateAndCd bin
call :CreateAndCd WindowsInstallPackage

cd %ROOT%\TaskTool\scripts
CALL MakeWin64VsRelease.bat
cd %ROOT%\ConfigurationEditingTool\scripts
CALL MakeWin64VsRelease.bat

cd %ROOT%\bin\WindowsInstallPackage
copy %ROOT%\scripts\WindowsInstallerScript.nsi WindowsInstallerScript.nsi
copy %ROOT%\TaskTool\bin\Win64Vs\Release\TaskTool.exe TaskTool.exe
copy %ROOT%\ConfigurationEditingTool\bin\Win64Vs\Release\ConfigurationEditingTool.exe ConfigurationEditingTool.exe
xcopy %ROOT%\data /Y
xcopy %ROOT%\lib /Y

windeployqt ConfigurationEditingTool.exe

"c:\Program Files (x86)\NSIS\Bin\makensis.exe" %ROOT%\bin\WindowsInstallPackage\WindowsInstallerScript.nsi

pause

:CreateAndCd
IF NOT EXIST "%~1" ( 
	mkdir "%~1"
) 
cd %~1
EXIT /B 0