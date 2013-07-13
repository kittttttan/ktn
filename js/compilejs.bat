@echo off
rem compile with closure compiler
rem https://developers.google.com/closure/compiler/

setlocal
cd /d %~dp0

rem rewrite below path
set COMPILER=D:\Documents\js\compiler-latest\compiler.jar
set OPTIONS=--language_in=ECMASCRIPT5_STRICT --compilation_level SIMPLE_OPTIMIZATIONS
rem WHITESPACE_ONLY SIMPLE_OPTIMIZATIONS ADVANCED_OPTIMIZATIONS 
set COMPILE=java -jar %COMPILER% %OPTIONS%

set /a IS_NEW=0
set /a ALL=0
set /a QUIET=0
call :opt %*

echo %DATE% %TIME%> js.log

for %%i in (lib/src/*.js) do call :main lib %%i
for %%i in (test/src/*.js) do call :main test %%i

exit /b

:opt
:opt_loop
  if "%1"=="" goto :EOF
  if /i "%1"=="/H" (
    call :help
    exit /b
  ) else if /i "%1"=="/A" (
    set /a ALL=1
  ) else if /i "%1"=="/Q" (
    set /a QUIET=1
  )
  
  shift
  goto :opt_loop
goto :EOF

:help
  echo compile .js files with closure compiler
  echo.
  echo COMPILEJS [options]
  echo.
  echo ^ ^ /A  compile all
  echo ^ ^ /H  show help
  echo ^ ^ /Q  quiet
  exit /b
goto :EOF

:main
  for %%x in (%1/src/%2) do set SRC_T="%%~tx"
  for %%x in (%1/%~n2.js ?) do set DEST_T="%%~tx"
  call :cmp_date %SRC_T% %DEST_T%
  
  set /a DO_COMPILE=ALL ^| IS_NEW
  if %DO_COMPILE% EQU 1 (
    if %QUIET% EQU 0 echo compile %1/src/%2
    %COMPILE% --js %1/src/%2 --js_output_file %1/%~n2.js 2>> js.log
  )
goto :EOF

rem %1 src  "yyyy/MM/dd hh:mm"
rem %2 dest "yyyy/MM/dd hh:mm"
rem set IS_NEW=0 if src is older than dest
:cmp_date
  set /a IS_NEW=1

  if %2=="" goto :EOF
  
  set LDH=%1
  set RDH=%2
rem  echo %LDH%, %RDH%
  
  rem compare date yyyyMMdd
  set LDATE=%LDH:~1,4%%LDH:~6,2%%LDH:~9,2%
  set RDATE=%RDH:~1,4%%RDH:~6,2%%RDH:~9,2%
rem  echo %LDATE%, %RDATE%
  if %LDATE% GTR %RDATE% (
    goto :EOF
  ) else if %LDATE% LSS %RDATE% (
    set /a IS_NEW=0
    goto :EOF
  )
    
  rem compare time hhmm
  set LTIME=%LDH:~12,2%%LDH:~15,2%
  set RTIME=%RDH:~12,2%%RDH:~15,2%
rem  echo %LTIME%, %RTIME%
  if %LTIME% LSS %RTIME% (
    set /a IS_NEW=0
  )
goto :EOF
