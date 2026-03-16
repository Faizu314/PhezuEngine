@echo off

set INSTALLER_PATH=%~1

echo Script will continue setup after visual studio installation finishes.

echo Updating Visual Studio Installer...

"%INSTALLER_PATH%" --update --quiet --wait

if %ERRORLEVEL% NEQ 0 (
	echo Something went wrong updating visual studio installer: %ERRORLEVEL%
	exit /b 1
)

for /f "delims=" %%i in ('"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Product.BuildTools -property installationPath') do (
    set "BUILD_TOOLS_PATH=%%i"
)

if defined BUILD_TOOLS_PATH (
    echo Updating already existing Visual Studio Build Tools...
    "%INSTALLER_PATH%" update --wait --passive --norestart --installPath "%BUILD_TOOLS_PATH%"

    if !ERRORLEVEL! NEQ 0 (
		echo Something went wrong updating visual studio build tools: !ERRORLEVEL!
		exit /b 1
	)

	echo Adding required components to already existing Visual Studio Build Tools...
    "%INSTALLER_PATH%" modify --installPath "%BUILD_TOOLS_PATH%" ^
				   --add Microsoft.VisualStudio.Workload.VCTools ^
				   --wait --passive --norestart --includeRecommended

    if !ERRORLEVEL! NEQ 0 (
		echo Something went wrong adding required components to visual studio build tools: !ERRORLEVEL!
		exit /b 1
	)
) else (
    echo Installing Visual Studio Build Tools Product...
    "%INSTALLER_PATH%" --add Microsoft.VisualStudio.Product.BuildTools ^
    				   --add Microsoft.VisualStudio.Workload.VCTools ^
    				   --wait --passive --norestart --includeRecommended
    
    if !ERRORLEVEL! NEQ 0 (
		echo Something went wrong adding build tools product and required components: !ERRORLEVEL!
		exit /b 1
	)
)

set "RETURN_VALUE_1=%BUILD_TOOLS_PATH%\Common7\Tools\VsDevCmd.bat"

echo Successfully installed Visual Studio Build Tools.

exit /b 0
