@echo off
rem compile with closure compiler
rem https://developers.google.com/closure/compiler/

setlocal
cd /d %~d0%~p0

rem rewrite below path
set COMPILER=D:\Documents\js\compiler-latest\compiler.jar
set COMPILE=java -jar %COMPILER%

set /a QUIET=0
if /i "%1" == "/Q" set /a QUIET=1

echo %DATE% %TIME%> js.log

for %%i in (lib/*.js) do (
  if %QUIET% EQU 0 echo compile lib/%%i
  %COMPILE% --js lib/%%i --js_output_file lib/%%~ni-min.js 2>> js.log
)

for %%i in (test/*.js) do (
  if %QUIET% EQU 0 echo compile test/%%i
  %COMPILE% --js test/%%i --js_output_file test/%%~ni-min.js 2>> js.log
)
