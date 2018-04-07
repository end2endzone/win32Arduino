@echo off
powershell -nologo -executionpolicy bypass -File "%~dpn0.ps1" -path favicon.ico -url "https://www.google.com/favicon.ico"
pause
