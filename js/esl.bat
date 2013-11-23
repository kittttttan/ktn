@echo off
rem eslint

setlocal
cd /d %~dp0

eslint -c eslint.json ^
lib\src\decimal.js ^
lib\src\integer.js ^
lib\src\ktn.js ^
lib\src\rational.js
