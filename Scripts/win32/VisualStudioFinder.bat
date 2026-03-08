@echo off

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo vswhere not found at default path.

    where vswhere.exe >nul 2>nul
    if errorlevel 1 (
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
    echo Compatible Visual Studio installation not found. Component: "Desktop development with C++" required.
    exit 2
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
