@echo off
setlocal
cd /d "%~dp0"

set "GCC=C:\msys64\ucrt64\bin\gcc.exe"

if not exist "%GCC%" (
    echo GCC do MSYS2 nao foi encontrado em %GCC%.
    pause
    exit /b 1
)

"%GCC%" -std=c11 -Wall -Wextra -pedantic -Isrc src\raylib_main.c src\cards.c src\shop.c -LC:\msys64\ucrt64\lib -lraylib -lm -lopengl32 -lgdi32 -lwinmm -o jogo_gui.exe

if errorlevel 1 (
    echo Falha ao compilar jogo_gui.exe.
    pause
    exit /b 1
)

echo Compilacao concluida.
pause
