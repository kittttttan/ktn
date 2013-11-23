@echo off
rem plato
rem https://github.com/jsoverson/plato

setlocal
cd /d %~dp0

plato -r -d report ../src
