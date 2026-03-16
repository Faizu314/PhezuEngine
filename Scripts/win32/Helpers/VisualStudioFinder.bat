@echo off

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo vswhere not found at default path.

    where vswhere.exe >nul 2>nul
    if ERRORLEVEL 1 (
        echo vswhere not found in PATH.
        exit /b 1
    ) else (
	echo vswhere found in PATH variable.
        set "VSWHERE=vswhere.exe"
    )
)

for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Workload.NativeDesktop -property installationPath`) do (
    set "VSINSTALL=%%i"
)

if "%VSINSTALL%"=="" (
    echo Compatible Visual Studio IDE installation not found. Component: "Desktop development with C++" required.

    for /f "delims=" %%i in ('"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Product.BuildTools -requires Microsoft.VisualStudio.Workload.VCTools -property installationPath') do (
        set "BUILD_TOOLS_PATH=%%i"
    )

    if defined BUILD_TOOLS_PATH (
        echo Visual Studio Build Tools found at !BUILD_TOOLS_PATH!
        set "RETURN_VALUE_1=!BUILD_TOOLS_PATH!\Common7\Tools\VsDevCmd.bat"
        exit /b 2
    ) else (
        echo Visual Studio Build Tools with required components not installed
        exit /b 1
    )
)

for /f "delims=. tokens=1" %%v in ('"%VSWHERE%" -latest -property installationVersion') do (
    set "VS_MAJOR=%%v"
)

if "%VS_MAJOR%"=="18" set "VS_YEAR=2026"
if "%VS_MAJOR%"=="17" set "VS_YEAR=2022"
if "%VS_MAJOR%"=="16" set "VS_YEAR=2019"
if "%VS_MAJOR%"=="15" set "VS_YEAR=2017"

set "RETURN_VALUE_1=Visual Studio %VS_MAJOR% %VS_YEAR%"

echo Compatible Visual Studio installation found: %RETURN_VALUE_1%

exit /b 0
