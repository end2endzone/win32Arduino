@echo off

echo Change directory to output directory
cd /d %~dp0
cd ..\..\src\bin\Win32\Release

echo =======================================================================
echo Running unit tests...
echo =======================================================================
win32Arduino_unittest.exe
echo done
echo.

echo =======================================================================
echo Uploading test results to AppVeyor
echo =======================================================================
set TEST_RESULT_URL=https://ci.appveyor.com/api/testresults/junit/%APPVEYOR_JOB_ID%
set TEST_RESULT_FILE=%CD%\win32Arduino_unittest.testResults.xml
echo TEST_RESULT_URL=%TEST_RESULT_URL%
echo TEST_RESULT_FILE=%TEST_RESULT_FILE%
powershell "(New-Object 'System.Net.WebClient').UploadFile($($env:TEST_RESULT_URL), $($env:TEST_RESULT_FILE))"
