@echo off
cd /d %~dp0

mkdir build
cd build
cmake -G "Visual Studio 10 2010" ..
