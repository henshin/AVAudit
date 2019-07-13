@setlocal enableextensions enabledelayedexpansion
@echo off

for /f "usebackq delims=|" %%f in (`dir /b "C:\Users\caive\samples\defender\smallest_samples"`) do (
	call C:\PROGRA~1\WINDOW~1\MpCmdRun.exe -Scan -DisableRemediation -ScanType 3 -File %%f
	set /a count=count+1
)
endlocal