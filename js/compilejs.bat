@echo off

rem compile with closure compiler
rem https://developers.google.com/closure/compiler/

rem rewrite below path
set COMPILER=D:\Documents\js\compiler-latest\compiler.jar
set COMPILE=java -jar %COMPILER%

cd /d %~d0%~p0

echo %DATE% %TIME% > js.log

for %%i in (lib/src/*.js) do (
  %COMPILE% --js lib/src/%%i --js_output_file lib/%%~ni.js 2>> js.log
)

for %%i in (test/src/*.js) do (
  %COMPILE% --js test/src/%%i --js_output_file test/%%~ni.js 2>> js.log
)
