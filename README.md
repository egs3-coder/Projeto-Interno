<p align="center">
  <img src="./metadecklogo.png" alt="Logo do MetaDeck" width="500">
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
- **Integração**
- **Feedback Visual**
- **Balanceamento**

---

## Backlog

<img src="Backlog.png" alt="Captura de tela do MetaDeck" width="600">

<br><br><br>

<p align="center">
  <img src="UH.png" alt="Captura de tela do MetaDeck" width="600">
</p>

--- 
## Ferramentas Utilizadas

🔗 [Trello](https://trello.com/b/peA1EPFt/projeto-interno)  
🎨 [Figma](https://www.figma.com/design/ni9lD5vNeYUJGzGCVwKJI0/MetaDeck?node-id=0-1&t=eL88baV89WgyTCQP-1)

---

## Demonstração do Projeto

[🎥 Demonstração do Projeto](https://drive.google.com/file/d/13AJscFJPLU1kD3o7pkONpn4DUUwXMx6R/view?usp=sharing)

---

### Diagrama de Atividade

## 1 — Início de Partida

```mermaid
flowchart TD
    A([Início]) --> B[Iniciar fase]
    B --> C[Exibir mão e tentativas]
    C --> D{Pontuar ou melhorar mão?}

    D -->|Pontuar| E[Selecionar cartas]
    E --> F[Sistema identifica combinação]
    F --> G[Calcular pontos base + bônus]
    G --> H[Exibir pontuação]
    H --> I[Remover cartas usadas da fase]
    I --> J[Comprar novas cartas]
    J --> K[Atualizar mãos e descartes]
    K --> L{Meta da fase atingida?}

    L -->|Sim| M[Encerrar fase com sucesso]
    M --> N[Avançar para próxima fase]
    N --> O([Fim])

    L -->|Não| P{Tentativas acabaram?}
    P -->|Sim| Q[Encerrar fase por falha]
    Q --> O
    P -->|Não| D

    D -->|Melhorar mão| R[Descartar cartas]
    R --> S[Comprar novas cartas]
    S --> T[Atualizar descartes]
    T --> U{Ainda há tentativas?}
    U -->|Sim| D
    U -->|Não| Q
```

## 2 — Sistema de Fases

```mermaid
flowchart TD
   A([Início]) --> B[Iniciar run]
    B --> C[Definir meta da fase]
    C --> D[Iniciar fase]
    D --> E[Jogador tenta bater a meta]
    E --> F{Meta atingida?}

    F -->|Sim| G[Conceder progressão]
    G --> H[Aumentar dificuldade]
    H --> I[Transição rápida]
    I --> J{Próxima fase disponível?}

    J -->|Sim| C
    J -->|Não| K[Finalizar run com vitória]
    K --> L([Fim])

    F -->|Não| M[Encerrar run por falha]
    M --> L
```
## 3 — Sistema de Perguntas

```mermaid
flowchart TD
   A([Início]) --> B[Entre fases]
    B --> C[Exibir pergunta rápida]
    C --> D[Jogador responde]
    D --> E{Resposta correta?}

    E -->|Sim| F[Conceder moedas extras]
    F --> G[Continuar run]
    G --> H([Fim])

    E -->|Não| I[Exibir dica]
    I --> J[Reduzir recompensa]
    J --> K[Reapresentar pergunta]
    K --> L[Jogador responde novamente]
    L --> M{Acertou?}

    M -->|Sim| N[Conceder moedas reduzidas]
    N --> G

    M -->|Não| O[Continuar run sem bônus]
    O --> H
```

## 4 — BOSS 

```mermaid
flowchart TD
    A([Início]) --> B[Verificar fase de Boss]
    B --> C[Exibir Boss e meta elevada]
    C --> D[Apresentar pergunta difícil]
    D --> E{Resposta correta?}

    E -->|Sim| F[Evitar ou reduzir penalidade]
    F --> G[Jogar fase Boss]
    G --> H{Meta atingida?}

    H -->|Sim| I[Vencer Boss]
    I --> J[Remover debuff]
    J --> K[Retornar ao fluxo normal]
    K --> L([Fim])

    H -->|Não| M[Encerrar run por falha]
    M --> L

    E -->|Não| N[Ativar debuff]
    N --> O[Aplicar penalidade]
    O --> P[Jogar fase Boss com debuff]
    P --> Q{Meta atingida?}

    Q -->|Sim| I
    Q -->|Não| M
```
## 5 — Sistema de Moeda

```mermaid
flowchart TD
    A([Início]) --> B[Concluir fase ou acertar pergunta]
    B --> C[Calcular moedas por desempenho]
    C --> D[Adicionar moedas ao saldo]
    D --> E[Exibir saldo atual]
    E --> F{Usar na loja?}

    F -->|Sim| G[Selecionar compra]
    G --> H[Descontar moedas]
    H --> I[Atualizar saldo]
    I --> J([Fim])

    F -->|Não| K[Manter saldo para próxima fase]
    K --> J
```

## 6 — Sistema de Loja

```mermaid
flowchart TD
    A([Início]) --> B[Fim da fase]
    B --> C[Abrir loja]
    C --> D[Exibir itens: Coringas, Tarot, Pacotes e Cupons]
    D --> E{Deseja comprar?}

    E -->|Não| F[Fechar loja]
    F --> G[Ir para próxima fase]
    G --> H([Fim])

    E -->|Sim| I[Selecionar item]
    I --> J{Saldo suficiente?}

    J -->|Sim| K[Descontar moedas]
    K --> L[Adicionar item ou benefício]
    L --> M{Comprar outro item?}
    M -->|Sim| D
    M -->|Não| F

    J -->|Não| N[Informar saldo insuficiente]
    N --> D
```
## 7 — Sistema de Pacotes

```mermaid
flowchart TD
    A([Início]) --> B[Obter ou comprar pacote]
    B --> C[Abrir pacote]
    C --> D[Gerar 3 ou 5 opções]
    D --> E[Exibir opções]
    E --> F[Jogador escolhe uma ou mais cartas]
    F --> G[Adicionar cartas ao inventário]
    G --> H[Atualizar build]
    H --> I([Fim])
```

## 8 — Coringas

```mermaid
flowchart TD
A([Início]) --> B[Adquirir Coringa]
    B --> C{Há slot disponível?}

    C -->|Sim| D[Equipar Coringa]
    D --> E[Aplicar efeito único]
    E --> F[Atualizar pontuação e sinergias]
    F --> G{Deseja vender?}

    G -->|Sim| H[Receber metade do valor]
    H --> I[Liberar slot]
    I --> J([Fim])

    G -->|Não| K[Manter Coringa equipado]
    K --> J

    C -->|Não| L[Escolher substituir ou cancelar]
    L --> M{Substituir?}

    M -->|Sim| N[Vender Coringa antigo por metade]
    N --> O[Equipar novo Coringa]
    O --> P[Aplicar efeito único]
    P --> Q[Atualizar pontuação e sinergias]
    Q --> J

    M -->|Não| R[Cancelar troca]
    R --> J
```
## 9 — Cartas de Tarot

```mermaid
flowchart TD
   A([Início]) --> B[Jogador possui carta de Tarot]
    B --> C{Usar carta?}

    C -->|Sim| D[Selecionar carta de Tarot]
    D --> E[Aplicar efeito imediato]
    E --> F[Consumir carta]
    F --> G[Atualizar estado da run]
    G --> H([Fim])

    C -->|Não| I[Continuar run sem usar]
    I --> H
```
## 10 — Cupons

```mermaid
flowchart TD
    A([Início]) --> B[Registrar acertos do quiz]
    B --> C{Atingiu 2 acertos?}

    C -->|Não| B
    C -->|Sim| D[Liberar cupom]
    D --> E[Exibir cupom na loja]
    E --> F{Cupom adquirido?}

    F -->|Sim| G[Ativar efeito passivo]
    G --> H[Manter ativo até o fim da run]
    H --> I([Fim])

    F -->|Não| J[Cupom permanece disponível]
    J --> I
```
## 11 — Recompensas de Fase

```mermaid
flowchart TD
   A([Início]) --> B[Completar fase]
    B --> C[Conceder moedas]
    C --> D{Há bônus extra?}

    D -->|Sim| E[Liberar pacote ou bônus]
    E --> F[Integrar resultado do quiz]
    F --> G[Desbloquear loja]
    G --> H[Continuar progressão]
    H --> I([Fim])

    D -->|Não| G
```
## 12 — Integração

```mermaid
flowchart TD
    A([Início]) --> B[Concluir fase]
    B --> C[Transição rápida]
    C --> D[Exibir quiz entre fases]
    D --> E[Calcular recompensa]
    E --> F[Enviar recompensa para a loja]
    F --> G[Abrir loja]
    G --> H{Comprar ou seguir?}

    H -->|Comprar| I[Comprar itens]
    I --> J[Iniciar próxima fase]
    J --> K([Fim])

    H -->|Seguir| J
```

## 13 — Feedback Visual

```mermaid
flowchart TD
    A([Início]) --> B[Ação do jogador ou evento do sistema]
    B --> C[Atualizar interface]
    C --> D[Mostrar mãos possíveis e tabela de pontuação]
    D --> E[Animar pontuação]
    E --> F[Exibir efeitos de Coringas]
    F --> G[Exibir feedback do quiz]
    G --> H[Jogador entende resultado]
    H --> I[Continuar jogando]
    I --> J([Fim])
```
## 14 — Balanceamento

```mermaid
flowchart TD
   A([Início]) --> B[Iniciar run balanceada]
    B --> C[Aplicar Coringas como principal fonte de poder]
    C --> D[Aplicar Tarot como recurso situacional]
    D --> E[Aplicar Cupons com impacto moderado]
    E --> F[Usar quiz apenas como vantagem]
    F --> G{Errou perguntas?}

    G -->|Sim| H[Continuar run com menor vantagem]
    H --> I[Tomar decisões estratégicas com cartas]

    G -->|Não| J[Ganhar bônus moderado]
    J --> I

    I --> K{Estratégia é o fator principal?}
    K -->|Sim| L[Manter jogo justo e evitar bola de neve]
    L --> M([Fim])

```
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
        <img src="./ewerton.jpeg" width="100" height="100" alt="Foto de Ewerton Guilherme da Silva">
      </td>
      <td><strong>Ewerton Guilherme da Silva</strong></td>
      <td><strong>Desenvolvedor Back-end</strong></td>
      <td>Atuação no planejamento do projeto, organização das ideias principais e contribuição nas decisões relacionadas à estrutura e desenvolvimento do sistema.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="./lauan.jpeg" width="100" height="100" alt="Foto de Lauan Gonçalves dos Santos">
      </td>
      <td><strong>Lauan Gonçalves dos Santos</strong></td>
      <td><strong>Scrum Master</strong></td>
      <td>Responsável pelo apoio à organização visual do projeto, prototipação das telas e representação dos fluxos e interfaces do jogo.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="./davi.jpeg" width="100" height="100" alt="Foto de Davi Magno Campelo do Nascimento">
      </td>
      <td><strong>Davi Magno Campelo do Nascimento</strong></td>
      <td><strong>Desenvolvedor Front-end</strong></td>
      <td>Contribuiu com a construção das interações visíveis ao jogador, organização dos menus, mensagens e navegação do sistema.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="./IMG-20260423-WA0142.jpg" width="100" height="100" alt="Foto de Aquiles Pereira dos Santos - adicionar depois">
      </td>
      <td><strong>Aquiles Pereira dos Santos</strong></td>
      <td><strong>Testes / QA</strong></td>
      <td>Responsável pela validação das funcionalidades, testes do sistema e verificação do comportamento esperado das mecânicas implementadas.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="./joao.jpeg" width="100" height="100" alt="Foto de João Ricardo Alves de Brito">
      </td>
      <td><strong>João Ricardo Alves de Brito</strong></td>
      <td><strong>Product Owner</strong></td>
      <td>Atuação no apoio à lógica interna da aplicação, organização de dados, regras do sistema e funcionamento das principais mecânicas.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="./mateus.jpeg" width="100" height="100" alt="Foto de Mateus Valerino Barros de Santana">
      </td>
      <td><strong>Mateus Valerino Barros de Santana</strong></td>
      <td><strong>Desenvolvedor Front-end</strong></td>
      <td>Contribuiu com a construção das telas, apresentação das informações ao jogador e melhoria da experiência durante a execução do jogo.</td>
    </tr>
    <tr>
      <td align="center">
        <img src="./lucas.jpeg" width="100" height="100" alt="Foto de Lucas Aprígio dos Santos">
      </td>
      <td><strong>Lucas Aprígio dos Santos</strong></td>
      <td><strong>Desenvolvedor Back-end</strong></td>
      <td>Apoio na implementação das funcionalidades internas do sistema, estrutura de suporte da aplicação e organização do funcionamento geral do projeto.</td>
    </tr>
  </tbody>
</table>

---


