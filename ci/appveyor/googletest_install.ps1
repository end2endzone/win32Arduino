############################################
# Find third_party folder
############################################
function Get-AbsolutePath ($Path)
{
    # System.IO.Path.Combine has two properties making it necesarry here:
    #   1) correctly deals with situations where $Path (the second term) is an absolute path
    #   2) correctly deals with situations where $Path (the second term) is relative
    # (join-path) commandlet does not have this first property
    $Path = [System.IO.Path]::Combine( ((pwd).Path), ($Path) );

    # this piece strips out any relative path modifiers like '..' and '.'
    $Path = [System.IO.Path]::GetFullPath($Path);

    return $Path;
}
$scriptFolder = Split-Path -Path $MyInvocation.MyCommand.Path
$thirdPartyFolder = Get-AbsolutePath -Path "$scriptFolder\..\..\third_party"

############################################
# Install googletest github package.
############################################
$name = "googletest"
Write-Output "Installing $name package..."

# Run install_github_package script.
$argumentList = "-name $name -installpath $thirdPartyFolder -url `"http://codeload.github.com/google/googletest/zip/release-1.6.0`""
Invoke-Expression "& `"$scriptFolder\install_github_package.ps1`" $argumentList"

# Copy installation environment variable 
$env:GOOGLETEST_HOME = $env:INSTALL_HOME
$buildFolder = "$env:GOOGLETEST_HOME\build"

############################################
# Deleting previous build folder (if any)
############################################
function DeleteIfExists($path)
{
  if ( Test-Path $path -PathType Container )
  {
    $command_output = Remove-Item –Path "$path" –recurse
  }
}
DeleteIfExists -Path $buildFolder

############################################
# Create build directory
############################################
$command_output = New-Item -ItemType Directory -Force -Path $buildFolder

############################################
Write-Output "Launching cmake..."
############################################
cd $buildFolder
cmake -G "Visual Studio 10 2010" -Dgtest_force_shared_crt=ON -DCMAKE_CXX_FLAGS_DEBUG=/MDd -DCMAKE_CXX_FLAGS_RELEASE=/MD ..
Write-Output "done."
Write-Output ""
