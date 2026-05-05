# PIF 2026.1 — Jogo de Cartas + Adivinhacao

Base em C11 para o Projeto Integrador 2026.1. O projeto junta:

1. **Requisitos PIF do jogo de adivinhacao**: alvo aleatorio 1..100, dicas "Muito baixo", "Muito alto" e "Acertou", persistencia em arquivo texto, leitura de historico, estatisticas e sugestoes.
2. **Projeto interno do jogo de cartas**: fases com metas crescentes, maos de cartas, pontuacao por combinacoes, quiz, Boss com debuff, moedas, loja, pacotes, coringas, tarot, cupons e feedback textual.

## Como compilar

### Linux/macOS/WSL

```bash
make
./build/jogo
```

### Sem Makefile

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/cards.c src/history.c src/analysis.c src/pif_guess.c src/quiz.c src/shop.c src/game.c -lm -o jogo
./jogo
```

### Windows com MinGW

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c src/cards.c src/history.c src/analysis.c src/pif_guess.c src/quiz.c src/shop.c src/game.c -lm -o jogo.exe
jogo.exe
```

## GUI opcional com raylib

O MVP principal nao depende de bibliotecas externas, para manter compatibilidade com C11. Mesmo assim, foi deixada uma base visual opcional em `src/raylib_main.c`.

Para compilar a GUI, instale a raylib e rode:

```bash
make raylib
./build/jogo_raylib
```

Essa GUI e apenas uma base de tela/click em cartas. A run completa fica no executavel console, pronta para evoluir depois com raylib/raygui.

## Menu do programa

- `1) Jogar run de cartas do Projeto Interno`
- `2) Jogar adivinhacao PIF e salvar historico`
- `3) Analisar historico - Capstone 2`
- `0) Sair`

## Formato do historico

O arquivo `history.txt` segue o formato pedido:

```text
timestamp;alvo;tentativas;baixos;altos;palpites_csv
```

Exemplo:

```text
2026-03-31 16:35:10;42;7;4;2;10,80,50,40,45,43,42
```

## Cronograma alinhado aos Capstones

### Semana 1 — RNG

Implementado em:

- `cards.c`: embaralhamento do deck com `rand`
- `pif_guess.c`: alvo aleatorio de 1 a 100
- `quiz.c` e `shop.c`: escolhas aleatorias de perguntas, coringas, tarot e pacotes

### Semana 2 — Loop de jogo e persistencia

Implementado em:

- `main.c`: menu principal
- `pif_guess.c`: loop de adivinhacao
- `history.c`: salvar e carregar `history.txt`
- `game.c`: loop principal das fases de cartas

### Capstone 1 — Jogo funcional + historico basico

Entregavel:

- Menu jogar/analisar/sair
- Adivinhacao funcional com dicas
- Historico salvo e carregado
- Run de cartas jogavel

### Semana 4 — Recursao soma/min/max

Implementado em `analysis.c`:

- `rec_sum_attempts`
- `rec_min_attempts`
- `rec_max_attempts`

### Semana 5 — Desvio padrao recursivo

Implementado em `analysis.c`:

- `rec_sum_sq_diff`
- `sqrt` para desvio padrao populacional

### Capstone 2 — Analise completa com sugestoes

Implementado em `analysis.c`:

- Total de sessoes
- Media de tentativas
- Melhor e pior sessao
- Desvio padrao
- Vies medio para baixo/alto
- Heuristicas textuais: chute inicial distante, busca linear, estrategia repetitiva e aproximacao de busca binaria

### Semana 7 — Integracao completa

Implementado em:

- `game.c`: integra cartas + fases + quiz + Boss + loja
- `shop.c`: moedas, coringas, tarot, pacotes e cupons
- `quiz.c`: perguntas entre fases e Boss

### Capstone 3 — Projeto polido e documentado

Entregavel esperado:

- Refatorar nomes e mensagens finais
- Completar este README com autores e prints
- Testar com 100+ sessoes no historico
- Subir no GitHub com commits de todos os integrantes
- Adicionar o professor como colaborador: `mrcostaalencar`

## Mapa de funcionalidades do Projeto Interno

- Card 1: loop principal de cartas em `game.c`
- Card 2: fases e metas crescentes em `play_phase`
- Card 3: quiz rapido em `quiz.c`
- Card 4: Boss com debuff em `play_phase`
- Card 5: moedas em `PlayerBuild`
- Card 6: loja em `shop.c`
- Card 7: pacotes em `open_pack`
- Card 8: coringas em `JokerType` e `calculate_play_score`
- Card 9: tarot em `use_tarot`
- Card 10: cupons em `post_phase_quiz_and_rewards`
- Card 11: recompensas de fase em `play_phase`
- Card 12: integracao quiz + gameplay em `post_phase_quiz_and_rewards`
- Card 13: feedback visual textual em `print_score_table`, `print_hand` e mensagens de pontuacao
- Card 14: balanceamento inicial nos custos, metas e efeitos moderados de cupons

## Sugestao de commits no GitHub

Cada integrante pode assumir um modulo:

- Integrante 1: `main.c`, `pif_guess.c`
- Integrante 2: `history.c`, `analysis.c`
- Integrante 3: `cards.c`, `game.c`
- Integrante 4: `quiz.c`, `shop.c`
- Integrante 5: `raylib_main.c`, README e testes
