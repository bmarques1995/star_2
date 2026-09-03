Param(
    [Parameter(Mandatory=$true)]
    [string]$presetDir,
    [string]$relativeSourceLocation
)

$rootDir = Get-Location
$sourceDir = Join-Path $rootDir $relativeSourceLocation
$presetAbsoluteDir = Join-Path $rootDir $presetDir
Set-Location $presetAbsoluteDir
$lib = Join-Path $presetAbsoluteDir "vcpkg_installed\x64-windows\debug\bin\star.dll"
$exec = Join-Path $presetAbsoluteDir "vcpkg_installed\x64-windows\debug\bin\star_tests.exe"

OpenCppCoverage.exe --modules $lib --sources $sourceDir -- $exec

Set-Location $rootDir

# Example
#.\gen_cov.ps1 -presetDir out/build/windows-vcpkg-debug -relativeSourceLocation src