@echo off

set PACKAGE=%~1
set DOWNLOAD_LINK=%~2
set OUTPUT_PATH=%~3

:loop_start

echo Downloading %PACKAGE% from the following link: %DOWNLOAD_LINK%

curl -L -C - "%DOWNLOAD_LINK%" -o "%OUTPUT_PATH%"

if %ERRORLEVEL% neq 0 (
    echo Error: Something went wrong while downloading %PACKAGE%.
    pause
    goto :loop_start
)

echo Successfully downloaded %PACKAGE%.

exit /b 0