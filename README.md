# Jogo PIF Capstone - Base Raylib

Esta e a base visual em `raylib` usada para a apresentacao do jogo.

O executavel principal desta versao e o `jogo_gui.exe`, compilado a partir de:

```powershell
gcc -std=c11 -Wall -Wextra -pedantic -Isrc src/raylib_main.c src/cards.c src/shop.c -lraylib -lm -o jogo_gui.exe
```

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
- Relatorio analitico no fim da tentativa e na tela de historico, usando funcoes recursivas para soma, minimo, maximo e soma dos quadrados

## Como rodar na sua maquina

### Opcao mais simples

Use:

```powershell
powershell -ExecutionPolicy Bypass -File .\rodar_raylib.ps1
```

Ou clique em:

- `rodar_raylib.bat`

O script:

- recompila se os fontes estiverem mais novos que o `jogo_gui.exe`
- abre o executavel pronto no fim

### Rodando direto o executavel

Se o `jogo_gui.exe` ja estiver presente e funcionando:

```powershell
.\jogo_gui.exe
```

### Recompilando manualmente

```powershell
gcc -std=c11 -Wall -Wextra -pedantic -Isrc src/raylib_main.c src/cards.c src/shop.c -lraylib -lm -o jogo_gui.exe
.\jogo_gui.exe
```

## Como compartilhar com a equipe

Para quem so precisa jogar e apresentar, envie a pasta com estes arquivos:

- `jogo_gui.exe`
- `rodar_raylib.ps1`
- `rodar_raylib.bat`
- `src/player_data.h`
- `src/raylib_question_bank.h`
- `src/raylib_main.c`

Se a outra maquina nao for recompilar, normalmente basta abrir:

```powershell
.\jogo_gui.exe
```

Se a equipe quiser recompilar, a maquina precisa ter:

- `gcc`
- `raylib`

## Arquivos importantes

- `src/raylib_main.c`: interface, fluxo da run e telas do raylib
- `src/player_data.h`: jogador atual, modalidades, ranking e historico
- `src/raylib_question_bank.h`: perguntas por categoria e dificuldade
- `src/cards.c` e `src/shop.c`: mecanicas base da run
- `run_history_raylib.txt`: historico salvo automaticamente
- `historico.txt`: massa de teste com 100 sessoes sinteticas
- `assets/README.md`: padrao de nomes para assets opcionais

## Assets opcionais

O jogo tenta carregar imagens em `assets/` para cartas, coringas, tarots, cupons, pacotes, aprimoramentos e o personagem provocador.

Se algum arquivo nao existir, a GUI usa o desenho proprio em `raylib`, entao o executavel continua abrindo normalmente.

Consulte `assets/README.md` para os nomes esperados.

## Fluxo recomendado para apresentar

1. Abrir `rodar_raylib.bat`
2. Cadastrar o jogador em `Perfil`
3. Escolher as categorias de pergunta
4. Jogar a run
5. Ao terminar, mostrar:
   - a pontuacao final
   - a modalidade detectada
   - a posicao no ranking
   - o historico do jogador
