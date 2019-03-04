;--------------------------------

!define VERSION "0.9.4"
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
  
  ; Put file there
  File TaskTool.exe
  File 7z.exe
  File ConfigurationEditingTool.exe
  File Qt5Core.dll
  File Qt5Gui.dll
  File Qt5Widgets.dll

  ; Start menu shortcuts
  CreateDirectory "$SMPROGRAMS\TaskManager"
  CreateShortcut "$SMPROGRAMS\TaskManager\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortcut "$SMPROGRAMS\TaskManager\TaskTool.lnk" "$INSTDIR\TaskTool.exe" "" "$INSTDIR\TaskTool.exe" 0
  CreateShortcut "$SMPROGRAMS\TaskManager\Configuration Editor.lnk" "$INSTDIR\ConfigurationEditingTool.exe" "" "$INSTDIR\ConfigurationEditingTool.exe" 0
  
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
  
SectionEnd ; end the section

Section "Uninstall"
  
  Delete $INSTDIR\TaskTool.exe
  Delete $INSTDIR\7z.exe
  Delete $INSTDIR\ConfigurationEditingTool.exe
  Delete $INSTDIR\Qt5Core.dll
  Delete $INSTDIR\Qt5Gui.dll
  Delete $INSTDIR\Qt5Widgets.dll
  Delete $INSTDIR\uninstall.exe

  RMDir "$INSTDIR"
  
  ; Remove Start menu shortcuts
  Delete "$SMPROGRAMS\TaskManager\*.*"
  RMDir "$SMPROGRAMS\TaskManager"  
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\App Paths\TaskTool.exe"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\TaskManager"
  DeleteRegKey HKLM "Software\TaskManager"
  
SectionEnd
