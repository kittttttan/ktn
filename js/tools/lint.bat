@echo off
rem Closure Linter
rem https://developers.google.com/closure/utilities/

setlocal
cd /d %~dp0

rem set OPT=--strict
set OPT=--nobeep --nojsdoc --custom_jsdoc_tags=example,fileOverview,method,property,static
gjslint %OPT% -r ../src>lint.log
