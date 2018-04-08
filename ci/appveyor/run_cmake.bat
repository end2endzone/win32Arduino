@echo off
powershell -nologo -executionpolicy bypass -File "%~dpn0.ps1" -build %GOOGLETEST_HOME%\build -src %GOOGLETEST_HOME%
