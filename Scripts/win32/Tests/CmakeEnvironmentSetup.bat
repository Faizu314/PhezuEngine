set CMAKE_ENV=%1

echo "Setting up CMake Environment: %CMAKE_ENV%"

if "%CMAKE_ENV%"=="--system" (

	echo CMake already installed...
	exit /b 0

)

echo Hiding CMake installation path...
where cmake

set "PATH=%PATH:C:\Program Files\CMake\bin;=%"
set "PATH=%PATH:C:\Program Files (x86)\CMake\bin;=%"
set "PATH=%PATH:C:\Program Files\CMake\bin=%"
set "PATH=%PATH:C:\Program Files (x86)\CMake\bin=%"

where cmake >nul 2>&1
if ERRORLEVEL 1 (
    echo CMake is now hidden
) else (
    echo CMake is still found
    exit 1
)

if "%CMAKE_ENV%"=="internal" (

	echo Installing internal cmake...
	call Scripts\win32\Helpers\SetupCmake.bat

)

echo CMake Environment Setup Complete
exit /b 0