@echo off

if /i "%~1"=="--auto" (
    set AUTO_MODE=1
) else (
    set AUTO_MODE=0
)

set "CMAKE_COMMAND="%CD%\Vendor\Windows\CMake\bin\cmake.exe""

echo Using CMake command: %CMAKE_COMMAND%

%CMAKE_COMMAND% --version
if %ERRORLEVEL% neq 0 (
    echo Error: Something went wrong with the provided CMake executable.
    pause
    exit 1
)

set BUILD_COMMAND=%CMAKE_COMMAND% --build . --config Release

:: Find Visual Studio installation path

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo vswhere not found at default path.

    where vswhere.exe >nul 2>nul
    if errorlevel 1 (
        echo vswhere not found in PATH.
	pause
        exit 1
    ) else (
	echo vswhere found in PATH variable.
        set "VSWHERE=vswhere.exe"
    )
)

for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -property installationPath`) do (
    set "VSINSTALL=%%i"
)

if "%VSINSTALL%"=="" (
    echo Visual Studio installation not found.
    pause
    exit 1
)

for /f "delims=. tokens=1" %%v in ('"%VSWHERE%" -latest -property installationVersion') do (
    set "VS_MAJOR=%%v"
)

if "%VS_MAJOR%"=="17" set "VS_YEAR=2022"
if "%VS_MAJOR%"=="16" set "VS_YEAR=2019"
if "%VS_MAJOR%"=="15" set "VS_YEAR=2017"

set "GENERATOR=Visual Studio %VS_MAJOR% %VS_YEAR%"

echo Compatible Visual Studio installation found: %GENERATOR%

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

%BUILD_COMMAND%

if %ERRORLEVEL% neq 0 (
    echo Build failed. Please check the errors above.
    pause
    exit 1
)

pause
exit 0