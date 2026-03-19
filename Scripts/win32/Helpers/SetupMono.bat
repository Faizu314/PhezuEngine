set PACKAGE=mono
set MONO_LINK=https://download.mono-project.com/archive/6.12.0/windows-installer/mono-6.12.0-x64-0.msi
set OUTPUT_DIR=%CD%\Vendor\win32\temp\mono

if not exist "Vendor\win32\mono\.installed" (
    cmd /c call Scripts\win32\Helpers\ValidateContinuation.bat "Enter Y to continue with Mono installation: "

    if not exist %OUTPUT_DIR% (
        mkdir %OUTPUT_DIR%
    )
    
    call Scripts\win32\Helpers\Downloader.bat %PACKAGE% %MONO_LINK% %OUTPUT_DIR%\mono-6.12.0-x64-0.msi
    call Scripts\win32\Helpers\MonoInstaller.bat %CD% %OUTPUT_DIR%\mono-6.12.0-x64-0.msi "%OUTPUT_DIR%\extract"

    if %ERRORLEVEL% neq 0 (
        echo Error installing Mono.
        pause
        exit 1
    )
    echo Successfully installed Mono
) else (
    echo Mono already installed
)

exit /b 0