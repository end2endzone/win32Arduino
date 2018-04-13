@echo off

echo =======================================================================
echo Uploading test results to AppVeyor
echo =======================================================================
cd /d %~dp0
cd ..\..\publish
set TEST_RESULT_URL=https://ci.appveyor.com/api/testresults/junit/%APPVEYOR_JOB_ID%
set TEST_RESULT_FILE=%CD%\win32Arduino_unittest.x86.release.xml
echo TEST_RESULT_URL=%TEST_RESULT_URL%
echo TEST_RESULT_FILE=%TEST_RESULT_FILE%
powershell "(New-Object 'System.Net.WebClient').UploadFile($($env:TEST_RESULT_URL), $($env:TEST_RESULT_FILE))"
