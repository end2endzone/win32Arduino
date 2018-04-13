@echo off
curl -fsS -o %TEMP%\apache-ant-1.10.2-bin.zip "http://archive.apache.org/dist/ant/binaries/apache-ant-1.10.2-bin.zip"
7z x %TEMP%\apache-ant-1.10.2-bin.zip "-oC:\Program Files (x86)\ant"
set ANT_HOME=C:\Program Files (x86)\ant\apache-ant-1.10.2
set PATH=%PATH%;%ANT_HOME%\bin
call ant -version
