$targetfile = "..\..\src\CMakeLists.txt"
Write-Output "Disabling unit tests from $targetfile..."

$token = "add_subdirectory(win32Arduino_unittest)"
$value = "#$token"
(Get-Content $targetfile) -replace [Regex]::Escape($token), $value | Set-Content $targetfile

$token = "add_dependencies(win32Arduino_unittest win32Arduino)"
$value = "#$token"
(Get-Content $targetfile) -replace [Regex]::Escape($token), $value | Set-Content $targetfile

Write-Output "Writing file $targetfile completed."
