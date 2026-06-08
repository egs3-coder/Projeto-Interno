# Plano de Testes

## Objetivo

Validar as funcionalidades principais do jogo: execucao, run, quiz, historico, ranking, colecao e compatibilidade em notebooks.

## Casos de Teste

| ID | Cenario | Procedimento | Resultado esperado |
| --- | --- | --- | --- |
| T01 | Abrir jogo | Executar `rodar_jogo_gui.bat` | Janela do jogo abre sem erro de DLL |
| T02 | Configurar jogador | Informar nome e selecionar categorias | Perfil ativo aparece no menu |
| T03 | Iniciar run | Clicar em jogar | Tela de blind ou pre-blind aparece |
| T04 | Jogar mao | Selecionar cartas e jogar | Pontuacao da fase aumenta |
| T05 | Descartar | Selecionar cartas e descartar | Cartas sao substituidas e descarte reduz |
| T06 | Quiz correto | Responder alternativa correta | Feedback positivo e recompensa aplicada |
| T07 | Quiz errado | Responder alternativa errada | Dica aparece e penalidade/recompensa reduzida e aplicada |
| T08 | Boss | Chegar em Blind Chefe | Quiz de boss aparece antes da fase |
| T09 | Historico | Finalizar run | Registro aparece em `run_history.txt` |
| T10 | Ranking | Abrir ranking | Top 10 por modalidade e carregado |
| T11 | Sessoes | Buscar jogador | Runs do jogador aparecem |
| T12 | Colecao | Abrir colecao | Itens descobertos aparecem desbloqueados |
| T13 | Notebook | Rodar em 1368x700 | Janela cabe na tela e cliques continuam alinhados |

## Massa de Teste

O arquivo `historico.txt` contem 100 partidas simuladas para validacao de relatorio, ranking e sessoes.

## Criterios de Aceite

- O projeto compila com GCC usando o comando documentado.
- O jogo abre pela raiz do projeto.
- Ha README final atualizado.
- Ha historico com 100 sessoes.
- As telas de historico/ranking conseguem ler os registros salvos.
