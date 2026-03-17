set VS_ENV=%1
set CMAKE_ENV=%2
set MONO_ENV=%3

pushd "%~dp0"

call VisualStudioEnvironmentSetup.bat %VS_ENV%

popd

call Scripts\win32\Helpers\CmakeEnvironmentSetup.bat %CMAKE_ENV%
call Scripts\win32\Helpers\MonoEnvironmentSetup.bat %MONO_ENV%