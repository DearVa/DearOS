@echo off
del /q ..\data\*.*
for %%i in (*.lua) do (
    echo compiling %cd%\%%i
    luac -o ..\data\%%i -- %cd%\%%i
    ren ..\data\%%i *.
)
pause