$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

$exe = Join-Path $PSScriptRoot "jogo_gui.exe"
$sources = @(
    (Join-Path $PSScriptRoot "src\\raylib_main.c")
    (Join-Path $PSScriptRoot "src\\cards.c")
    (Join-Path $PSScriptRoot "src\\shop.c")
    (Join-Path $PSScriptRoot "src\\player_data.h")
    (Join-Path $PSScriptRoot "src\\raylib_question_bank.h")
)

$needsBuild = -not (Test-Path $exe)
if (-not $needsBuild) {
    $exeTime = (Get-Item $exe).LastWriteTime
    foreach ($file in $sources) {
        if ((Get-Item $file).LastWriteTime -gt $exeTime) {
            $needsBuild = $true
            break
        }
    }
}

if ($needsBuild) {
    $gcc = Get-Command gcc -ErrorAction SilentlyContinue
    if (-not $gcc) {
        throw "gcc nao foi encontrado. Para recompilar, instale o MinGW com raylib ou compartilhe esta pasta ja com o jogo_gui.exe pronto."
    }

    & $gcc.Source -std=c11 -Wall -Wextra -pedantic -Isrc src/raylib_main.c src/cards.c src/shop.c -lraylib -lm -o jogo_gui.exe
    if ($LASTEXITCODE -ne 0) {
        throw "Falha ao compilar o jogo_gui.exe."
    }
}

& $exe
