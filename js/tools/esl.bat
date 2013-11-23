@echo off
rem eslint

setlocal
cd /d %~dp0

eslint -c eslint.json ^
..\src\decimal.js ^
..\src\integer.js ^
..\src\ktn.js ^
..\src\rational.js
