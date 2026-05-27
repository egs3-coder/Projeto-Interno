@echo off
setlocal
cd /d "%~dp0"

set "__COMPAT_LAYER=DPIUNAWARE"

if not exist "jogo_gui.exe" (
    echo Nao foi encontrado o arquivo jogo_gui.exe nesta pasta.
    pause
    exit /b 1
)

if not exist "libraylib.dll" (
    echo Nao foi encontrado o arquivo libraylib.dll nesta pasta.
    pause
    exit /b 1
)

if not exist "glfw3.dll" (
    echo Nao foi encontrado o arquivo glfw3.dll nesta pasta.
    echo Esta DLL e necessaria para abrir a versao grafica atual.
    pause
    exit /b 1
)

jogo_gui.exe
