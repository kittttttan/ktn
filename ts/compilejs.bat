@echo off

rem compile with closure compiler
rem https://developers.google.com/closure/compiler/

rem rewrite below path
set COMPILER=D:\Documents\js\compiler-latest\compiler.jar
set COMPILE=java -jar %COMPILER%

cd /d %~d0%~p0

for %%i in (lib/*.js) do (
  %COMPILE% --js lib/%%i --js_output_file lib/%%~ni-min.js
)

for %%i in (test/*.js) do (
  %COMPILE% --js test/%%i --js_output_file test/%%~ni-min.js
)
