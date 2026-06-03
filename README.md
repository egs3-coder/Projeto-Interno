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

# Funcionalidades Desenvolvidas por Integrante

## Ewerton Guilherme da Silva — Back-end

Ewerton atuou na implementação das regras principais da partida e no controle da lógica interna do jogo.

### Funcionalidades executadas

- Início de Partida
- Sistema de Fases
- Sistema de Moeda
- Recompensas de Fase
- Controle de pontuação da run
- Validação de vitória ou derrota na fase
- Avanço entre rodadas e apostas

### Descrição da contribuição

Ewerton ficou responsável por estruturar a lógica base da partida. Sua implementação permite que o jogador inicie uma run, avance pelas fases, acumule pontuação e receba moedas conforme seu desempenho.

Nas telas de início e seleção de desafio, sua contribuição aparece na preparação dos dados da partida, como jogador vinculado, modalidade detectada, categorias selecionadas, aposta atual, blind ativa e meta de pontuação. A partir dessas informações, o sistema consegue iniciar corretamente a run e controlar o progresso do jogador.

O sistema de fases controla a meta de pontos de cada desafio, como a **Small Blind** e fases posteriores. Quando o jogador atinge a pontuação necessária, o jogo libera a recompensa da fase e permite o avanço para a próxima etapa.

Também foi implementado o controle de moedas recebidas ao final da fase, considerando recompensa da blind, mãos restantes e juros. Esse sistema alimenta diretamente a loja, permitindo que o jogador compre cartas, pacotes e melhorias.

Além disso, Ewerton contribuiu para as telas finais, nas quais são exibidos dados como pontuação total, melhor mão, mão mais jogada, quantidade de mãos jogadas, aposta alcançada, rodada, dinheiro e resultado final da run.

### Telas relacionadas

- Tela de início da partida.
- Tela de seleção de desafio.
- Tela principal da rodada.
- Tela de recompensa da fase.
- Tela de run concluída.
- Tela de fim de jogo.

![Menu principal com jogador vinculado e opções iniciais](UH1.jpeg)

![Tela de seleção de desafio da run](UH2.jpeg)

![Tela principal da rodada com pontuação, meta, dinheiro, mãos e descartes](UH5.jpeg)

![Tela de recompensa da fase com cálculo de moedas](UH11.jpeg)

![Tela de run concluída com resumo da partida](UH12.jpeg)

![Tela de fim de jogo com estatísticas da tentativa](UH12.2.jpeg)

---

## Lucas Aprígio dos Santos — Back-end

Lucas atuou no desenvolvimento dos sistemas de perguntas, boss, cartas especiais e integração das mecânicas internas com a progressão da run.

### Funcionalidades executadas

- Sistema de Perguntas
- BOSS
- Coringas
- Cartas de Tarot
- Cupons
- Integração dos efeitos especiais com a partida
- Controle das respostas corretas e incorretas

### Descrição da contribuição

Lucas ficou responsável por implementar a lógica das perguntas educacionais utilizadas durante a run. O sistema de perguntas permite que o jogador responda desafios de diferentes áreas, como lógica, matemática, programação e português.

Nas telas de perguntas, sua contribuição aparece na seleção da categoria da pergunta, definição da dificuldade, controle das alternativas, validação da resposta escolhida e aplicação dos efeitos gerados pelo acerto ou erro. As perguntas são usadas tanto como recurso de progressão quanto como forma de liberar benefícios durante a partida.

Também foi desenvolvido o sistema de **Boss**, no qual o jogador enfrenta uma pergunta especial em determinadas fases. A mecânica do chefe altera o desafio da partida, podendo bloquear ou dificultar o progresso caso o jogador erre. Lucas ficou responsável pela lógica que define quando o chefe aparece, como a pergunta é validada e qual consequência será aplicada.

Além disso, Lucas contribuiu na implementação das cartas especiais, como **Coringas**, **Cartas de Tarot** e **Cupons**. Essas cartas alteram o estado da partida, oferecem vantagens estratégicas e permitem novas formas de melhorar a pontuação ou a run.

Sua contribuição também está presente na integração entre loja, pacotes e coleção, garantindo que os itens comprados, desbloqueados ou utilizados tenham efeito dentro da partida.

### Telas relacionadas

- Tela de pergunta da fase.
- Tela de pergunta do chefe.
- Tela de seleção de desafio com Boss.
- Tela principal da rodada.
- Tela da loja.
- Tela de coleção.

![Pergunta de fase com categoria, dificuldade e alternativas](UH3.jpeg)

![Tela de pergunta do chefe com desafio especial](BOSS.jpeg)

![Seleção de desafio com Blind Chefe](UH4.png)

![Tela da loja com cartas, pacotes e itens especiais](UH6.jpeg)

![Coleção de coringas](UH8.jpeg)

![Coleção de tarots](UH9.png)

![Coleção de cupons](UH10.png)

---

## Mateus Valerino Barros de Santana — Front-end

Mateus atuou na construção das telas principais do jogo e na organização visual da experiência do jogador.

### Funcionalidades executadas

- Interface do Início de Partida
- Interface do Sistema de Fases
- Interface do Sistema de Moeda
- Interface das Recompensas de Fase
- Feedback Visual na tela principal
- Organização dos painéis da rodada

### Descrição da contribuição

Mateus foi responsável por desenvolver a parte visual das telas principais do jogo. Ele trabalhou na criação do menu inicial, tela de perfil, seleção de desafio e interface da rodada.

Na tela de menu, Mateus organizou os botões principais, painel de jogador vinculado, mensagens inferiores e identidade visual do jogo. Na tela de perfil, estruturou o campo de nome, botões de categorias, modalidade detectada e opções de salvar ou iniciar a run.

Na tela principal da partida, Mateus organizou os painéis laterais que exibem informações importantes, como pontuação, meta da fase, quantidade de mãos, descartes disponíveis, dinheiro atual e aposta em andamento.

Também contribuiu para o feedback visual da partida, garantindo que o jogador receba informações claras durante o jogo, como mensagens de fase iniciada, pontos obtidos, moedas recebidas e status da rodada.

Nas telas de recompensa, sua contribuição aparece na organização do painel de pagamento, exibindo recompensa da fase, mãos restantes, juros e total de moedas obtidas.

### Telas relacionadas

- Menu principal.
- Perfil da run.
- Seleção de desafio.
- Tela principal da partida.
- Tela de recompensa.
- Barra inferior de mensagens.

![Menu principal do jogo](UH1.jpeg)

![Tela de perfil da run com nome, categorias e modalidade](UH1.1.jpeg)

![Tela de seleção de desafio](UH2.jpeg)

![Interface principal da rodada](UH5.jpeg)

![Tela de recompensa e pagamento da fase](UH11.jpeg)

---

## Davi Magno Campelo do Nascimento — Front-end

Davi atuou no desenvolvimento das telas de loja, pacotes, coleção, perguntas e telas finais do jogo.

### Funcionalidades executadas

- Sistema de Loja
- Sistema de Pacotes
- Interface dos Coringas
- Interface das Cartas de Tarot
- Interface dos Cupons
- Feedback Visual das perguntas
- Tela de coleção
- Tela de fim de jogo e run concluída

### Descrição da contribuição

Davi ficou responsável por implementar visualmente as telas ligadas aos itens especiais e ao progresso externo da partida. Ele trabalhou na loja, onde o jogador pode comprar cartas, pacotes, vouchers e melhorias usando as moedas conquistadas.

Na tela da loja, Davi organizou a exibição dos itens disponíveis, preços, botão de próxima rodada, botão de atualizar e cartas/pacotes compráveis. Também contribuiu para a apresentação visual dos pacotes, deixando claro quais itens podem ser adquiridos durante a run.

Também desenvolveu a interface da coleção, permitindo visualizar os itens disponíveis no jogo, como coringas, cupons, marcas, chefes e cartas de tarot. A tela de coleção foi organizada com abas e navegação por páginas.

Nas telas de perguntas, Davi contribuiu para a exibição das alternativas, dificuldade, categoria da pergunta e mensagens de orientação ao jogador. Além disso, trabalhou nas telas finais, como **Run Concluída** e **Fim de Jogo**, exibindo pontuação, modalidade, melhor mão, análise e opções de nova tentativa.

Davi também participou da construção das telas de ranking e histórico, onde são apresentados dados salvos das runs, melhores pontuações, vitórias, derrotas e comparação por modalidade.

### Telas relacionadas

- Loja.
- Sistema de pacotes.
- Coleção.
- Pergunta de fase.
- Pergunta do chefe.
- Run concluída.
- Fim de jogo.

![Tela da loja com itens e pacotes](UH6.jpeg)

![Coleção de coringas com navegação](UH7.png)

![Coleção de coringas](UH8.jpeg)

![Coleção de cartas de tarot](UH9.png)

![Coleção de cupons](UH10.png)

![Pergunta de fase](UH3.jpeg)

![Pergunta do chefe](BOSS.jpeg)

![Tela de run concluída](UH12.jpeg)

![Tela de fim de jogo](UH12.2.jpeg)

![Histórico por jogador](UH12.1.jpeg)

![Ranking Top 10 por modalidade](UH12.3.jpeg)

---

## Lauan Gonçalves dos Santos — Scrum Master

Lauan atuou na organização da equipe, acompanhamento das tarefas e controle do andamento do projeto.

### Atividades executadas

- Organização das funcionalidades por prioridade.
- Acompanhamento do desenvolvimento das telas.
- Apoio na divisão das tarefas entre back-end e front-end.
- Organização das issues no GitHub.
- Acompanhamento das entregas por etapa.
- Apoio na comunicação entre os membros da equipe.
- Verificação do andamento das funcionalidades planejadas.
- Apoio na preparação do material de apresentação.

### Funcionalidades acompanhadas

- Início de Partida
- Sistema de Fases
- Sistema de Perguntas
- BOSS
- Sistema de Moeda
- Sistema de Loja
- Sistema de Pacotes
- Coringas
- Cartas de Tarot
- Cupons
- Recompensas de Fase
- Feedback Visual

### Descrição da contribuição

Lauan garantiu que as funcionalidades fossem organizadas e distribuídas entre os integrantes. Como Scrum Master, acompanhou o progresso da equipe, ajudou a identificar pendências e auxiliou na organização das entregas no GitHub.

Sua atuação foi importante para manter o projeto alinhado com o prazo e com os objetivos definidos pela equipe.

Com base nas telas desenvolvidas, Lauan acompanhou a integração entre menu, perfil, partida, perguntas, loja, coleção, ranking, histórico e telas finais. Também apoiou a organização das issues relacionadas a bugs visuais, melhorias de interface, testes de funcionalidades e validação das entregas.

Lauan também atuou no acompanhamento da documentação do projeto, garantindo que as funcionalidades fossem descritas de forma clara para apresentação e registro no GitHub.

### Telas acompanhadas

![Menu principal](UH1.jpeg)

![Perfil da run](UH1.1.jpeg)

![Sistema de fases](UH2.jpeg)

![Tela da loja](UH6.jpeg)

![Coleção](UH8.jpeg)

![Run concluída](UH12.jpeg)

---

## Aquiles Pereira dos Santos — Testes / QA

Aquiles atuou na validação das funcionalidades implementadas, realizando testes de jogabilidade, interface, fluxo de telas e comportamento das mecânicas.

### Funcionalidades testadas

- Início de Partida
- Sistema de Fases
- Sistema de Perguntas
- BOSS
- Sistema de Moeda
- Sistema de Loja
- Sistema de Pacotes
- Coringas
- Cartas de Tarot
- Cupons
- Recompensas de Fase
- Feedback Visual

### Testes realizados

- Teste de início da run com jogador vinculado.
- Teste de início da run sem jogador vinculado.
- Teste de seleção de categorias no perfil.
- Teste de avanço entre fases.
- Teste de pontuação mínima da blind.
- Teste de perguntas corretas e incorretas.
- Teste de pergunta do chefe.
- Teste de ganho de moedas.
- Teste de compra na loja.
- Teste de abertura de pacotes.
- Teste de exibição dos coringas.
- Teste de exibição das cartas de tarot.
- Teste de exibição dos cupons.
- Teste de recompensa ao vencer uma fase.
- Teste de mensagens visuais durante a partida.
- Teste das telas de fim de jogo e run concluída.

### Problemas identificados

Durante os testes, foram observados alguns pontos de melhoria:

- Alguns textos aparecem sobrepostos em telas com muitas informações.
- Certos elementos visuais ficam próximos demais em resoluções menores.
- Algumas descrições longas ultrapassam os limites dos painéis.
- A tela de ranking e histórico pode ficar visualmente carregada.
- A coleção possui muitos itens e pode precisar de melhor espaçamento.
- Algumas mensagens de feedback poderiam permanecer por mais tempo na tela.

### Descrição da contribuição

Aquiles teve papel importante na validação da experiência do usuário. Seu trabalho permitiu identificar erros visuais, problemas de alinhamento, falhas de fluxo e ajustes necessários nas funcionalidades principais do jogo.

Com base nos prints, Aquiles analisou telas como menu, perfil, seleção de desafio, tela de partida, perguntas, boss, loja, coleção, ranking, histórico, run concluída e fim de jogo. Durante a validação, verificou se os botões estavam acessíveis, se as mensagens estavam claras, se as informações da run apareciam corretamente e se o fluxo do jogador fazia sentido.

Também foram analisados comportamentos de acerto e erro em perguntas, ganho de moedas, uso da loja, visualização de coleções, resultado da run e exibição do histórico.

### Telas analisadas

![Perfil com textos e botões de categoria](UH1.1.jpeg)

![Tela de seleção de desafio](UH2.jpeg)

![Tela principal da partida](UH5.jpeg)

![Pergunta de fase](UH3.jpeg)

![Pergunta do chefe](BOSS.jpeg)

![Tela de loja](UH6.jpeg)

![Coleção de cartas](UH8.jpeg)

![Run concluída](UH12.jpeg)

![Fim de jogo](UH12.2.jpeg)

![Ranking Top 10](UH12.3.jpeg)

---

## João Ricardo Alves de Brito — Product Owner

João Ricardo atuou como Product Owner, definindo a visão do produto, validando as funcionalidades e garantindo que o jogo mantivesse sua proposta educacional.

### Atividades executadas

- Definição da proposta principal do MetaDeck.
- Validação das funcionalidades essenciais do jogo.
- Definição das prioridades do produto.
- Validação do fluxo de início da partida.
- Validação do sistema de perguntas.
- Validação da mecânica de Boss.
- Validação do uso de loja, pacotes e cartas especiais.
- Acompanhamento da experiência do jogador.
- Apoio na definição das categorias educacionais.
- Validação das telas finais e feedback visual.

### Funcionalidades validadas

- Início de Partida
- Sistema de Fases
- Sistema de Perguntas
- BOSS
- Sistema de Moeda
- Sistema de Loja
- Sistema de Pacotes
- Coringas
- Cartas de Tarot
- Cupons
- Recompensas de Fase
- Feedback Visual

### Descrição da contribuição

João Ricardo foi responsável por garantir que o jogo atendesse ao objetivo proposto: unir mecânicas de cartas com perguntas educacionais. Ele acompanhou a implementação das funcionalidades principais e validou se o fluxo da partida estava coerente para o jogador.

Sua atuação ajudou a manter o foco do projeto, garantindo que as mecânicas de jogo, perguntas, loja, recompensas e feedback visual estivessem alinhadas com a experiência esperada.

Com base nas telas desenvolvidas, João Ricardo validou se o menu inicial deixava claro o início da run, se o perfil permitia escolher nome e categorias, se as perguntas estavam adequadas ao objetivo educacional, se o Boss funcionava como desafio especial e se a loja, pacotes, coringas, tarots e cupons aumentavam a estratégia da partida.

Também validou as telas finais, como **Run Concluída**, **Fim de Jogo**, **Histórico** e **Ranking**, verificando se elas comunicavam corretamente o desempenho do jogador.

### Telas validadas

![Menu principal](UH1.jpeg)

![Perfil da run](UH1.1.jpeg)

![Pergunta de fase](UH3.jpeg)

![Pergunta do chefe](BOSS.jpeg)

![Loja](UH6.jpeg)

![Coleção de cartas especiais](UH9.png)

![Run concluída](UH12.jpeg)

![Fim de jogo](UH12.2.jpeg)

![Histórico por jogador](UH12.1.jpeg)

![Ranking por modalidade](UH12.3.jpeg)

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


