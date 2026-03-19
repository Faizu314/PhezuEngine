@echo off

if %AUTO_MODE%==1 (
    exit /b 0
)

set /p USER_INPUT=%~1

if /i "%USER_INPUT%"=="Y" (
    exit /b 0
)

echo Exiting.
pause
exit 0