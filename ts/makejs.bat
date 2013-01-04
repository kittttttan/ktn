@echo off

cd /d %~d0%~p0

for %%i in (lib/*.ts) do (
  tsc lib/%%i -c
)

for %%i in (test/*.ts) do (
  tsc test/%%i -c
)
