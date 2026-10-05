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
cd /d "%~dp0\..\.."

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

call Scripts\win32\Helpers\SetupCmake.bat

if not exist "Build" (
    mkdir "Build"
)
cd "Build"

call "E:\Users\faiza\CLibraries\emsdk\emsdk_env.bat"

set "CONFIG_ARGS=-DCMAKE_BUILD_TYPE=%BUILD_CONFIG%"

call emcmake %CMAKE_COMMAND% .. -G Ninja "%CONFIG_ARGS%"

%CMAKE_COMMAND% --build . --config %BUILD_CONFIG%

endlocal
pause
exit 0