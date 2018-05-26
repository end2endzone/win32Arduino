@echo off

cd /d %~dp0
cd ..\..
mkdir build >NUL 2>NUL
cd build
cmake -DWIN32ARDUINO_BUILD_TEST=ON -DWIN32ARDUINO_BUILD_SAMPLES=ON ..
