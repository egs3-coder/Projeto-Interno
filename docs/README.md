# MGTA Deck - Capstone 3

Jogo academico em C inspirado em jogos de cartas roguelike, combinando maos de poker, quizzes por area de conhecimento, progressao por apostas/blinds, ranking por modalidade e historico de partidas.

## O que esta pronto nesta base

- Run completa com cartas, blinds, boss, loja, packs, tarot e coringas
- Interface `raylib` integrada ao loop principal da run
- Botao `Info tentativa` durante a fase e a loja, com abas de maos de poker, blinds e quiz
- Baralho restante na tela da fase, com tooltip/listagem das cartas que ainda podem vir
- Uso de Tarot direto na mao: selecione cartas, selecione o Tarot e use quando o limite estiver valido
- Tela de perfil antes da run para:
  - inserir o nome do jogador
  - escolher categorias de perguntas
  - classificar automaticamente a modalidade da run
- Modalidades com ranking top 10:
  - `Matematicos`
  - `Linguisticos`
  - `Logicos`
  - `Programadores`
  - `Escolarizados`
  - `Concurseiros`
  - `Engenheiros`
  - `Intelectuais`
  - `Perfil Misto`
- Historico por jogador salvo em `run_history_raylib.txt`
- Botao para desvincular o jogador atual e trocar de pessoa no menu
- Banco de perguntas por categoria:
  - `Logica Classica`
  - `Logica Matematica`
  - `Logica de Prog. em C`
  - `Significados em Portugues`
- Escalonamento das perguntas por aposta:
  - apostas `1-2`: `Facil`
  - apostas `3-4`: `Avancada`
  - apostas `5-6`: `Intermediaria`
  - apostas `7-8`: `Dificil`
  - boss e pulo de blind: `Muito Dificil`
- Perguntas erradas voltam na tentativa ate serem acertadas, registrando quantas tentativas foram necessarias
- Cupons agora exigem uma blind jogada e duas perguntas corretas na aposta atual
- Pulos de fase podem render marca, tamanho de mao, pacotes gratis, dinheiro ou atualizacoes gratis na loja
- Assets opcionais em `assets/`, com fallback desenhado por `raylib`
- Relatorio analitico no fim da tentativa e na tela de historico, usando funcoes recursivas para soma, minimo, maximo e soma dos quadrados.

## Como executar

Abra a pasta do projeto e rode:

```powershell
.\rodar_jogo_gui.bat
```

Ou execute diretamente:

```powershell
.\jogo_gui.exe
```

Arquivos que devem ficar juntos para a versao grafica:

- `jogo_gui.exe`
- `libraylib.dll`
- `glfw3.dll`, quando presente na distribuicao
- `mouse_calibration.ini`
- `rodar_jogo_gui.bat`

## Como compilar

Com GCC e raylib disponiveis no ambiente:

```powershell
gcc -std=c11 -Wall -Wextra -pedantic -Isrc src/raylib_main.c src/cards.c src/shop.c -lraylib -lm -o jogo_gui.exe
```

## Funcionalidades

- Perfil de jogador e modalidades de perguntas.
- Runs por apostas, Small Blind, Big Blind e Blind Chefe.
- Quiz por categoria e dificuldade.
- Pulo de fase por pergunta muito dificil.
- Cartas, melhorias, selos, coringas, tarots e cupons.
- Colecao persistente por jogador.
- Ranking e tela de sessoes/historico.
- Relatorio funcional por partida salvo em `run_history.txt`.

## Estrutura

- `src/cards.c` / `src/cards.h`: cartas, baralho e avaliacao de maos.
- `src/shop.c` / `src/shop.h`: loja, coringas, tarots, cupons, bosses e recompensas.
- `src/quiz.c` / `src/quiz.h`: banco de perguntas e avaliacao de respostas.
- `src/player_data.c` / `src/player_data.h`: perfil, colecao, ranking e historico de runs.
- `src/history.c` / `src/history.h`: historico do modo numerico/console.
- `src/raylib_main.c`: interface grafica em raylib.

## Historico e testes

O arquivo `run_history.txt` contem registros de partidas no formato usado pela tela de ranking e sessoes. Para atender aos testes de volume, tambem ha `historico.txt` com 100 partidas simuladas/documentadas para validacao.

## Artefatos

Os artefatos de desenvolvimento estao em `docs/`:

- `docs/user_stories.md`
- `docs/sprints_tarefas.md`
- `docs/plano_de_testes.md`
- `docs/wireframes_mockups.md`
- `docs/adr.md`

## Observacoes

As pastas de backup em `outras versoes/` foram mantidas para rastreabilidade. Os executaveis legacy tambem foram preservados para comparacao e recuperacao.

## Arquivos importantes

- `src/raylib_main.c`: interface, fluxo da run e telas do raylib
- `src/player_data.h`: jogador atual, modalidades, ranking e historico
- `src/raylib_question_bank.h`: perguntas por categoria e dificuldade
- `src/cards.c` e `src/shop.c`: mecanicas base da run
- `run_history_raylib.txt`: historico salvo automaticamente
- `historico.txt`: massa de teste com 100 sessoes sinteticas
- `assets/README.md`: padrao de nomes para assets opcionais
O jogo tenta carregar imagens em `assets/` para cartas, coringas, tarots, cupons, pacotes, aprimoramentos e o personagem provocador.

Se algum arquivo nao existir, a GUI usa o desenho proprio em `raylib`, entao o executavel continua abrindo normalmente.

Consulte `assets/README.md` para os nomes esperados.


