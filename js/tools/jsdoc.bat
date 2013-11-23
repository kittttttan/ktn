@echo off
rem jsdoc
rem https://github.com/jsdoc3/jsdoc

setlocal
cd /d %~dp0
set JSDOC=D:\Documents\js\jsdoc\jsdoc.cmd

%JSDOC% -r ../src
