if not exist "Build" (
    mkdir "Build"
)
cd "Build"

echo Using generator: %GENERATOR%
%CMAKE_COMMAND% .. -G "%GENERATOR%"

if %ERRORLEVEL% neq 0 (
    echo Failed to generate build system files.

    pause
    exit 1
)

%CMAKE_COMMAND% --build . --config %BUILD_CONFIG%

if %ERRORLEVEL% neq 0 (
    echo Build failed. Please check the errors above.
    pause
    exit 1
)