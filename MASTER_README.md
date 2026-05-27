Master e uma pasta limpa para continuar o projeto sem depender da base antiga de `rodando em notes`.

Conteudo principal:
- `src/`: codigo-fonte atualizado a partir de `jogo_pif_capstone`
- `assets/`: spritesheets e shaders atuais
- `jogo_gui.exe`: executavel presente na base mais nova
- `libraylib.dll` e `glfw3.dll`: DLLs necessarias para rodar em outra maquina
- `rodar_master.bat`: abre o executavel atual
- `compilar_master_msys2.bat`: recompila com o MSYS2 UCRT64, se instalado

Observacao importante:
- A tentativa de misturar a `raylib_main.c` nova com a arquitetura antiga de `rodando em notes` gerou incompatibilidades de tipos entre `Deck`, `Card` e `player_data`.
- Nesta pasta `Master`, a base foi reunida a partir da versao mais consistente do projeto para evitar esse conflito.

Se voce tiver MSYS2 + raylib:
1. Abra o terminal `UCRT64`.
2. Entre nesta pasta.
3. Rode `gcc -std=c11 -Wall -Wextra -pedantic -Isrc src/raylib_main.c src/cards.c src/shop.c -lraylib -lm -lopengl32 -lgdi32 -lwinmm -o jogo_gui.exe`

Se quiser apenas testar:
- Execute `rodar_master.bat`.
