set ROOT_DIR=%~1
set MONO_MSI_PATH=%~2
set MONO_EXTRACT_DIR=%~3

:: Extract Mono .msi

echo Extracting Mono msi
msiexec /a "%MONO_MSI_PATH%" TARGETDIR="%MONO_EXTRACT_DIR%" /qn

if %ERRORLEVEL% neq 0 (
    echo Error extracting mono
    exit /b 1
)

:: Entire Mono Installation

echo copying mono folder
if not exist "%ROOT_DIR%\Vendor\win32\mono" (
    mkdir "%ROOT_DIR%\Vendor\win32\mono"
)
robocopy "%MONO_EXTRACT_DIR%\Mono" "%ROOT_DIR%\Vendor\win32\mono" /E /COPYALL /NJH /NJS /NC /NS

if %ERRORLEVEL% geq 8 (
    echo Error copying mono files
    exit /b 1
)

:: Mono Include Headers for Phezu

echo copying mono headers
if not exist "%ROOT_DIR%\Phezu\Vendor\include\win32\mono" (
    mkdir "%ROOT_DIR%\Phezu\Vendor\include\win32\mono"
)
robocopy "%MONO_EXTRACT_DIR%\Mono\include\mono-2.0\mono" "%ROOT_DIR%\Phezu\Vendor\include\win32\mono" /E /COPYALL /NJH /NJS /NC /NS

if %ERRORLEVEL% geq 8 (
    echo Error copying mono headers
    exit /b 1
)

type nul > "%ROOT_DIR%\Vendor\win32\mono\.installed"

exit /b 0