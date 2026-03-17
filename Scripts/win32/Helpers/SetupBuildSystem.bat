set PACKAGE=vs-build-system
set VS_LINK=https://aka.ms/vs/stable/vs_BuildTools.exe
set OUTPUT_DIR=%CD%\Vendor\win32\temp\vs

call Scripts\win32\Helpers\VisualStudioFinder.bat

if %ERRORLEVEL%==1 (
    echo Did not find any compatible Visual Studio IDE or Build Tools installation.

    call Scripts\win32\Helpers\ValidateContinuation.bat "Enter Y to continue with Visual Studio Installer Download: "

    if not exist %OUTPUT_DIR% (
        mkdir %OUTPUT_DIR%
    )
    
    call Scripts\win32\Helpers\Downloader.bat %PACKAGE% %VS_LINK% %OUTPUT_DIR%\vs_BuildTools.exe
    call Scripts\win32\Helpers\VisualStudioBuildToolsInstaller.bat %OUTPUT_DIR%\vs_BuildTools.exe

    if !ERRORLEVEL! neq 0 (
        echo Unable to install build system.
        pause
        exit 1
    )

    call "%RETURN_VALUE_1%" -arch=x64

    set "GENERATOR=NMake Makefiles"
) else if %ERRORLEVEL%==2 (
    call "%RETURN_VALUE_1%" -arch=x64

    set "GENERATOR=NMake Makefiles"
) else (
    set "GENERATOR=%RETURN_VALUE_1%"
)

exit /b 0