@echo off
setlocal
cd /d "%~dp0"

if not exist "jogo_gui.exe" (
    echo Nao foi encontrado o arquivo jogo_gui.exe nesta pasta.
    pause
    exit /b 1
)

set "__COMPAT_LAYER=HIGHDPIAWARE"
start "" "%~dp0jogo_gui.exe"
