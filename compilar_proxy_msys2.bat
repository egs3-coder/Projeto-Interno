@echo off
setlocal
cd /d "%~dp0"

set "GCC=C:\msys64\ucrt64\bin\gcc.exe"

if not exist "%GCC%" (
    echo GCC do MSYS2 nao foi encontrado em %GCC%.
    echo Instale o MSYS2 e execute: pacman -S mingw-w64-ucrt-x86_64-gcc
    pause
    exit /b 1
)

echo Compilando proxy DLL (libraylib.dll)...

"%GCC%" -std=c11 -O2 -shared -o libraylib.dll proxy_libraylib.c -luser32 -lgdi32

if errorlevel 1 (
    echo.
    echo FALHA na compilacao. Verifique os erros acima.
    pause
    exit /b 1
)

echo.
echo Proxy compilado com sucesso!
echo libraylib.dll agora e o proxy de escala automatica.
echo Certifique-se de que libraylib_real.dll esta na mesma pasta.
echo.
echo Para rodar o jogo: execute rodar_com_proxy.bat
pause
