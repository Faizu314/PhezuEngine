set VS_ENV=%1

echo "Setting up Visual Studio Environment: %VS_ENV%"

echo Removing Community and Professional Visual Studio Installations...
call VsRemove.bat "Microsoft.VisualStudio.Product.Community"
call VsRemove.bat "Microsoft.VisualStudio.Product.Professional"

if "%VS_ENV%"=="--no_vs" (
	
	echo Uninstalling Visual Studio Build Tools...
	call VsRemove.bat "Microsoft.VisualStudio.Product.BuildTools"

	echo Uninstalling Visual Studio Enterprise IDE...
	call VsRemove.bat "Microsoft.VisualStudio.Product.Enterprise"

) else if "%VS_ENV%"=="--ide_full" (

	echo Uninstalling Visual Studio Build Tools...
	call VsRemove.bat "Microsoft.VisualStudio.Product.BuildTools"

) else if "%VS_ENV%"=="--ide_partial" (

	echo Uninstalling Visual Studio Build Tools...
	call VsRemove.bat "Microsoft.VisualStudio.Product.BuildTools"
	echo Uninstalling workload: Microsoft.VisualStudio.Workload.NativeDesktop of Visual Studio IDE...
	call VsRemove.bat "Microsoft.VisualStudio.Product.Enterprise" "Microsoft.VisualStudio.Workload.NativeDesktop"

) else if "%VS_ENV%"=="--bt_full" (

	echo Uninstalling Visual Studio Enterprise IDE...
	call VsRemove.bat "Microsoft.VisualStudio.Product.Enterprise"

) else if "%VS_ENV%"=="--bt_partial" (

	echo Uninstalling Visual Studio Enterprise IDE...
	call VsRemove.bat "Microsoft.VisualStudio.Product.Enterprise"
	echo Uninstalling workload: Microsoft.VisualStudio.Workload.VCTools of Visual Studio Build Tools...
	call VsRemove.bat "Microsoft.VisualStudio.Product.BuildTools" "Microsoft.VisualStudio.Workload.VCTools"

)

echo Visual Studio Environment Setup Complete
exit /b 0