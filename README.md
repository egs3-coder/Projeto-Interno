# 🔎 Numbrix C

O **Numbrix C** é um jogo de adivinhação numérica desenvolvido em **linguagem C**, com execução em **terminal**, criado para unir **entretenimento, lógica e estratégia** em uma experiência mais dinâmica do que os jogos tradicionais do gênero.

No jogo, o usuário deve descobrir números secretos gerados pelo sistema, utilizando dicas e raciocínio lógico para avançar. No entanto, o projeto não se limita ao modelo clássico de uma única rodada: o **Numbrix C** introduz um sistema de **fases progressivas**, em que o jogador pode precisar acertar **mais de um número por etapa**, aumentando gradualmente o nível de desafio e tornando a partida mais envolvente.

Além disso, o jogo conta com **níveis de dificuldade**, **pontuação acumulativa**, **ranking local**, **buffs e hacks estratégicos** e elementos de **análise de desempenho**, o que faz com que cada partida seja mais interativa e recompensadora. Dessa forma, o jogador não apenas tenta acertar números, mas também busca evoluir, pontuar mais, superar fases e melhorar suas decisões ao longo da partida.

O projeto foi pensado para mostrar que, mesmo em um ambiente simples como o terminal, é possível construir uma aplicação criativa, organizada e divertida, aplicando conceitos importantes de programação, como estruturas condicionais, laços de repetição, funções, manipulação de arquivos e controle de fluxo.

---

## Ferramentas 

🔗 [Trello](https://trello.com/b/peA1EPFt/projeto-interno)

## Tela do Kanban

 [Kanban](Captura de tela 2026-04-09 112910.png)

Captura de tela 2026-04-09 112910.png
## 👨‍💻 Equipe

A equipe do **Numbrix C** foi organizada de forma colaborativa, distribuindo responsabilidades entre planejamento, prototipação, desenvolvimento, testes e apoio à documentação do projeto.

| Integrante | Função | Descrição |
|---|---|---|
| **Ewerton Guilherme da Silva** | **Product Owner / Desenvolvedor Back-end** | Responsável pela organização das ideias principais do projeto, definição das histórias de usuário e apoio na implementação das regras centrais do jogo, como fases, lógica de progressão e estrutura geral do sistema. |
| **Lauan Gonçalves dos Santos** | **Design / Figma** | Responsável pela organização visual do projeto, protótipos e representação das interfaces e fluxos do jogo, ajudando a planejar a experiência do usuário e a apresentação visual das telas e diagramas. |
| **Davi Magno Campelo do Nascimento** | **Desenvolvedor Front-end** | Responsável pela construção das interações visíveis ao jogador no terminal, incluindo menus, mensagens da partida, exibição de pontuação, feedbacks e organização da navegação do sistema. |
| **Aquiles Pereira dos Santos** | **Testes / QA** | Responsável pela validação das funcionalidades do jogo, identificação de erros e verificação do comportamento esperado das fases, pontuação, ranking e demais mecânicas implementadas. |
| **João Ricardo Alves de Brito** | **Desenvolvedor Back-end** | Responsável pelo apoio na lógica interna do sistema, manipulação de dados do jogo, controle de ranking, armazenamento de informações e funcionamento das regras principais da aplicação. |
| **Mateus Valerino Barros de Santana** | **Desenvolvedor Front-end** | Responsável pelo apoio na construção das telas em terminal, organização da exibição das informações ao jogador e melhoria da experiência durante a execução das fases e eventos do jogo. |
| **Lucas Aprígio dos Santos** | **Desenvolvedor Back-end** | Responsável pelo apoio na implementação das funcionalidades internas do sistema, contribuindo com a lógica das partidas, manipulação de arquivos e estrutura de suporte ao funcionamento do jogo.

---

## 🎯 Objetivo

O principal objetivo do **Numbrix C** é transformar um jogo simples de adivinhação em uma experiência mais **estratégica, desafiadora e educativa**, incentivando o desenvolvimento do raciocínio lógico e a aplicação prática de conceitos fundamentais da programação em C.

Além de proporcionar diversão ao usuário, o projeto busca demonstrar como uma ideia básica pode ser expandida com mecânicas de progressão, recompensa e tomada de decisão, tornando o jogo mais atrativo e diferenciado.

---

## 🚀 Diferenciais

O **Numbrix C** se destaca por apresentar elementos que vão além do modelo tradicional de “acertar um número e encerrar o jogo”. Entre seus principais diferenciais, estão:

- 🎯 **fases progressivas com dificuldade crescente**
- 🔢 **múltiplos números secretos em uma mesma fase**
- 🏆 **sistema de pontuação acumulativa**
- 📊 **ranking local de melhores jogadores**
- ⚡ **buffs e hacks estratégicos**
- 📈 **análise de desempenho e evolução do jogador**

Com isso, o projeto oferece uma experiência mais completa, interativa e interessante, mantendo a essência do jogo de adivinhação, mas adicionando camadas de estratégia e progressão que aumentam o engajamento do usuário.

## 💡 Funcionalidades

O **Numbrix C** foi projetado para oferecer uma experiência mais dinâmica e estratégica do que um jogo tradicional de adivinhação, incorporando mecânicas que aumentam o desafio, a progressão e o engajamento do jogador ao longo das partidas.

- 🎯 **Geração do Alvo (RNG)**  
- 🔢 **Loop de Adivinhação e Validação**  
- ⏳ **Gravação do Histórico de Partidas**  
- 🏆 **Leitura e Reconstrução do Histórico**  
- 🔥 **Motor Analítico - Agregação Básica**  
- 🎁 **Motor Analítico - Variância e Viés**  
- 🎲 **Geração de Heurísticas Textuais**  
- ⚡ **Menu Principal e Navegação Modular**  
- ⚖️ **Resumo Pós-Sessão**  
- 📊 **Épico: "Além do Console" (Expansão e Multidisciplinaridade)**  


 ## 🔄 Diagrama de Atividades do Sistema
```mermaid
flowchart TD

    Inicio((Inicio)) --> Menu[Menu Principal]

    Menu -->|Jogar nova partida| GerarAlvo
    Menu -->|Analisar historico| LerArquivo
    Menu -->|Sair| Fim((Fim))

    %% =========================
    %% ATIVIDADE: JOGAR
    %% =========================

    subgraph Jogar [Atividade Jogar Nova Partida]

        GerarAlvo[Gerar numero aleatorio 1 a 100]
        ReceberPalpite[Receber palpite do usuario]
        ValidarEntrada[Validar tipo e faixa]
        DecEntrada{Entrada valida?}
        DecAcerto{Palpite igual ao alvo?}
        Dica[Exibir dica Muito alto ou Muito baixo]
        Resumo[Exibir resumo da rodada]
        Salvar[Salvar dados no historico]

        GerarAlvo --> ReceberPalpite
        ReceberPalpite --> ValidarEntrada
        ValidarEntrada --> DecEntrada

        DecEntrada -- Nao --> ReceberPalpite
        DecEntrada -- Sim --> DecAcerto

        DecAcerto -- Nao --> Dica
        Dica --> ReceberPalpite

        DecAcerto -- Sim --> Resumo
        Resumo --> Salvar

    end

    Salvar --> Menu

    %% =========================
    %% ATIVIDADE: ANALISAR
    %% =========================

    subgraph Analisar [Atividade Analisar Historico]

        LerArquivo[Ler arquivo de sessoes]
        Agregacao[Calcular totais e media]
        Estatistica[Calcular desvio padrao e vies]
        Heuristica[Gerar recomendacoes estrategicas]
        Exibir[Exibir painel ao jogador]

        LerArquivo --> Agregacao
        Agregacao --> Estatistica
        Estatistica --> Heuristica
        Heuristica --> Exibir

    end

    Exibir --> Menu
```
