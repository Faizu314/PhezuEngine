set VS_ENV=%1

echo "Setting up Visual Studio Environment: %VS_ENV%"

echo Removing Enterprise and Professional Visual Studio Installations...
CALL VsRemove.bat "Microsoft.VisualStudio.Product.Enterprise"
CALL VsRemove.bat "Microsoft.VisualStudio.Product.Professional"

if "%VS_ENV%"=="ide_full" (

	echo Uninstalling Visual Studio Build Tools...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.BuildTools"

) else if "%VS_ENV%"=="ide_partial" (

	echo Uninstalling Visual Studio Build Tools...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.BuildTools"
	echo Uninstalling component: Microsoft.VisualStudio.Component.VC.Tools.x86.x64 of Visual Studio IDE...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.Community" "Microsoft.VisualStudio.Component.VC.Tools.x86.x64"

) else if "%VS_ENV%"=="ide_missing_workload" (

	echo Uninstalling Visual Studio Build Tools...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.BuildTools"
	echo Uninstalling workload: Microsoft.VisualStudio.Workload.NativeDesktop of Visual Studio IDE...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.Community" "Microsoft.VisualStudio.Workload.NativeDesktop"

) else if "%VS_ENV%"=="bt_full" (

	echo Uninstalling Visual Studio Community IDE...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.Community"

) else if "%VS_ENV%"=="bt_partial" (

	echo Uninstalling Visual Studio Community IDE...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.Community"
	echo Uninstalling component: Microsoft.VisualStudio.Component.VC.Tools.x86.x64 of Visual Studio Build Tools...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.BuildTools" "Microsoft.VisualStudio.Component.VC.Tools.x86.x64"

) else if "%VS_ENV%"=="bt_missing_workload" (

	echo Uninstalling Visual Studio Community IDE...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.Community"
	echo Uninstalling workload: Microsoft.VisualStudio.Workload.VCTools of Visual Studio Build Tools...
	CALL VsRemove.bat "Microsoft.VisualStudio.Product.BuildTools" "Microsoft.VisualStudio.Workload.VCTools"

)

echo Visual Studio Environment SetupComplete
exit /b 0