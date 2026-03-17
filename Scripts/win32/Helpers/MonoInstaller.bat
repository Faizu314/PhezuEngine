@echo off

set ROOT_DIR=%~1
set MONO_MSI_PATH=%~2
set MONO_EXTRACT_DIR=%~3

:: Extract Mono .msi

echo Extracting Mono msi
msiexec /a "%MONO_MSI_PATH%" TARGETDIR="%MONO_EXTRACT_DIR%" /qn

if %ERRORLEVEL% neq 0 (
    echo Error extracting mono
    exit /b 1
)

:: Entire Mono Installation

echo copying mono folder
if not exist "%ROOT_DIR%\Vendor\win32\mono" (
    mkdir "%ROOT_DIR%\Vendor\win32\mono"
)
robocopy "%MONO_EXTRACT_DIR%\Mono" "%ROOT_DIR%\Vendor\win32\mono" /E /COPYALL /NJH /NJS /NC /NS

if %ERRORLEVEL% geq 8 (
    echo Error copying mono files
    exit /b 1
)

:: Mono Include Headers for Phezu

echo copying mono headers
if not exist "%ROOT_DIR%\Phezu\Vendor\include\win32\mono" (
    mkdir "%ROOT_DIR%\Phezu\Vendor\include\win32\mono"
)
robocopy "%MONO_EXTRACT_DIR%\Mono\include\mono-2.0\mono" "%ROOT_DIR%\Phezu\Vendor\include\win32\mono" /E /COPYALL /NJH /NJS /NC /NS

if %ERRORLEVEL% geq 8 (
    echo Error copying mono headers
    exit /b 1
)

:: Mono .dll and .lib for Phezu

if not exist "%ROOT_DIR%\Phezu\Vendor\lib\win32" (
    mkdir "%ROOT_DIR%\Phezu\Vendor\lib\win32"
)

echo copying dll: copy /Y "%MONO_EXTRACT_DIR%\Mono\bin\mono-2.0-sgen.dll" "%ROOT_DIR%\Phezu\Vendor\lib\win32\mono-2.0-sgen.dll"
copy /Y "%MONO_EXTRACT_DIR%\Mono\bin\mono-2.0-sgen.dll" "%ROOT_DIR%\Phezu\Vendor\lib\win32\mono-2.0-sgen.dll"

if %ERRORLEVEL% neq 0 (
    echo Error copying mono sgen dll
    exit /b 1
)

echo copying lib: copy /Y "%MONO_EXTRACT_DIR%\Mono\lib\mono-2.0-sgen.lib" "%ROOT_DIR%\Phezu\Vendor\lib\win32\mono-2.0-sgen.lib"

copy /Y "%MONO_EXTRACT_DIR%\Mono\lib\mono-2.0-sgen.lib" "%ROOT_DIR%\Phezu\Vendor\lib\win32\mono-2.0-sgen.lib"

if %ERRORLEVEL% neq 0 (
    echo Error copying mono sgen lib
    exit /b 1
)

type nul > "%ROOT_DIR%\Vendor\win32\mono\.installed"

exit /b 0