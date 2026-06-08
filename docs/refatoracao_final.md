# Refatoracao Final

## Objetivo

Organizar a entrega final sem descaracterizar a versao funcional do jogo. Os
executaveis antigos e backups foram preservados, enquanto a documentacao e os
arquivos de apoio foram atualizados para a avaliacao do Capstone 3.

## Estrutura em headers

Os modulos principais possuem pares `.c` e `.h`:

- `analysis.c` / `analysis.h`
- `cards.c` / `cards.h`
- `game.c` / `game.h`
- `history.c` / `history.h`
- `pif_guess.c` / `pif_guess.h`
- `player_data.c` / `player_data.h`
- `quiz.c` / `quiz.h`
- `shop.c` / `shop.h`

`main.c` e `raylib_main.c` permanecem como pontos de entrada, por isso nao
exigem headers proprios.

## Correcoes e limpeza

- README final criado na raiz com especificacao de execucao, compilacao,
  funcionalidades, persistencia e artefatos.
- README interno de `src/` alinhado com a estrutura real dos modulos.
- Artefatos de user stories, sprints, plano de testes, wireframes/mockups e
  ADRs adicionados em `docs/`.
- Historico de 100 partidas gerado em `historico.txt` e `run_history.txt`.
- Backups em `outras versoes/` mantidos para rastreabilidade.

## Observacao tecnica

A versao grafica depende de raylib e de DLLs presentes na pasta do executavel.
Ao testar em outro computador, os arquivos indicados no README devem permanecer
juntos para evitar erro de vinculo dinamico.
