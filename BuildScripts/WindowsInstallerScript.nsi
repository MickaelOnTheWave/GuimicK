; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

!define VERSION "0.9.1"
; The name of the installer
Name "Task Manager ${VERSION}"

; The file to write
OutFile "..\TaskManagerSetup-${VERSION}.exe"

; The default installation directory
InstallDir $PROGRAMFILES32\TaskManager

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
  File ConfigurationEditingTool.exe
  File libstdc++-6.dll
  File libwinpthread-1.dll
  File Qt5Core.dll
  File Qt5Gui.dll
  File Qt5Widgets.dll
  
SectionEnd ; end the section
