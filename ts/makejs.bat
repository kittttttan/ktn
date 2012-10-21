@echo off

cd /d %~d0%~p0

rem for %%i in (lib/*.ts) do (
rem   tsc lib/%%i
rem )

for %%i in (test/*.ts) do (
  tsc test/%%i
)
