# Plano de Testes

## Build

- Compilar com `gcc -std=c11 -Wall -Wextra -pedantic -Isrc src/raylib_main.c src/cards.c src/shop.c -lraylib -lm -o jogo_gui.exe`.
- Abrir `jogo_gui.exe`.
- Abrir por `rodar_raylib.ps1`.

## Fluxo principal

- Cadastrar jogador e categorias.
- Iniciar run.
- Jogar Small Blind, Big Blind e Boss.
- Comprar coringas, tarots, cupons e pacotes.
- Encerrar run por derrota e por vitoria.

## Informacoes da tentativa

- Abrir `Info tentativa` durante fase.
- Verificar aba de maos de poker e tooltips.
- Verificar aba de blinds.
- Responder perguntas e conferir aba de quiz.

## Regras novas

- Errar pergunta e conferir repeticao na proxima pergunta.
- Tentar comprar cupom sem blind jogada ou sem duas respostas corretas.
- Acertar pulo de fase e validar recompensa extra.
- Selecionar cartas demais para Tarot e confirmar botao inativo.
- Aplicar Tarot e verificar carta atualizada na mao/baralho.

## Historico e relatorio

- Finalizar runs e verificar `run_history_raylib.txt`.
- Conferir ranking top 10 por modalidade.
- Conferir tela de historico por jogador.
- Validar que `historico.txt` possui 100 sessoes sinteticas.
