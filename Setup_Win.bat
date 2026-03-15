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

:: Install CMake

set PACKAGE=cmake
set CMAKE_LINK=https://github.com/Kitware/CMake/releases/download/v4.3.0-rc2/cmake-4.3.0-rc2-windows-x86_64.msi
set OUTPUT_DIR=%CD%\Vendor\win32\temp\cmake

cmake_ --version
if %ERRORLEVEL% neq 0 (
    if not exist "Vendor\win32\cmake\.installed" (
        echo Did not find cmake installation.
        
        call :ValidateContinuation "Enter Y to continue with Cmake installation: "

        mkdir %OUTPUT_DIR%
        call Scripts/win32/Downloader.bat %PACKAGE% %CMAKE_LINK% %OUTPUT_DIR%\cmake-4.3.0-rc2-windows-x86_64.msi
        call Scripts/win32/CmakeInstaller.bat %CD% %OUTPUT_DIR%\cmake-4.3.0-rc2-windows-x86_64.msi %OUTPUT_DIR%\extract

        if !ERRORLEVEL! neq 0 (
            echo Error installing Cmake. %ERRORLEVEL%
            pause
            exit 1
        )
        echo Successfully installed Cmake at "%CD%\Vendor\win32\cmake"
    )

    set "CMAKE_COMMAND="%CD%\Vendor\win32\cmake\bin\cmake.exe""
) else (
    echo Cmake installation found.

    set "CMAKE_COMMAND=cmake"
)

echo Using CMake command: %CMAKE_COMMAND%


:: Install Build System

set PACKAGE=vs-build-system
set VS_LINK=https://aka.ms/vs/stable/vs_BuildTools.exe
set OUTPUT_DIR=%CD%\Vendor\win32\temp\vs

call Scripts/win32/VisualStudioFinder.bat

if %ERRORLEVEL%==1 (
    echo Did not find any compatible Visual Studio IDE or Build Tools installation.

    call :ValidateContinuation "Enter Y to continue with Visual Studio Installer Download: "

    mkdir %OUTPUT_DIR%
    call Scripts/win32/Downloader.bat %PACKAGE% %VS_LINK% %OUTPUT_DIR%\vs_BuildTools.exe
    call Scripts/win32/VisualStudioBuildToolsInstaller.bat %OUTPUT_DIR%\vs_BuildTools.exe

    if !ERRORLEVEL! neq 0 (
        echo Unable to install build system.
        pause
        exit 1
    )

    call "%RETURN_VALUE_1%" -arch=x64

    set "GENERATOR=NMake Makefiles"
) else if %ERRORLEVEL%==2 (
    call "%RETURN_VALUE_1%" -arch=x64

    set "GENERATOR=NMake Makefiles"
) else (
    set "GENERATOR=%RETURN_VALUE_1%"
)

:: Install Mono

set PACKAGE=mono
set MONO_LINK=https://download.mono-project.com/archive/6.12.0/windows-installer/mono-6.12.0-x64-0.msi
set OUTPUT_DIR=%CD%\Vendor\win32\temp\mono

if not exist "Vendor\win32\mono\.installed" (
    call :ValidateContinuation "Enter Y to continue with Mono installation: "

    mkdir %OUTPUT_DIR%
    call Scripts/win32/Downloader.bat %PACKAGE% %MONO_LINK% %OUTPUT_DIR%\mono-6.12.0-x64-0.msi
    call Scripts/win32/MonoInstaller.bat %CD% %OUTPUT_DIR%\mono-6.12.0-x64-0.msi "%OUTPUT_DIR%\extract"

    if %ERRORLEVEL% neq 0 (
        echo Error installing Mono.
        pause
        exit 1
    )
    echo Successfully installed Mono
) else (
    echo Mono already installed
)

rmdir /s /q "%CD%\Vendor\win32\temp"

echo Successfully setup the environment for PhezuEngine, proceeding to build and compile...

:: Build Engine

if not exist "Build" (
    mkdir "Build"
)
cd "Build"

echo Using generator: %GENERATOR%
%CMAKE_COMMAND% .. -G "%GENERATOR%"

if %ERRORLEVEL% neq 0 (
    echo Failed to generate build system files.

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


:ValidateContinuation

if %AUTO_MODE%==1 (
    goto :eof
)

set /p USER_INPUT=%~1

if /i "%USER_INPUT%"=="Y" (
    goto :eof
)

echo Exiting.
pause
exit 0