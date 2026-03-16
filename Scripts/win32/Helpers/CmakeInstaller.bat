@echo off

set ROOT_DIR=%~1
set CMAKE_MSI_PATH=%~2
set CMAKE_EXTRACT_DIR=%~3

:: Extract Cmake .msi

echo Extracting Cmake msi
msiexec /a "%CMAKE_MSI_PATH%" TARGETDIR="%CMAKE_EXTRACT_DIR%" /qn

if %ERRORLEVEL% neq 0 (
    echo Error extracting cmake
    exit /b 1
)

:: Entire Mono Installation

echo copying cmake folder
mkdir "%ROOT_DIR%\Vendor\win32\cmake"
robocopy "%CMAKE_EXTRACT_DIR%\PFiles64\CMake" "%ROOT_DIR%\Vendor\win32\cmake" /E /COPYALL /NJH /NJS /NC /NS

if %ERRORLEVEL% geq 8 (
    echo Error copying cmake files
    exit /b 1
)

%ROOT_DIR%\Vendor\win32\cmake\bin\cmake.exe --version
if %ERRORLEVEL% neq 0 (
    echo Error: Something went wrong with the installed CMake executable.
    exit /b 1
)

type nul > "%ROOT_DIR%\Vendor\win32\cmake\.installed"

exit /b 0