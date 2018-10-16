
cd ../bin
IF NOT EXIST WindowsInstallPackage (
	mkdir WindowsInstallPackage
)
cd ../bin/WindowsInstallPackage

REM call command-line build for tasktool and editor
copy ..\..\BuildScripts\WindowsInstallerScript.nsi WindowsInstallerScript.nsi
copy ..\..\TaskTool\bin\Release\TaskTool.exe TaskTool.exe
copy ..\..\ConfigurationEditingTool\bin\Release\ConfigurationEditingTool.exe ConfigurationEditingTool.exe
copy ..\..\data\license.txt license.txt
xcopy ..\..\lib
"c:\Program Files (x86)\NSIS\Bin\makensis.exe" WindowsInstallerScript.nsi