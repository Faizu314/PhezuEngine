@echo off

set PACKAGE=%~1
set LINK=%~2
set OUTPUT_DIR=%~3

:loop_start

echo Downloading %PACKAGE% from the following link: %LINK%

curl -C - "%LINK%" -o "%OUTPUT_DIR%"

if %ERRORLEVEL% neq 0 (
    echo Error: Something went wrong while downloading mono.
    pause
    goto :loop_start
)

echo Successfully downloaded mono.