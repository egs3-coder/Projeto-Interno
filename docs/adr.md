# ADR - Decisoes de Arquitetura

## ADR 001 - Linguagem C com raylib

Decisao: manter o projeto em C usando raylib para a interface grafica.

Motivo: o projeto foi desenvolvido como atividade de C e a raylib oferece uma camada simples para janelas, desenho e entrada de mouse/teclado.

Consequencia: a distribuicao precisa manter executavel e DLLs compativeis na mesma pasta.

## ADR 002 - Persistencia em arquivos texto

Decisao: salvar historico, colecao e perfis em arquivos `.txt`.

Motivo: facilita inspecao manual, testes e portabilidade sem banco de dados.

Consequencia: os campos precisam evitar `;` e quebras de linha, por isso o codigo sanitiza nomes e modalidades.

## ADR 003 - Backups de executaveis legacy

Decisao: preservar executaveis e DLLs antigas em vez de substitui-los sem rastreabilidade.

Motivo: algumas maquinas dependem da combinacao legacy/proxy para rodar corretamente.

Consequencia: novas versoes devem ser geradas com nomes claros ou acompanhadas de backup.
