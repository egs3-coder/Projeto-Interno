# Protocolo

O jogo se passa em um futuro próximo, onde uma inteligência artificial chamada **Protocolo** monitora e avalia constantemente o comportamento humano. O jogador participa de uma simulação chamada **Protocolo 100**, em que deve descobrir um número oculto entre 1 e 100. Entretanto, o verdadeiro objetivo do sistema não é apenas verificar se o jogador acerta, mas analisar seu raciocínio, sua capacidade de adaptação, sua consistência e a forma como lida com a pressão e com a lógica imposta pela máquina.

Ao longo das partidas, o sistema registra cada tentativa e transforma o histórico do jogador em dados de comportamento. Com base nisso, gera análises, métricas de desempenho e dicas personalizadas, tornando a experiência mais profunda e estratégica. No modo avançado, o jogador enfrenta um bot em tempo real, sob limite de tempo e com a possibilidade de mutações no número-alvo, criando uma atmosfera de instabilidade e tensão.

Com uma estética inspirada em ficção tecnológica e suspense psicológico, o jogo propõe mais do que um simples desafio numérico: ele convida o usuário a enfrentar uma inteligência que aprende com seus erros, corrige seus padrões e transforma cada decisão em evidência. Assim, **Protocolo 100** se apresenta como um jogo moderno, atrativo e inovador, com forte apelo visual e conceitual para diferentes públicos.

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

# Funcionalidades com Nova Temática

A seguir, apresenta-se a lista de funcionalidades reformulada com nomenclaturas mais imersivas e alinhadas à nova temática do jogo.

## UH1 – Inicialização do Protocolo
O sistema ativa a sessão e gera o código oculto que dará início ao teste.

## UH2 – Leitura de Resposta Cognitiva
O jogador insere palpites e recebe feedback imediato do sistema, indicando se sua leitura está acima, abaixo ou próxima do padrão esperado.

## UH3 – Arquivo de Vigilância
Cada partida concluída é registrada automaticamente como um dossiê de comportamento do jogador.

## UH4 – Reconstrução de Perfil
O sistema lê os registros anteriores para montar o histórico analítico das sessões já realizadas.

## UH5 – Painel de Evolução
Exibe o resumo das sessões, média de tentativas, melhor desempenho e pior desempenho do jogador.

## UH6 – Mapa de Tendência Comportamental
Analisa consistência, viés de resposta, impulsividade e padrão de erro com base nas jogadas anteriores.

## UH7 – Interferência Adaptativa da IA
Entrega dicas inteligentes, identifica hábitos repetitivos e reconhece padrões eficientes ou ineficientes do jogador.

## UH8 – Central do Sistema
Menu principal onde o usuário escolhe iniciar nova simulação, acessar análises ou encerrar a execução.

## UH9 – Relatório Pós-Sessão
Ao final da rodada, o sistema apresenta um diagnóstico completo da performance daquela execução.

## UH10 – Modo Ruptura
Modo especial com cronômetro, adversário automático, penalidade lógica, mutação do alvo e disputa direta entre humano e máquina.

---

## Tela do Kanban

<img src="board.png" alt="Captura de tela do Numbrix C" width="600">

## Ferramentas 

🔗 [Trello](https://trello.com/b/peA1EPFt/projeto-interno)
 


 ## 🔄 Diagrama de Atividades do Sistema

## UH1 - Geração de Alvo Aleatório

```mermaid
flowchart TD
    A([Início da partida]) --> B[Inicializar semente com srand]
    B --> C[Gerar número com rand]
    C --> D[Aplicar faixa válida de 1 a 100]
    D --> E[Armazenar número secreto]
    E --> F([Partida pronta para iniciar])
```

---

## UH2 - Validação de Palpites

```mermaid
flowchart TD
    A([Início do turno]) --> B[Receber palpite via stdin]
    B --> C{Entrada é um inteiro válido?}

    C -- Não --> D[Limpar buffer de entrada]
    D --> E[Informar erro de entrada]
    E --> B

    C -- Sim --> F{Palpite está entre 1 e 100?}
    F -- Não --> G[Informar valor fora da faixa]
    G --> B

    F -- Sim --> H[Calcular diferença entre palpite e alvo]
    H --> I{Palpite igual ao alvo?}

    I -- Sim --> J[Exibir: Acertou]
    J --> K([Fim do turno])

    I -- Não --> L{Palpite menor que o alvo?}
    L -- Sim --> M[Exibir: Muito baixo]
    L -- Não --> N[Exibir: Muito alto]

    M --> O{Diferença > 30?}
    N --> P{Diferença > 30?}

    O -- Sim --> Q[Feedback visual Frio - Azul]
    O -- Não --> R{Diferença < 10?}
    P -- Sim --> Q
    P -- Não --> S{Diferença < 10?}

    R -- Sim --> T[Feedback visual Quente - Vermelho]
    R -- Não --> U[Feedback neutro]
    S -- Sim --> T
    S -- Não --> U

    Q --> K
    T --> K
    U --> K
```

---

## UH3 - Registro de Partidas

```mermaid
flowchart TD
    A([Fim da partida]) --> B[Coletar dados da sessão]
    B --> C[Montar linha: timestamp;alvo;tentativas;abaixo;acima;lista_csv]
    C --> D[Abrir arquivo com fopen]
    D --> E{Arquivo abriu corretamente?}

    E -- Não --> F[Exibir erro de gravação]
    F --> G([Jogo continua normalmente])

    E -- Sim --> H[Gravar linha com fprintf]
    H --> I[Fechar arquivo]
    I --> J([Sessão salva])
```

---

## UH4 - Análise de Histórico

```mermaid
flowchart TD
    A([Abrir menu de análise]) --> B[Verificar existência do arquivo de histórico]
    B --> C{Arquivo existe?}

    C -- Não --> D[Informar ausência de dados]
    D --> E([Fim da análise])

    C -- Sim --> F[Abrir arquivo]
    F --> G[Ler linha por linha com fgets]
    G --> H{Ainda há linha para ler?}

    H -- Sim --> I[Parsear dados da linha]
    I --> J[Popular estrutura Session]
    J --> K[Armazenar sessão carregada]
    K --> G

    H -- Não --> L[Fechar arquivo]
    L --> M[Disponibilizar dados para métricas e relatórios]
    M --> E
```

---

## UH5 - Métricas Básicas

```mermaid
flowchart TD
    A([Iniciar cálculo de métricas]) --> B{Há sessões carregadas?}

    B -- Não --> C[Informar que não há sessões para análise]
    C --> D([Fim])

    B -- Sim --> E[Usar recursão para somar tentativas]
    E --> F[Usar recursão para obter mínimo]
    F --> G[Usar recursão para obter máximo]
    G --> H[Calcular média = soma / total de sessões]
    H --> I[Identificar melhor sessão]
    I --> J[Identificar pior sessão]
    J --> K[Exibir total de sessões]
    K --> L[Exibir média de tentativas]
    L --> M[Exibir melhor e pior sessão]
    M --> D
```

---

## UH6 - Análise de Performance

```mermaid
flowchart TD
    A([Iniciar análise de performance]) --> B{Há histórico disponível?}

    B -- Não --> C[Informar ausência de dados]
    C --> D([Fim])

    B -- Sim --> E[Calcular média de tentativas]
    E --> F[Calcular viés baixo = low_count / attempts_count]
    F --> G[Calcular viés alto = high_count / attempts_count]
    G --> H[Usar recursão para somar quadrados das diferenças]
    H --> I[Aplicar sqrt para obter desvio padrão]
    I --> J[Interpretar consistência do jogador]
    J --> K[Exibir viés alto e viés baixo]
    K --> L[Exibir desvio padrão]
    L --> D
```

---

## UH7 - Dicas Dinâmicas

```mermaid
flowchart TD
    A([Iniciar geração de dicas]) --> B[Carregar histórico e vetor de palpites]
    B --> C[Analisar primeiros palpites do jogador]
    C --> D{Tendência inicial muito baixa?}

    D -- Sim --> E[Gerar dica: evitar começar tão baixo]
    D -- Não --> F{Tendência inicial muito alta?}

    F -- Sim --> G[Gerar dica: evitar começar tão alto]
    F -- Não --> H[Seguir análise]

    E --> H
    G --> H

    H --> I[Usar recursão para contar passos]
    I --> J[Usar recursão para detectar sequência monotônica]
    J --> K{Estratégia linear ineficiente detectada?}

    K -- Sim --> L[Alertar sobre busca linear / pouca eficiência]
    K -- Não --> M[Seguir análise]

    L --> M
    M --> N{Desvio padrão baixo e média alta?}

    N -- Sim --> O[Alertar estratégia repetitiva e pouco eficiente]
    N -- Não --> P[Seguir análise]

    O --> P
    P --> Q{Comportamento semelhante à busca binária e poucos chutes?}

    Q -- Sim --> R[Desbloquear badge: Algoritmo Humano]
    Q -- Não --> S[Sem badge]

    R --> T[Exibir dicas personalizadas]
    S --> T
    T --> U([Fim])
```

---

## UH8 - Navegação do Jogo

```mermaid
flowchart TD
    A([Início do programa]) --> B[Exibir menu CLI]
    B --> C{Escolha do usuário}

    C -- Jogar nova partida --> D[Chamar módulo do jogo]
    D --> B

    C -- Analisar histórico --> E[Chamar módulo de histórico/análise]
    E --> B

    C -- Sair --> F([Encerrar programa])

    C -- Opção inválida --> G[Exibir mensagem de opção inválida]
    G --> B
```

---

## UH9 - Resumo da Rodada

```mermaid
flowchart TD
    A([Jogador acertou o alvo]) --> B[Coletar Data/Hora da sessão]
    B --> C[Recuperar alvo da rodada]
    C --> D[Recuperar total de tentativas]
    D --> E[Recuperar quantidade de palpites altos]
    E --> F[Recuperar quantidade de palpites baixos]
    F --> G[Exibir resumo completo da rodada]
    G --> H{Log foi salvo com sucesso?}

    H -- Sim --> I[Exibir mensagem: Sessão salva com sucesso]
    H -- Não --> J[Exibir mensagem de erro no salvamento]

    I --> K([Fim])
    J --> K
```

---

## UH10 - Expansão de Jogo

```mermaid
flowchart TD
    A([Início da partida avançada]) --> B{Modo especial foi ativado?}

    B -- Não --> C[Seguir para modo normal]
    C --> Z([Fim])

    B -- Sim --> D[Selecionar comportamento do bot]
    D --> E{Bot linear ou binário?}
    E -- Linear --> F[Inicializar bot em modo linear]
    E -- Binário --> G[Inicializar bot em modo binário]

    F --> H[Iniciar temporizador global]
    G --> H

    H --> I[Inicializar pontuação, penalidades e contador de erros]
    I --> J([Novo turno])

    J --> K[Receber palpite do jogador]
    K --> L[Gerar palpite do bot no mesmo turno]
    L --> M{Jogador quebrou a lógica das dicas anteriores?}

    M -- Sim --> N[Aplicar penalidade de pontuação]
    M -- Não --> O[Manter pontuação]

    N --> P[Comparar palpite do jogador com o alvo]
    O --> P

    P --> Q{Jogador acertou?}
    Q -- Sim --> R[Definir jogador como vencedor]
    Q -- Não --> S[Comparar palpite do bot com o alvo]

    S --> T{Bot acertou?}
    T -- Sim --> U[Definir bot como vencedor]
    T -- Não --> V[Incrementar tentativas incorretas do jogador]

    V --> W{A cada 3 erros do jogador?}
    W -- Sim --> X[Mutar alvo em +2 ou -2]
    W -- Não --> Y[Manter alvo atual]

    X --> AA[Garantir alvo entre 1 e 100]
    Y --> AB[Atualizar tempo e pontuação]
    AA --> AB
    AB --> J

    R --> AC[Parar temporizador]
    U --> AC

    AC --> AD[Calcular pontuação final com base no tempo]
    AD --> AE[Exibir penalidades aplicadas]
    AE --> AF[Exibir modo do bot utilizado]
    AF --> AG[Exibir resultado do duelo]
    AG --> AH([Fim da sessão avançada])
```
