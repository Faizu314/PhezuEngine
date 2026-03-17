set PACKAGE=cmake
set CMAKE_LINK=https://github.com/Kitware/CMake/releases/download/v4.3.0-rc2/cmake-4.3.0-rc2-windows-x86_64.msi
set OUTPUT_DIR=%CD%\Vendor\win32\temp\cmake

cmake --version
if %ERRORLEVEL% neq 0 (
    if not exist "Vendor\win32\cmake\.installed" (
        echo Did not find cmake installation.
        
        call Scripts\win32\Helpers\ValidateContinuation.bat "Enter Y to continue with Cmake installation: "

        if not exist %OUTPUT_DIR% (
            mkdir %OUTPUT_DIR%
        )
        
        call Scripts\win32\Helpers\Downloader.bat %PACKAGE% %CMAKE_LINK% %OUTPUT_DIR%\cmake-4.3.0-rc2-windows-x86_64.msi
        call Scripts\win32\Helpers\CmakeInstaller.bat %CD% %OUTPUT_DIR%\cmake-4.3.0-rc2-windows-x86_64.msi %OUTPUT_DIR%\extract

        if !ERRORLEVEL! neq 0 (
            echo Error installing Cmake. %ERRORLEVEL%
            pause
            exit 1
        )
        echo Successfully installed Cmake at "%CD%\Vendor\win32\cmake"
    )

    set "CMAKE_COMMAND="%CD%\Vendor\win32\cmake\bin\cmake.exe""
) else (
    echo Cmake installation found.

    set "CMAKE_COMMAND=cmake"
)

echo Using CMake command: %CMAKE_COMMAND%

exit /b 0