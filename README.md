<p align="center">
  <img src="./metadecklogo.png" alt="Logo do MetaDeck" width="500">
</p>

O **MetaDeck** é um jogo de cartas estratégico em que o jogador avança por fases cada vez mais desafiadoras, usando combinações, escolhas táticas e adaptação para superar metas de pontuação. Durante a partida, cada decisão influencia diretamente o desempenho, tornando cada rodada única e dinâmica.

A proposta do jogo é oferecer uma experiência envolvente e lúdica, em que estratégia, atenção e criatividade caminham juntas. Com uma atmosfera vibrante e desafios progressivos, o **MetaDeck** estimula o jogador a pensar antes de agir, explorar diferentes possibilidades e buscar a melhor forma de seguir avançando.

---

## Backlog

<img src="Backlog.png" alt="Captura de tela do MetaDeck" width="600">


<img src="UH.png" alt="Captura de tela do MetaDeck" width="600">

--- 
## Ferramentas 

🔗 [Trello](https://trello.com/b/peA1EPFt/projeto-interno)

---

## 👨‍💻 Equipe

A equipe do **Protocolo** foi organizada de forma colaborativa, distribuindo responsabilidades entre planejamento, prototipação, desenvolvimento, testes e apoio à documentação do projeto.

| Integrante | Função | Descrição |
|---|---|---|
| **Ewerton Guilherme da Silva** | **Product Owner / Desenvolvedor Back-end** | Responsável pela organização das ideias principais do projeto, definição das histórias de usuário e apoio na implementação das regras centrais do jogo, como fases, lógica de progressão e estrutura geral do sistema. |
| **Lauan Gonçalves dos Santos** | **Scrum Master** | Responsável pela organização visual do projeto, protótipos e representação das interfaces e fluxos do jogo, ajudando a planejar a experiência do usuário e a apresentação visual das telas e diagramas. |
| **Davi Magno Campelo do Nascimento** | **Desenvolvedor Front-end** | Responsável pela construção das interações visíveis ao jogador no terminal, incluindo menus, mensagens da partida, exibição de pontuação, feedbacks e organização da navegação do sistema. |
| **Aquiles Pereira dos Santos** | **Testes / QA** | Responsável pela validação das funcionalidades do jogo, identificação de erros e verificação do comportamento esperado das fases, pontuação, ranking e demais mecânicas implementadas. |
| **João Ricardo Alves de Brito** | **Desenvolvedor Back-end** | Responsável pelo apoio na lógica interna do sistema, manipulação de dados do jogo, controle de ranking, armazenamento de informações e funcionamento das regras principais da aplicação. |
| **Mateus Valerino Barros de Santana** | **Desenvolvedor Front-end** | Responsável pelo apoio na construção das telas em terminal, organização da exibição das informações ao jogador e melhoria da experiência durante a execução das fases e eventos do jogo. |
| **Lucas Aprígio dos Santos** | **Desenvolvedor Back-end** | Responsável pelo apoio na implementação das funcionalidades internas do sistema, contribuindo com a lógica das partidas, manipulação de arquivos e estrutura de suporte ao funcionamento do jogo.

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
