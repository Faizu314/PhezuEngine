setlocal enabledelayedexpansion

set PRODUCT_ID=%1
set COMPONENT_ID=%2
set ACTION=%3

if "%PRODUCT_ID%"=="" (
    echo Usage: script.bat PRODUCT_ID COMPONENT_ID [add^|remove]
    exit /b 1
)

set "VSWHERE=C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
set "INSTALLER=C:\Program Files (x86)\Microsoft Visual Studio\Installer\setup.exe"

if not exist "%VSWHERE%" (
    echo vswhere not found
    exit /b 1
)

echo Searching for product: %PRODUCT_ID%

if "%COMPONENT_ID%"=="" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -products %PRODUCT_ID% -property installationPath`) do (

        set "INSTALL_PATH=%%i"
        echo Found installation: !INSTALL_PATH!

        echo Removing product
        "%INSTALLER%" uninstall ^
            --installPath "!INSTALL_PATH!" ^
            --passive --norestart
    )

    if not defined INSTALL_PATH (
        echo Product installation not found
    )

    echo Done
    endlocal
    exit /b 0
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -products %PRODUCT_ID% -requires %COMPONENT_ID% -property installationPath`) do (

    set "INSTALL_PATH=%%i"
    echo Found installation: !INSTALL_PATH!

    echo Removing component %COMPONENT_ID%
    "%INSTALLER%" modify ^
        --installPath "!INSTALL_PATH!" ^
        --remove %COMPONENT_ID% ^
        --passive --norestart
)

if not defined INSTALL_PATH (
    echo Product installation not found
)

echo Done
endlocal
exit /b 0