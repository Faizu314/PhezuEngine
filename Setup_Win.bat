@echo off
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

set "CMAKE_COMMAND="%CD%\Vendor\win32\cmake\bin\cmake.exe""

echo Using CMake command: %CMAKE_COMMAND%

%CMAKE_COMMAND% --version
if %ERRORLEVEL% neq 0 (
    echo Error: Something went wrong with the provided CMake executable.
    pause
    exit 1
)

CALL Scripts/win32/VisualStudioFinder.bat

if %ERRORLEVEL% neq 0 (
    echo Error: Did not find any compatible Visual Studio IDE installation.
    pause
    exit 1
) else (
    set "GENERATOR=%RETURN_VALUE_1%"
)

:: Install Mono

set PACKAGE=mono
set MONO_LINK=https://download.mono-project.com/archive/6.12.0/windows-installer/mono-6.12.0-x64-0.msi
set OUTPUT_DIR=%CD%\Vendor\win32\temp

if not exist "Vendor/win32/mono/.installed" (
    mkdir %OUTPUT_DIR%
    CALL Scripts/win32/Downloader.bat %PACKAGE% %MONO_LINK% %OUTPUT_DIR%\mono-6.12.0-x64-0.msi
    CALL Scripts/win32/MonoInstaller.bat %CD% %OUTPUT_DIR%\mono-6.12.0-x64-0.msi "%CD%\Vendor\win32\temp\extract"
    if %ERRORLEVEL% neq 0 (
        echo Error installing Mono.
        pause
        exit 1
    )
    echo Successfully installed Mono
) else (
    echo Mono already installed
)

if %AUTO_MODE%==1 (
    set USER_INPUT=Y
) else (
    set /p USER_INPUT="Build Visual Studio solution files? (Y/N) "
)

if /i "%USER_INPUT%"=="N" (
    echo Exiting.
    pause
    exit 1
)

if not exist "Build" (
    mkdir "Build"
)
cd "Build"

echo Using generator: %GENERATOR%
%CMAKE_COMMAND% .. -G "%GENERATOR%"

if %ERRORLEVEL% neq 0 (
    echo Failed to generate visual studio files.

    pause
    exit 1
)

%CMAKE_COMMAND% --build . --config %BUILD_CONFIG%

if %ERRORLEVEL% neq 0 (
    echo Build failed. Please check the errors above.
    pause
    exit 1
)

pause
exit 0