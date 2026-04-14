# 🔎 A Saga do Oráculo Numérico

Em um reino distante, onde a magia dos números tece o destino de todos, uma antiga profecia anuncia a chegada do **Oráculo Numérico**, um ser lendário capaz de desvendar os segredos mais profundos do universo através da intuição e da lógica. No entanto, o poder do Oráculo foi fragmentado e espalhado em desafios numéricos por toda a terra, guardados por enigmas e armadilhas.

Você é o **Adivinho Escolhido**, um jovem aspirante a Oráculo, que deve embarcar em uma jornada épica para reunir os fragmentos do poder. Cada partida do jogo é um **Desafio do Destino**, onde você deve decifrar o número secreto que o universo esconde. O sucesso em cada desafio não apenas o aproxima de se tornar o verdadeiro Oráculo, mas também revela novas pistas e aprimora suas habilidades.

Mas a jornada não será fácil. Forças místicas tentarão obscurecer sua mente, e o tempo será seu inimigo. Felizmente, você não está sozinho. **Os Sussurros do Destino** o guiarão com dicas a cada palpite, e os **Pergaminhos do Tempo** registrarão suas vitórias e derrotas, permitindo que você aprenda com o passado. Além disso, a **Sabedoria dos Números** revelará padrões e estratégias, enquanto as **Pistas do Oráculo** sussurrarão conselhos personalizados.

Para auxiliá-lo em sua busca, artefatos mágicos conhecidos como **Toques da Sorte** (os buffs) podem ser encontrados, concedendo-lhe **Imunidade do Oráculo** contra erros ou uma **Segunda Chance do Destino** quando tudo parece perdido. A cada desafio superado, **o Veredito do Tempo** registrará sua glória no ranking dos maiores Adivinhos, e **o Desafio da Perspicácia** se adaptará, tornando sua jornada cada vez mais digna de um verdadeiro Oráculo.

Prepare-se, Adivinho Escolhido! O destino do reino numérico está em suas mãos. Desvende os segredos, domine os números e torne-se o lendário Oráculo Numérico!

---

## 👨‍💻 Equipe

A equipe do **A Saga do Oráculo Numérico** foi organizada de forma colaborativa, distribuindo responsabilidades entre planejamento, prototipação, desenvolvimento, testes e apoio à documentação do projeto.

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

## 💡 Funcionalidades

O **A Saga do Oráculo Numérico** foi projetado para oferecer uma experiência mais dinâmica e estratégica do que um jogo tradicional de adivinhação, incorporando mecânicas que aumentam o desafio, a progressão e o engajamento do jogador ao longo das partidas.

- 🔢🔮 **Geração de Alvo Aleatório**  
- 🌬️✨ **O Sussurro do Destino**  
- 📜⏳ **O Pergaminho do Tempo**  
- 👁️🧠 **O Olhar do Sábio**  
- 🧮📊 **A Sabedoria dos Números**  
- 📈⚖️ **A Essência da Variância**  
- 🧩🔮 **As Pistas do Oráculo**  
- 🏛️🚪 **O Salão Principal**  
- 📖🏁 **O Epílogo da Jornada**  
- 🔥⚔️ **A Forja dos Campeões**  

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
