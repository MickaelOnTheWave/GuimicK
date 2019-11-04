;--------------------------------

!define VERSION "1.0.13"
; The name of the installer
Name "Task Manager ${VERSION}"

; The file to write
OutFile "..\TaskManagerSetup-${VERSION}.exe"

; The default installation directory
InstallDir $PROGRAMFILES64\TaskManager

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\TaskManager" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

PageEx license
	LicenseData "license.txt"
	LicenseForceSelection checkbox "I Accept"
PageExEnd

Page directory
Page instfiles

;--------------------------------

; The stuff to install
Section "" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  EnVar::SetHKLM
  
  ; Put file there
  File 7z.exe
  File 7z.dll
  File ConfigurationEditingTool.exe
  File msvcp140.dll
  File vcruntime140.dll
  File Qt5Core.dll
  File Qt5Gui.dll
  File Qt5Widgets.dll
  File TaskTool.exe  
  File license.txt  
  File report.css
  File /r "iconengines"
  File /r "imageformats"
  File /r "platforms"
  File /r "styles"
  File /r "translations"

  ; Start menu shortcuts
  CreateDirectory "$SMPROGRAMS\TaskManager"
  CreateShortcut "$SMPROGRAMS\TaskManager\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\TaskManager\Configure.lnk" "$INSTDIR\ConfigurationEditingTool.exe" "" "$INSTDIR\ConfigurationEditingTool.exe" 0
  
  ; Reg Keys
  WriteRegStr HKLM "Software\TaskManager" "Install_Dir" "$INSTDIR"
  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\App Paths\TaskTool.exe" "" "$INSTDIR\TaskTool.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\App Paths\TaskTool.exe" "Path" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskManager" "DisplayName" "Task Manager"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskManager" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskManager" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskManager" "NoRepair" 1  
  
  WriteUninstaller "uninstall.exe"
  EnVar::AddValue "Path" "$INSTDIR"  
SectionEnd ; end the section

Section "Uninstall"
  
  RMDir /r $INSTDIR

  ; Remove Start menu shortcuts
  Delete "$SMPROGRAMS\TaskManager\*.*"
  RMDir "$SMPROGRAMS\TaskManager"  
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\App Paths\TaskTool.exe"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskManager"
  DeleteRegKey HKLM "Software\TaskManager"
  DeleteRegKey HKCU "Software\Task Manager"
  
SectionEnd
