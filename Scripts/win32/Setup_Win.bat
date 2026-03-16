@echo off
setlocal enabledelayedexpansion
title Phezu Engine Installer

net session >nul 2>&1
if %errorlevel% neq 0 (
    echo Requesting administrative privileges...
    powershell -Command "Start-Process '%~f0' -WorkingDirectory '%CD%' -Verb runAs"
    exit /b
)

echo Running as admin
cd /d "%~dp0"

if /i "%~1"=="--auto" (
    set AUTO_MODE=1
) else (
    set AUTO_MODE=0
)
if "%~2"=="--Release" (
    set BUILD_CONFIG=Release
) else if "%~2"=="--Debug" (
    set BUILD_CONFIG=Debug
) else if "%~2"=="" (
    set BUILD_CONFIG=Release
)

call Scripts\win32\SetupCmake.bat
call Scripts\win32\SetupBuildSystem.bat
call Scripts\win32\SetupMono.bat

rmdir /s /q "%CD%\Vendor\win32\temp"
echo Successfully setup the environment for PhezuEngine, proceeding to build and compile...

call Scripts\win32\BuildEngine.bat

pause
exit 0


:ValidateContinuation

if %AUTO_MODE%==1 (
    goto :eof
)

set /p USER_INPUT=%~1

if /i "%USER_INPUT%"=="Y" (
    goto :eof
)

endlocal
echo Exiting.
pause
exit 0