<p align="center">
  <img src="assets/metadecklogo.png" alt="Logo do MetaDeck" width="420">
</p>

# MetaDeck — Projeto Interno / Capstone 3

**MetaDeck** é um jogo de cartas estratégico desenvolvido em C. O jogador avança por fases, monta combinações de cartas, responde perguntas educacionais, compra melhorias na loja e tenta alcançar a maior pontuação possível em cada run.

Esta versão foi organizada para a entrega do **Capstone 3 (09/06/2026)**, com código limpo, documentação final, plano de testes, user stories, artefatos de sprint, wireframes/mockups, ADRs e histórico com 100 sessões.

---

## Status da entrega

| Critério solicitado | Status | Evidência no projeto |
| --- | --- | --- |
| Refatoração final | Atendido | Código reorganizado em módulos `.c`/`.h` e limpeza de comentários desnecessários em `src/` |
| README com especificação | Atendido | Este `README.md` e `docs/README.md` |
| Relatório final atualizado | Atendido | `docs/relatorio_final.md` e `docs/relatorio_conformidade.md` |
| Testes com 100+ sessões | Atendido | `history.txt` com 100 sessões e `run_history_raylib.txt` com histórico da versão gráfica |
| Código limpo e documentado | Atendido | Headers em `src/`, documentação em `docs/` e plano de testes |
| Artefatos do projeto | Atendido | `docs/user_stories.md`, `docs/sprints_tarefas.md`, `docs/plano_de_testes.md`, `docs/wireframes_mockups.md`, `docs/adr.md` |

---

## Funcionalidades principais

- Início de partida com perfil do jogador.
- Sistema de fases com metas de pontuação.
- Blinds comuns, Big Blind e Blind Chefe/Boss.
- Sistema de perguntas por categoria e dificuldade.
- Pontuação baseada em combinações de cartas.
- Sistema de moedas.
- Loja com coringas, cartas de tarot, cupons e pacotes.
- Recompensas de fase.
- Coleção persistente por jogador.
- Ranking e histórico de partidas.
- Tela final com resumo da run.
- Histórico em arquivo texto para análise e testes.

---

## Como executar a versão gráfica

No Windows, mantenha os arquivos da raiz juntos e execute:

```powershell
.\jogo_gui.exe
```

Também é possível usar os scripts auxiliares:

```powershell
.\rodar_jogo_gui_dpi.bat
.\rodar_raylib.ps1
```

Arquivos importantes para a versão gráfica:

- `jogo_gui.exe`
- `libraylib.dll`
- `glfw3.dll`
- `libgcc_s_seh-1.dll`
- `libstdc++-6.dll`
- `libwinpthread-1.dll`
- `assets/`
- `mouse_calibration.ini`

---

## Como compilar a versão console

A versão console não depende da raylib. Ela pode ser compilada com GCC:

```bash
make
```

Para executar:

```bash
./build/jogo
```

Para validar build e histórico:

```bash
make check
```

Para limpar apenas os arquivos gerados de compilação:

```bash
make clean
```

---

## Como compilar a versão raylib

A versão gráfica exige que a biblioteca raylib e o arquivo `raylib.h` estejam instalados no ambiente de compilação:

```bash
make raylib
```

Em ambiente Windows/MSYS2, também é possível usar os scripts `.bat` já incluídos na raiz do projeto.

---

## Estrutura do projeto

```text
Projeto-Interno/
├── README.md
├── Makefile
├── history.txt
├── historico.txt
├── run_history_raylib.txt
├── jogo_gui.exe
├── assets/
├── docs/
├── src/
│   ├── analysis.c / analysis.h
│   ├── cards.c / cards.h
│   ├── game.c / game.h
│   ├── history.c / history.h
│   ├── main.c
│   ├── pif_guess.c / pif_guess.h
│   ├── quiz.c / quiz.h
│   ├── raylib_main.c
│   ├── raylib_question_bank.h
│   ├── shop.c / shop.h
│   └── player_data.h
├── tools/
│   └── gerar_history.py
└── tests/
    └── validacao_capstone3.md
```

---

## Histórico e testes

O arquivo `history.txt` contém **100 sessões** no formato usado pelo módulo `src/history.c`:

```text
data/hora;alvo;tentativas;palpites_baixos;palpites_altos;lista_de_palpites
```

Esse arquivo atende ao critério de volume de testes da entrega. O arquivo `historico.txt` foi mantido como cópia de compatibilidade, caso o avaliador procure pelo nome antigo.

A versão gráfica usa o arquivo `run_history_raylib.txt`, que também contém registros reais e simulados de runs da interface raylib.

---

## Documentação da entrega

A documentação final fica na pasta `docs/`:

| Arquivo | Conteúdo |
| --- | --- |
| `docs/README.md` | Índice da documentação |
| `docs/relatorio_final.md` | Relatório final do projeto |
| `docs/relatorio_conformidade.md` | Conferência dos critérios do Capstone 3 |
| `docs/refatoracao_final.md` | O que foi limpo e reorganizado |
| `docs/plano_de_testes.md` | Plano de testes e critérios de aceite |
| `docs/user_stories.md` | Histórias de usuário |
| `docs/sprints_tarefas.md` | Sprints, tarefas e responsáveis |
| `docs/wireframes_mockups.md` | Wireframes e referências visuais |
| `docs/adr.md` | Architecture Decision Records |
| `docs/programacao_par.md` | Relato de programação em par |

---

## Programação em par

Durante o desenvolvimento, a equipe trabalhou em pares para dividir responsabilidades e validar entregas:

| Par | Integrantes | Foco principal |
| --- | --- | --- |
| Par 1 | Ewerton Guilherme da Silva e Lucas Aprígio dos Santos | Back-end, lógica da partida, perguntas, Boss, moedas e recompensas |
| Par 2 | Mateus Valerino Barros de Santana e Davi Magno Campelo do Nascimento | Front-end, telas, loja, coleção, feedback visual e telas finais |
| Par 3 | Lauan Gonçalves dos Santos e Aquiles Pereira dos Santos | Gestão, QA, validação de fluxo e testes |
| Produto | João Ricardo Alves de Brito com os pares | Product Owner, validação da experiência e priorização |

O relato detalhado está em `docs/programacao_par.md`.

---

## Equipe

| Integrante | Função |
| --- | --- |
| Ewerton Guilherme da Silva | Desenvolvimento back-end e integração |
| Lucas Aprígio dos Santos | Desenvolvimento back-end e perguntas |
| Mateus Valerino Barros de Santana | Desenvolvimento front-end |
| Davi Magno Campelo do Nascimento | Desenvolvimento front-end |
| Lauan Gonçalves dos Santos | Scrum Master e organização |
| Aquiles Pereira dos Santos | Testes / QA |
| João Ricardo Alves de Brito | Product Owner |

---

## Observações finais

- Os arquivos `.h` foram mantidos para organizar as interfaces dos módulos principais.
- Comentários de desenvolvimento que não agregavam documentação foram removidos dos arquivos próprios em `src/`.
- O executável gráfico foi preservado para facilitar a avaliação em Windows.
- A pasta `docs/` reúne os artefatos exigidos pelo professor.
