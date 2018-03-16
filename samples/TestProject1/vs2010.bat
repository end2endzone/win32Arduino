@echo off
cd /d %~dp0

set SRC_DIR=%cd%

REM Create build directory
mkdir build >NUL 2>NUL
cd build

cmake -G "Visual Studio 10 2010" %SRC_DIR%
