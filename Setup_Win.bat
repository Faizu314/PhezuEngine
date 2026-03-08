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

set PACKAGE=cmake
set CMAKE_LINK=https://github.com/Kitware/CMake/releases/download/v4.3.0-rc2/cmake-4.3.0-rc2-windows-x86_64.msi
set OUTPUT_DIR=%CD%\Vendor\win32\temp\cmake

cmake_ --version
if %ERRORLEVEL% neq 0 (
    if not exist "Vendor\win32\cmake\.installed" (
        echo Did not find cmake installation. Downloading cmake...
        
        mkdir %OUTPUT_DIR%
        CALL Scripts/win32/Downloader.bat %PACKAGE% %CMAKE_LINK% %OUTPUT_DIR%\cmake-4.3.0-rc2-windows-x86_64.msi
        CALL Scripts/win32/CmakeInstaller.bat %CD% %OUTPUT_DIR%\cmake-4.3.0-rc2-windows-x86_64.msi %OUTPUT_DIR%\extract

        if !ERRORLEVEL! neq 0 (
            echo Error installing Cmake. %ERRORLEVEL%
            pause
            exit 1
        )
        echo Successfully installed Cmake
    )

    set "CMAKE_COMMAND="%CD%\Vendor\win32\cmake\bin\cmake.exe""
) else (
    echo Cmake installation found.

    set "CMAKE_COMMAND="cmake"
)

echo Using CMake command: %CMAKE_COMMAND%

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
set OUTPUT_DIR=%CD%\Vendor\win32\temp\mono

if not exist "Vendor\win32\mono\.installed" (
    mkdir %OUTPUT_DIR%
    CALL Scripts/win32/Downloader.bat %PACKAGE% %MONO_LINK% %OUTPUT_DIR%\mono-6.12.0-x64-0.msi
    CALL Scripts/win32/MonoInstaller.bat %CD% %OUTPUT_DIR%\mono-6.12.0-x64-0.msi "%OUTPUT_DIR%\extract"
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