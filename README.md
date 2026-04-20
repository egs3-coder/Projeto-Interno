
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

## Tela do Kanban

<img src="board.png" alt="Captura de tela do Numbrix C" width="600">

## Ferramentas 

🔗 [Trello](https://trello.com/b/peA1EPFt/projeto-interno)

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
...

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
...
