@echo off

echo =======================================================================
echo Setting environnment variables for googletest
echo =======================================================================
cd /d %~dp0
cd ..\..
set  GOOGLETEST_HOME=%REPOSITORY_ROOT%\third_party\googletest
setx GOOGLETEST_HOME %GOOGLETEST_HOME%
echo Found GOOGLETEST_HOME=%GOOGLETEST_HOME%
echo.

echo Downloading googletest to folder %GOOGLETEST_HOME%
echo.

echo ======================================================================
echo Deleting googletest repository folder (if any)
echo =======================================================================
if EXIST %GOOGLETEST_HOME% (
  rmdir /q/s %GOOGLETEST_HOME%
)
echo done.
echo.

echo ======================================================================
echo Cloning googletest repository
echo =======================================================================
git clone https://github.com/google/googletest.git %GOOGLETEST_HOME%
cd /d %GOOGLETEST_HOME%
git checkout release-1.6.0
echo done.
echo.

echo =======================================================================
echo Generating googletest Visual Studio 2010 solution
echo =======================================================================
cd /d %GOOGLETEST_HOME%

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
