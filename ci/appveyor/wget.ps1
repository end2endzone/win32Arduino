Param(
  [Parameter(Mandatory=$True)]
  [string]$path,
  [Parameter(Mandatory=$True)]
  [string]$url
)

Write-Host "Downloading '$url' to file '$path'..."

$username = ""
$password = ""
$client = New-Object System.Net.WebClient
# $client.Credentials = New-Object System.Net.Networkcredential($username, $password)
$client.DownloadFile($url, $path)
