
IF NOT EXIST "%~1" ( 
	mkdir "%~1"
) 
cd %~1
EXIT /B 0
