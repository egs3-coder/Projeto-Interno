# Relatorio de Conformidade - Capstone 3

Data da verificacao: 08/06/2026.

## Criterios verificados

| Criterio | Situacao | Evidencia |
| --- | --- | --- |
| Relatorio analitico funcional | Atendido | `run_history.txt` registra cada partida e e usado pela tela de sessoes/historico. |
| README com especificacao | Atendido | `README.md` na raiz do projeto. |
| Testes com 100+ sessoes | Atendido | `historico.txt` e `run_history.txt` com 100 registros de partidas. |
| Codigo com headers | Atendido | Modulos principais possuem arquivos `.h` correspondentes em `src/`. |
| Artefatos de desenvolvimento | Atendido | Pasta `docs/` com user stories, sprints, plano de testes, wireframes/mockups e ADRs. |
| Preservacao de versoes | Atendido | Executaveis legacy e pasta `outras versoes/` foram mantidos. |
| Compilacao do fonte atual | Atendido com aviso | `gcc -std=c11 -Wall -Wextra -pedantic -Isrc src/raylib_main.c src/cards.c src/shop.c -lraylib -lm -o jogo_gui_verificacao.exe`. |

## Arquivos principais da entrega

- `README.md`
- `historico.txt`
- `run_history.txt`
- `docs/user_stories.md`
- `docs/sprints_tarefas.md`
- `docs/plano_de_testes.md`
- `docs/wireframes_mockups.md`
- `docs/adr.md`
- `docs/refatoracao_final.md`

## Observacoes

O projeto mantem a identidade visual e os binarios antigos. As novas adicoes
foram feitas como documentacao, historico de teste e arquivos de apoio da
entrega, sem remover os backups existentes.

A compilacao de verificacao terminou sem erro. O unico aviso encontrado foi
uma funcao interna nao utilizada em `src/raylib_main.c`, sem impedir a geracao
do executavel de teste. O executavel temporario gerado apenas para verificar o
build foi removido para nao confundir com as versoes oficiais preservadas.
