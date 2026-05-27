@echo off
setlocal
cd /d "%~dp0"
set "PATH=C:\msys64\ucrt64\bin;%PATH%"
taskkill /IM jogo_gui.exe /F >nul 2>nul
jogo_gui.exe
