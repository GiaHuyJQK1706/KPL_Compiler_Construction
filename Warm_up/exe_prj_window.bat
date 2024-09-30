@echo off
mingw32-make
if %ERRORLEVEL% NEQ 0 (
    echo Makefile execution failed.
    exit /b %ERRORLEVEL%
)
.\Warm_up
pause
