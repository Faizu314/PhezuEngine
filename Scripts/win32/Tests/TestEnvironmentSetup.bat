@echo off

set VS_ENV=%1
set CMAKE_ENV=%2
set MONO_ENV=%3

pushd "%~dp0"

CALL VisualStudioEnvironmentSetup.bat %VS_ENV%