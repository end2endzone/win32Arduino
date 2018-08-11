@echo off

:: Validate appveyor's environment
if "%APPVEYOR_BUILD_FOLDER%"=="" (
  echo Please define 'APPVEYOR_BUILD_FOLDER' environment variable.
  exit /B 1
)

set GTEST_ROOT=%APPVEYOR_BUILD_FOLDER%\third_parties\googletest\install
set rapidassist_DIR=%APPVEYOR_BUILD_FOLDER%\third_parties\RapidAssist\install
set win32arduino_DIR=%APPVEYOR_BUILD_FOLDER%\install
set LIBRARY_TEMP_DIR=%APPVEYOR_BUILD_FOLDER%\temp

echo ============================================================================
echo Copying files...
echo ============================================================================
mkdir %LIBRARY_TEMP_DIR% >NUL 2>NUL
echo Copying unit test template files...
xcopy /Y /S %APPVEYOR_BUILD_FOLDER%\usage\template\*.*       %LIBRARY_TEMP_DIR%
echo Copying ButtonLibrary files...
xcopy /Y /S %APPVEYOR_BUILD_FOLDER%\usage\ButtonLibrary\*.*  %LIBRARY_TEMP_DIR%

echo ============================================================================
echo Generating...
echo ============================================================================
cd /d %LIBRARY_TEMP_DIR%
mkdir build >NUL 2>NUL
cd build
cmake ..
if %errorlevel% neq 0 exit /b %errorlevel%

echo ============================================================================
echo Compiling...
echo ============================================================================
cmake --build . --config Release
if %errorlevel% neq 0 exit /b %errorlevel%
echo.

::Delete all temporary environment variable created
set GTEST_ROOT=
set rapidassist_DIR=
set win32arduino_DIR=
set LIBRARY_TEMP_DIR=

::Return to launch folder
cd /d %~dp0
