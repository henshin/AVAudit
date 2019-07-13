@setlocal enableextensions enabledelayedexpansion
@echo off
rem this file adds the 256 smallest samples to a given binary file

SET /a count=0

for /f "usebackq delims=|" %%f in (`dir /b "C:\Users\caive\samples\unique_vir\actually_unique\smallest"`) do (
	echo !count!
	call addsec C:\Users\caive\source\repos\leaker\Release\leaker.exe C:\Users\caive\samples\unique_vir\actually_unique\smallest\%%f !count!
	set /a count=count+1
)
endlocal