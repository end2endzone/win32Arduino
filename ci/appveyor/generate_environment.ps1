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
$third_party = Get-AbsolutePath -Path "$scriptFolder\..\..\third_party"

############################################
# Generate envinronment.bat file
############################################
function AddEnvironment ($file, $name, $value)
{
  "echo Setting $name to value '$value'" | Add-Content $file
  "setx $name `"$value`" >NUL" | Add-Content $file
  "set  $name=$value" | Add-Content $file
}
$file = "$scriptFolder\environment.bat"
"@echo off" | Set-Content $file

AddEnvironment -File $file -Name THIRD_PARTY -Value $third_party
AddEnvironment -File $file -Name GOOGLETEST_HOME -Value $third_party\googletest
