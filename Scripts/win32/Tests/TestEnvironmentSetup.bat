@echo off

set VS_ENV=%1
set CMAKE_ENV=%2
set MONO_ENV=%3

pushd "%~dp0"

call VisualStudioEnvironmentSetup.bat %VS_ENV%

popd

call "%~dp0\CmakeEnvironmentSetup.bat" %CMAKE_ENV%
call "%~dp0\MonoEnvironmentSetup.bat" %MONO_ENV%