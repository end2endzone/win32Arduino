@echo off
cd /d %~dp0

mkdir build
cd build
cmake ..
