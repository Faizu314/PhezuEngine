if not exist "Build" (
    mkdir "Build"
)
cd "Build"

echo Using generator: %GENERATOR% and config args: %CONFIG_ARGS%
%CMAKE_COMMAND% .. -G "%GENERATOR%" "%CONFIG_ARGS%"

if %ERRORLEVEL% neq 0 (
    echo Failed to generate build system files, ensure that cmake's version supports the generator being used.

    pause
    exit 1
)

%CMAKE_COMMAND% --build . --config %BUILD_CONFIG%

if %ERRORLEVEL% neq 0 (
    echo Build failed. Please check the errors above.
    pause
    exit 1
)