cd ../
set ROOT=%CD%

IF NOT EXIST bin (
	mkdir bin
)
cd bin

IF NOT EXIST WindowsInstallPackage (
	mkdir WindowsInstallPackage
)
cd WindowsInstallPackage

cd %ROOT%\TaskTool\scripts
CALL MakeWin64VsRelease.bat
cd %ROOT%\ConfigurationEditingTool\scripts
CALL BuildWin64VsRelease.bat

cd %ROOT%\bin\WindowsInstallPackage
copy %ROOT%\BuildScripts\WindowsInstallerScript.nsi WindowsInstallerScript.nsi
copy %ROOT%\TaskTool\bin\Win64VsRelease\Release\TaskTool.exe TaskTool.exe
copy %ROOT%\ConfigurationEditingTool\bin\Win64VsRelease\Release\ConfigurationEditingTool.exe ConfigurationEditingTool.exe
copy %ROOT%\data\license.txt license.txt
xcopy %ROOT%\lib /Y
"c:\Program Files (x86)\NSIS\Bin\makensis.exe" %ROOT%\bin\WindowsInstallPackage\WindowsInstallerScript.nsi

pause