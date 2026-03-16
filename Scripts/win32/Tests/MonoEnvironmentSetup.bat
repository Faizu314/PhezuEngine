set MONO_ENV=%1

echo "Setting up CMake Environment: %MONO_ENV%"

if "%MONO_ENV%"=="mono" (

	echo Installing mono
	call Scripts\win32\Helpers\SetupMono.bat

)

echo Mono Environment Setup Complete
exit /b 0