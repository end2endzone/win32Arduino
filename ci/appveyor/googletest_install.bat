@echo off
cd /d %~dp0
cd ..\..

powershell -nologo -executionpolicy bypass -File "%~dp0install_github_package.ps1" -name "googletest" -installpath "%cd%\third_party" -url "http://codeload.github.com/google/googletest/zip/release-1.6.0"
echo.

echo =======================================================================
echo Generating googletest Visual Studio 2010 solution
echo =======================================================================
set GOOGLETEST_HOME=%cd%\third_party\googletest

echo Deleting previous build folder (if any)
rmdir /s/q build >NUL 2>NUL
echo done.
echo.

echo Creating build folder.
mkdir %GOOGLETEST_HOME%\build
echo done.
echo.

echo Launching cmake...
cd /d %GOOGLETEST_HOME%\build
cmake -G "Visual Studio 10 2010" -Dgtest_force_shared_crt=ON -DCMAKE_CXX_FLAGS_DEBUG=/MDd -DCMAKE_CXX_FLAGS_RELEASE=/MD ..
echo done.
echo.
