<p align="center">
  <img src="assets/metadecklogo.png/" alt="Logo do MetaDeck" width="500">
</p>

O **MetaDeck** é um jogo de cartas estratégico em que o jogador avança por fases cada vez mais desafiadoras, usando combinações, escolhas táticas e adaptação para superar metas de pontuação. Durante a partida, cada decisão influencia diretamente o desempenho, tornando cada rodada única e dinâmica.

A proposta do jogo é oferecer uma experiência envolvente e lúdica, em que estratégia, atenção e criatividade caminham juntas. Com uma atmosfera vibrante e desafios progressivos, o **MetaDeck** estimula o jogador a pensar antes de agir, explorar diferentes possibilidades e buscar a melhor forma de seguir avançando.

---

## Funcionalidade

- **Início de Partida**
- **Sistema de Fases**
- **Sistema de Perguntas**
- **BOSS**
- **Sistema de Moeda**
- **Sistema de Loja**
- **Sistema de Pacotes**
- **Coringas**
- **Cartas de Tarot**
- **Cupons**
- **Recompensas de Fase**
- **Feedback Visual**

---
## Relatorio

[Documentação do Projeto](https://docs.google.com/document/d/1QuwzkYMZ4sQCR2ggyGRRT0ve8KE0VRTHym0jKqX2Nnk/edit?usp=sharing)

---

## Backlog

<img src="Captura de tela 2026-05-28 195859.png" alt="Captura de tela do MetaDeck" width="600">

--- 
## Ferramentas Utilizadas

🔗 [Trello](https://trello.com/b/peA1EPFt/projeto-interno)  
🎨 [Figma](https://www.figma.com/design/ni9lD5vNeYUJGzGCVwKJI0/MetaDeck?node-id=0-1&t=eL88baV89WgyTCQP-1)

---

## Demonstração do Projeto

[🎥 Demonstração do Projeto](https://drive.google.com/file/d/15oD4ilm6bAYJCUN9YIHtGHEEcQMHdxNu/view?usp=drive_link)

---

### Diagrama de Atividade

🔗 [Diagrama](https://www.figma.com/board/oRGYxgpozYA9ofm71KWhNC/Diagrama-de-atividades?node-id=0-1&t=cdALj8wPUNesLKAL-1)

---

## Issue/bug tracker

[Issue/Bug Tracker](https://github.com/egs3-coder/Projeto-Interno/issues)

---

## Programação em Par Experimentada

# Relato de Programação Par a Par — MetaDeck

## Visão Geral

Durante o desenvolvimento do **MetaDeck**, a equipe utilizou uma abordagem de **programação par a par**, organizando os integrantes em duplas de trabalho para desenvolver, revisar, testar e validar as funcionalidades principais do jogo.

A divisão foi feita considerando as funções de cada integrante:

# Resumo Geral da Programação Par a Par

| Par | Integrantes | Área principal | Principais entregas |
|---|---|---|---|
| Par 1 | Ewerton e Lucas | Back-end | Partida, fases, perguntas, Boss, moedas, coringas, tarots, cupons e recompensas |
| Par 2 | Mateus e Davi | Front-end | Menu, perfil, partida, loja, coleção, perguntas, ranking, histórico e telas finais |
| Par 3 | Lauan e Aquiles | Gestão e QA | Organização, acompanhamento, testes, validação de fluxo e identificação de melhorias |
| Par 4 | João Ricardo com os demais pares | Produto | Validação da proposta, experiência do jogador e priorização das funcionalidades |

A programação par a par foi aplicada principalmente nas seguintes frentes:

- Back-end da partida.
- Front-end das telas.
- Testes e validação das funcionalidades.
- Revisão do fluxo do jogador.
- Organização das entregas no GitHub.
- Validação da experiência final do produto.

---

# Par 1 — Ewerton Guilherme da Silva e Lucas Aprígio dos Santos

## Área de atuação

**Back-end / Lógica do jogo**

## Funções no par

- **Ewerton Guilherme da Silva:** atuou na lógica principal da partida, fases, pontuação, moeda e recompensas.
- **Lucas Aprígio dos Santos:** atuou na lógica das perguntas, Boss, cartas especiais, coringas, tarots, cupons e integração dos efeitos especiais.

## Relato da atuação em par

Ewerton e Lucas trabalharam em conjunto na construção das regras internas do jogo. A dupla ficou responsável por garantir que a run funcionasse corretamente desde o início da partida até a finalização.

Ewerton focou na base da partida, controlando o início da run, avanço entre fases, metas de pontuação, moedas, recompensa da fase e validação de vitória ou derrota. Lucas complementou essa lógica implementando os sistemas educacionais e estratégicos, como perguntas, Boss, coringas, cartas de tarot e cupons.

A atuação em par foi importante porque muitas funcionalidades dependiam uma da outra. Por exemplo, o sistema de perguntas precisava interferir diretamente no estado da partida, enquanto as moedas recebidas nas fases eram usadas posteriormente na loja para comprar itens especiais. Dessa forma, a dupla precisou alinhar a lógica de progressão, recompensa e efeitos especiais.

## Funcionalidades relacionadas

- Início de Partida.
- Sistema de Fases.
- Sistema de Perguntas.
- BOSS.
- Sistema de Moeda.
- Coringas.
- Cartas de Tarot.
- Cupons.
- Recompensas de Fase.
- Controle de vitória e derrota.
- Integração dos efeitos especiais com a partida.

## Telas relacionadas

### Menu inicial e início da run

Nesta tela, o sistema verifica se existe jogador vinculado e prepara o início da partida.

![Menu principal do MetaDeck](UH1.jpeg)

### Perfil da run

Nesta tela, o jogador informa o nome e seleciona categorias que influenciam a modalidade da run.

![Perfil da run](UH1.1.jpeg)

### Seleção de desafio

Nesta tela, o back-end controla a fase atual, a blind, a meta, a recompensa e as opções de avanço.

![Seleção de desafio](UH2.jpeg)

### Tela principal da partida

Nesta tela, aparecem pontuação, meta, moedas, mãos, descartes e estado da rodada.

![Tela principal da rodada](UH5.jpeg)

### Pergunta da fase

Nesta tela, aparece a lógica de perguntas implementada na run.

![Pergunta de fase](UH3.jpeg)

### Boss

Nesta tela, o jogador enfrenta uma pergunta especial do chefe.

![Pergunta do Boss](BOSS.jpeg)

### Recompensa da fase

Nesta tela, aparece o cálculo de moedas recebidas após vencer a fase.

![Recompensa da fase](UH11.jpeg)

### Run concluída

Nesta tela, são exibidos os dados finais da partida após vitória.

![Run concluída](UH12.jpeg)

### Fim de jogo

Nesta tela, são exibidas as informações finais quando o jogador perde a run.

![Fim de jogo](UH12.2.jpeg)

## Resumo da contribuição do par

A dupla Ewerton e Lucas foi responsável por estruturar a lógica central do MetaDeck. Enquanto Ewerton cuidou da progressão da partida, pontuação e recompensas, Lucas implementou a parte educacional e estratégica, conectando perguntas, Boss e cartas especiais ao fluxo do jogo.

---

# Par 2 — Mateus Valerino Barros de Santana e Davi Magno Campelo do Nascimento

## Área de atuação

**Front-end / Interface do jogo**

## Funções no par

- **Mateus Valerino Barros de Santana:** atuou nas telas principais da partida, menu, perfil, seleção de desafio, interface da rodada, sistema de moeda e recompensas.
- **Davi Magno Campelo do Nascimento:** atuou nas telas de loja, pacotes, coleção, perguntas, Boss, ranking, histórico, fim de jogo e run concluída.

## Relato da atuação em par

Mateus e Davi trabalharam em conjunto na construção da interface visual do MetaDeck. A dupla ficou responsável por transformar as funcionalidades do back-end em telas utilizáveis pelo jogador.

Mateus atuou mais diretamente nas telas iniciais e principais da partida, organizando o menu, perfil, seleção de desafio e tela da rodada. Ele também estruturou os painéis laterais que mostram informações importantes como score, meta, dinheiro, mãos disponíveis e descartes.

Davi atuou nas telas complementares e estratégicas do jogo, como loja, pacotes, coleção, perguntas, Boss, ranking, histórico e telas finais. Sua atuação ajudou a deixar visível para o jogador os itens especiais, cartas desbloqueadas, resultados finais e dados salvos das runs.

A atuação em par foi importante para manter uma identidade visual consistente entre todas as telas. A dupla precisou alinhar cores, botões, painéis, fontes, mensagens e organização das informações para que o jogo tivesse unidade visual.

## Funcionalidades relacionadas

- Interface do Início de Partida.
- Interface do Sistema de Fases.
- Interface do Sistema de Moeda.
- Interface das Recompensas de Fase.
- Sistema de Loja.
- Sistema de Pacotes.
- Interface dos Coringas.
- Interface das Cartas de Tarot.
- Interface dos Cupons.
- Feedback Visual.
- Tela de Coleção.
- Tela de Ranking.
- Tela de Histórico.
- Tela de Run Concluída.
- Tela de Fim de Jogo.

## Telas relacionadas

### Menu principal

Mateus atuou na organização da interface inicial, botões principais, painel do jogador e mensagens inferiores.

![Menu principal](UH1.jpeg)

### Perfil da run

Mateus atuou na criação da tela de perfil, campo de nome, seleção de categorias e exibição da modalidade detectada.

![Perfil da run](UH1.1.jpeg)

### Seleção de desafio

Mateus organizou visualmente os painéis de blind, marca da rodada e atalho por quiz.

![Seleção de desafio](UH2.jpeg)

### Tela principal da rodada

Mateus trabalhou na organização dos painéis laterais, cartas, botões de jogar mão, descarte, classe e barra de mensagens.

![Tela principal da rodada](UH5.jpeg)

### Loja

Davi atuou na tela da loja, organizando itens, cartas, pacotes, preços, atualização e botão de próxima rodada.

![Loja](UH6.jpeg)

### Coleção de coringas

Davi atuou na interface da coleção, separando os tipos de cartas por abas e organizando a navegação.

![Coleção de coringas](UH8.jpeg)

![Coleção de coringas com navegação](UH7.png)

### Coleção de tarots

Davi organizou a visualização das cartas de tarot e a navegação entre páginas.

![Coleção de tarots](UH9.png)

### Coleção de cupons

Davi organizou a aba de cupons, exibindo quantidade desbloqueada e cartas disponíveis.

![Coleção de cupons](UH10.png)

### Pergunta de fase

Davi atuou na exibição visual da pergunta, alternativas, dificuldade, categoria e mensagem de orientação.

![Pergunta de fase](UH3.jpeg)

### Pergunta do Boss

Davi também trabalhou na interface da pergunta do chefe, diferenciando visualmente esse desafio especial.

![Pergunta do Boss](BOSS.jpeg)

### Run concluída

Davi organizou a tela final de vitória, exibindo pontuação, análise, estatísticas e botões de navegação.

![Run concluída](UH12.jpeg)

### Fim de jogo

Davi organizou a tela de derrota, exibindo estatísticas da tentativa e opções de nova tentativa, histórico e menu.

![Fim de jogo](UH12.2.jpeg)

### Histórico por jogador

Davi atuou na interface de histórico, listando runs anteriores, vitórias, pontuações e resultados.

![Histórico por jogador](UH12.1.jpeg)

### Ranking por modalidade

Davi organizou a tela de ranking, exibindo o Top 10 por modalidade.

![Ranking por modalidade](UH12.3.jpeg)

## Resumo da contribuição do par

A dupla Mateus e Davi foi responsável pela experiência visual do MetaDeck. Mateus concentrou sua atuação nas telas principais da partida, enquanto Davi desenvolveu as telas de loja, coleção, perguntas e resultados. Juntos, garantiram que o jogo tivesse uma interface completa, navegável e coerente com o estilo visual proposto.

---

# Par 3 — Lauan Gonçalves dos Santos e Aquiles Pereira dos Santos

## Área de atuação

**Gestão, acompanhamento e testes**

## Funções no par

- **Lauan Gonçalves dos Santos:** atuou como Scrum Master, organizando tarefas, acompanhando entregas e auxiliando na comunicação da equipe.
- **Aquiles Pereira dos Santos:** atuou como Testes / QA, validando funcionalidades, telas, fluxo do jogador e problemas visuais.

## Relato da atuação em par

Lauan e Aquiles atuaram em conjunto na organização e validação do projeto. Enquanto Lauan acompanhava o andamento das tarefas e ajudava a manter a equipe organizada, Aquiles testava as funcionalidades implementadas e verificava se o jogo estava funcionando corretamente.

A dupla acompanhou o desenvolvimento das telas e funcionalidades, verificando se cada parte estava de acordo com o planejamento do produto. Lauan organizou a divisão das tarefas e ajudou no acompanhamento das issues do GitHub. Aquiles validou as telas do ponto de vista do usuário, analisando jogabilidade, navegação, mensagens, botões, alinhamento visual e comportamento das mecânicas.

Essa atuação em par foi importante para identificar pendências, bugs visuais e ajustes necessários antes da entrega final.

## Funcionalidades acompanhadas e testadas

- Início de Partida.
- Sistema de Fases.
- Sistema de Perguntas.
- BOSS.
- Sistema de Moeda.
- Sistema de Loja.
- Sistema de Pacotes.
- Coringas.
- Cartas de Tarot.
- Cupons.
- Recompensas de Fase.
- Feedback Visual.

## Telas analisadas

### Menu principal

A dupla acompanhou o fluxo inicial, verificando se os botões estavam acessíveis e se o jogador conseguia iniciar a experiência corretamente.

![Menu principal](UH1.jpeg)

### Perfil da run

Foi analisado se o jogador conseguia informar o nome, selecionar categorias e salvar o perfil.

![Perfil da run](UH1.1.jpeg)

### Seleção de desafio

A dupla verificou se a fase, meta, recompensa e opções de jogar fase ou pular por quiz estavam compreensíveis.

![Seleção de desafio](UH2.jpeg)

### Tela principal da partida

Foram analisados os painéis de score, meta, dinheiro, descartes, mãos e botões de ação.

![Tela principal da partida](UH5.jpeg)

### Pergunta de fase

Aquiles testou a exibição das perguntas, alternativas, dificuldade e retorno visual após resposta.

![Pergunta de fase](UH3.jpeg)

### Pergunta do Boss

A dupla analisou a tela especial do Boss e a clareza das instruções para o jogador.

![Pergunta do Boss](BOSS.jpeg)

### Loja

Foram testados os itens da loja, preços, botão de atualizar, próxima rodada e compra de cartas/pacotes.

![Loja](UH6.jpeg)

### Coleção

A dupla verificou a navegação entre abas, exibição das cartas e organização dos itens desbloqueados.

![Coleção de coringas](UH8.jpeg)

![Coleção de tarots](UH9.png)

![Coleção de cupons](UH10.png)

### Telas finais

Foram analisadas as telas de run concluída, fim de jogo, histórico e ranking para verificar se os resultados estavam claros.

![Run concluída](UH12.jpeg)

![Fim de jogo](UH12.2.jpeg)

![Histórico por jogador](UH12.1.jpeg)

![Ranking por modalidade](UH12.3.jpeg)

## Problemas observados durante a validação

Durante os testes e acompanhamento, foram observados alguns pontos de melhoria:

- Alguns textos aparecem sobrepostos em telas com muitas informações.
- Certos elementos visuais ficam próximos demais em resoluções menores.
- Algumas descrições longas ultrapassam os limites dos painéis.
- A tela de ranking e histórico pode ficar visualmente carregada.
- A coleção possui muitos itens e pode precisar de melhor espaçamento.
- Algumas mensagens de feedback poderiam permanecer por mais tempo na tela.

## Resumo da contribuição do par

A dupla Lauan e Aquiles garantiu que o desenvolvimento seguisse organizado e que as funcionalidades fossem testadas antes da entrega. Lauan atuou no acompanhamento das tarefas e organização do fluxo de trabalho, enquanto Aquiles validou o jogo na prática, identificando problemas visuais, falhas de navegação e pontos de melhoria.

---

# Par 4 — João Ricardo Alves de Brito com os demais pares

## Área de atuação

**Product Owner / Validação do produto**

## Função no projeto

- **João Ricardo Alves de Brito:** atuou como Product Owner, validando a proposta do jogo, as funcionalidades principais e a experiência do jogador.

## Relato da atuação com os pares

João Ricardo atuou em ligação com os demais pares da equipe. Como Product Owner, sua função foi garantir que o jogo mantivesse a proposta principal: unir mecânicas de cartas, estratégia e perguntas educacionais em uma experiência jogável.

Com o par de back-end, João validou se as regras da partida, fases, perguntas, Boss, moedas e cartas especiais faziam sentido dentro da proposta do jogo. Com o par de front-end, validou se as telas estavam compreensíveis para o jogador e se as informações principais estavam sendo exibidas corretamente. Com o par de gestão e QA, acompanhou os testes e ajudou a priorizar melhorias.

Sua atuação foi importante para manter o foco do produto, garantindo que o MetaDeck não fosse apenas um jogo de cartas, mas também uma ferramenta de aprendizado com progressão, desafios e feedback visual.

## Funcionalidades validadas

- Início de Partida.
- Sistema de Fases.
- Sistema de Perguntas.
- BOSS.
- Sistema de Moeda.
- Sistema de Loja.
- Sistema de Pacotes.
- Coringas.
- Cartas de Tarot.
- Cupons.
- Recompensas de Fase.
- Feedback Visual.

## Telas validadas

### Fluxo inicial

João validou se o jogador conseguia compreender o menu, abrir o perfil e iniciar a run.

![Menu principal](UH1.jpeg)

![Perfil da run](UH1.1.jpeg)

### Fluxo de partida

João validou se a seleção de desafio, fase e tela principal apresentavam informações suficientes para o jogador tomar decisões.

![Seleção de desafio](UH2.jpeg)

![Tela principal da partida](UH5.jpeg)

### Fluxo educacional

João validou se as perguntas e o Boss estavam alinhados com a proposta educacional do jogo.

![Pergunta de fase](UH3.jpeg)

![Pergunta do Boss](BOSS.jpeg)

### Fluxo estratégico

João validou se loja, pacotes, coringas, tarots e cupons adicionavam estratégia à run.

![Loja](UH6.jpeg)

![Coleção de coringas](UH8.jpeg)

![Coleção de tarots](UH9.png)

![Coleção de cupons](UH10.png)

### Fluxo final

João validou se as telas finais comunicavam corretamente o desempenho do jogador.

![Run concluída](UH12.jpeg)

![Fim de jogo](UH12.2.jpeg)

![Histórico por jogador](UH12.1.jpeg)

![Ranking por modalidade](UH12.3.jpeg)

## Resumo da contribuição

João Ricardo atuou como ligação entre a proposta do produto e o trabalho dos pares. Sua validação ajudou a garantir que cada funcionalidade tivesse sentido dentro do jogo e que as telas fossem coerentes com a experiência esperada.

---

# Conclusão

A programação par a par permitiu que o desenvolvimento do MetaDeck fosse dividido de forma organizada entre lógica, interface, testes e validação do produto.

O par de back-end garantiu o funcionamento das regras do jogo. O par de front-end transformou essas regras em telas navegáveis e visuais. O par de gestão e QA acompanhou o desenvolvimento e validou a experiência do usuário. O Product Owner atuou em ligação com todos os pares para garantir que o jogo mantivesse sua proposta educacional e estratégica.

Dessa forma, o MetaDeck foi desenvolvido com colaboração entre diferentes funções, integrando programação, design de interface, testes e visão de produto.

---

## Equipe

A equipe do **MetaDeck** foi organizada de forma colaborativa, distribuindo responsabilidades entre planejamento, prototipação, desenvolvimento, testes e apoio à documentação do projeto.

<table>
  <thead>
    <tr>
      <th>Foto</th>
      <th>Integrante</th>
      <th>Função</th>
      <th>Descrição</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td align="center">
        <img src="assets/ewerton.jpeg" width="100" height="100" alt="Foto de Ewerton Guilherme da Silva">
      </td>
      <td><strong>Ewerton Guilherme da Silva</strong></td>
      <td><strong>Desenvolvedor Back-end</strong></td>
      <td>Atuação no planejamento do projeto, organização das ideias principais e contribuição nas decisões relacionadas à estrutura e desenvolvimento do sistema.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="assets/lauan.jpeg" width="100" height="100" alt="Foto de Lauan Gonçalves dos Santos">
      </td>
      <td><strong>Lauan Gonçalves dos Santos</strong></td>
      <td><strong>Scrum Master</strong></td>
      <td>Responsável pelo apoio à organização visual do projeto, prototipação das telas e representação dos fluxos e interfaces do jogo.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="assets/davi.jpeg" width="100" height="100" alt="Foto de Davi Magno Campelo do Nascimento">
      </td>
      <td><strong>Davi Magno Campelo do Nascimento</strong></td>
      <td><strong>Desenvolvedor Front-end</strong></td>
      <td>Contribuiu com a construção das interações visíveis ao jogador, organização dos menus, mensagens e navegação do sistema.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="assets/IMG-20260423-WA0142.jpg" width="100" height="100" alt="Foto de Aquiles Pereira dos Santos - adicionar depois">
      </td>
      <td><strong>Aquiles Pereira dos Santos</strong></td>
      <td><strong>Testes / QA</strong></td>
      <td>Responsável pela validação das funcionalidades, testes do sistema e verificação do comportamento esperado das mecânicas implementadas.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="assets/joao.jpeg" width="100" height="100" alt="Foto de João Ricardo Alves de Brito">
      </td>
      <td><strong>João Ricardo Alves de Brito</strong></td>
      <td><strong>Product Owner</strong></td>
      <td>Atuação no apoio à lógica interna da aplicação, organização de dados, regras do sistema e funcionamento das principais mecânicas.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="assets/mateus.jpeg" width="100" height="100" alt="Foto de Mateus Valerino Barros de Santana">
      </td>
      <td><strong>Mateus Valerino Barros de Santana</strong></td>
      <td><strong>Desenvolvedor Front-end</strong></td>
      <td>Contribuiu com a construção das telas, apresentação das informações ao jogador e melhoria da experiência durante a execução do jogo.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="assets/lucas.jpeg" width="100" height="100" alt="Foto de Lucas Aprígio dos Santos">
      </td>
      <td><strong>Lucas Aprígio dos Santos</strong></td>
      <td><strong>Desenvolvedor Back-end</strong></td>
      <td>Apoio na implementação das funcionalidades internas do sistema, estrutura de suporte da aplicação e organização do funcionamento geral do projeto.</td>
    </tr>
  </tbody>
</table>

---


