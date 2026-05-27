@echo off
setlocal
cd /d "%~dp0"

if not exist "jogo_gui.exe" (
    echo Nao foi encontrado o arquivo jogo_gui.exe nesta pasta.
    pause
    exit /b 1
)

if not exist "libraylib.dll" (
    echo Nao foi encontrado o arquivo libraylib.dll nesta pasta.
    echo Deixe a DLL ao lado do jogo_gui.exe para rodar em outras maquinas.
    pause
    exit /b 1
)

if not exist "libraylib_real.dll" (
    echo Nao foi encontrado o arquivo libraylib_real.dll nesta pasta.
    echo Esta DLL e necessaria para a versao com proxy de escala.
    pause
    exit /b 1
)

if not exist "glfw3.dll" (
    echo Nao foi encontrado o arquivo glfw3.dll nesta pasta.
    pause
    exit /b 1
)

set "__COMPAT_LAYER=DPIUNAWARE"
jogo_gui.exe
