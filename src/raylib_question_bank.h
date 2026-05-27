#ifndef RAYLIB_QUESTION_BANK_H
#define RAYLIB_QUESTION_BANK_H

#include <stdlib.h>

#include "player_data.h"

typedef enum {
    RAY_QUIZ_CATEGORY_LOGIC_CLASSIC = 0,
    RAY_QUIZ_CATEGORY_MATH_LOGIC,
    RAY_QUIZ_CATEGORY_C_PROGRAMMING,
    RAY_QUIZ_CATEGORY_PORTUGUESE,
    RAY_QUIZ_CATEGORY_COUNT
} RayQuizCategory;

typedef enum {
    RAY_QUIZ_DIFFICULTY_FACIL = 0,
    RAY_QUIZ_DIFFICULTY_AVANCADA,
    RAY_QUIZ_DIFFICULTY_INTERMEDIARIA,
    RAY_QUIZ_DIFFICULTY_DIFICIL,
    RAY_QUIZ_DIFFICULTY_MUITO_DIFICIL,
    RAY_QUIZ_DIFFICULTY_COUNT
} RayQuizDifficulty;

typedef struct {
    const char *question;
    const char *options[4];
    int answer;
    const char *hint;
    RayQuizCategory category;
    RayQuizDifficulty difficulty;
} RayQuizQuestion;

typedef struct {
    const char *question;
    const char *options[4];
    int answer;
    const char *hint;
} RayQuizQuestionEntry;

#define RAY_QUIZ_QUESTION_COUNT 20
#define QE(q, a, b, c, d, ans, hint) {q, {a, b, c, d}, ans, hint}

static const RayQuizQuestionEntry LOGIC_BANK[RAY_QUIZ_DIFFICULTY_COUNT][RAY_QUIZ_QUESTION_COUNT] = {
    {
        QE("Se p=V e q=F, quanto vale p && q?", "Verdadeiro", "Falso", "Tautologia", "Contradicao", 2, "Na conjuncao, as duas partes precisam ser verdadeiras."),
        QE("Se p=V e q=F, quanto vale p || q?", "Verdadeiro", "Falso", "Impossivel saber", "Contradicao", 1, "Na disjuncao, basta uma parte ser verdadeira."),
        QE("Se p=F, quanto vale !p?", "Verdadeiro", "Falso", "Contingente", "Nao e proposicao", 1, "A negacao inverte o valor logico."),
        QE("Se p=V e q=F, quanto vale p -> q?", "Verdadeiro", "Falso", "Tautologia", "Nao e proposicao", 2, "A implicacao so falha em V -> F."),
        QE("Se p=V e q=V, quanto vale p <-> q?", "Falso", "Contradicao", "Verdadeiro", "Indefinido", 3, "A bicondicional vale V quando os valores coincidem."),
        QE("Se p=V e q=F, quanto vale p <-> q?", "Verdadeiro", "Falso", "Tautologia", "Contingente", 2, "A bicondicional e falsa quando os valores diferem."),
        QE("Se p=F e q=F, quanto vale !(p || q)?", "Verdadeiro", "Falso", "Impossivel saber", "Nao e proposicao", 1, "Primeiro avalie a disjuncao."),
        QE("Se p=V e q=F, quanto vale !(p && q)?", "Verdadeiro", "Falso", "Tautologia", "Contradicao", 1, "p && q ja e falso."),
        QE("Se p=V e q=F, quanto vale q -> p?", "Verdadeiro", "Falso", "Contradicao", "Contingente", 1, "A implicacao F -> V e verdadeira."),
        QE("A frase '2 e menor que 5' e:", "Nao e proposicao", "Proposicao verdadeira", "Proposicao falsa", "Paradoxo", 2, "Ela admite valor logico."),
        QE("A frase 'Feche a porta' e:", "Proposicao verdadeira", "Proposicao falsa", "Nao e proposicao", "Tautologia", 3, "Ordens nao recebem valor logico."),
        QE("A frase 'Se 4 e par, entao 9 e impar' e:", "Verdadeira", "Falsa", "Nao e proposicao", "Contraditoria", 1, "As duas partes sao verdadeiras."),
        QE("Uma conjuncao de duas proposicoes verdadeiras e:", "Sempre falsa", "Sempre verdadeira", "Nunca proposicao", "Contingente", 2, "V && V resulta em V."),
        QE("Uma disjuncao de duas proposicoes falsas e:", "Verdadeira", "Falsa", "Tautologica", "Impossivel saber", 2, "F || F resulta em F."),
        QE("Se p=V, quanto vale p && !p?", "Verdadeiro", "Falso", "Contingente", "Nao e proposicao", 2, "Uma proposicao nao pode ser verdadeira junto com sua negacao."),
        QE("Se p=F, quanto vale p || !p?", "Falso", "Contradicao", "Verdadeiro", "Impossivel saber", 3, "Lei do terceiro excluido."),
        QE("A frase 'Todo numero par e divisivel por 2' e:", "Proposicao falsa", "Proposicao verdadeira", "Nao e proposicao", "Paradoxo", 2, "E uma afirmacao matematica valida."),
        QE("A frase '3 e primo ou 4 e impar' e:", "Falsa", "Nao e proposicao", "Verdadeira", "Contradicao", 3, "Basta um dos lados ser verdadeiro."),
        QE("A frase 'Se 7 > 10, entao 2 < 1' e:", "Verdadeira", "Falsa", "Contingente", "Sem sentido", 1, "Implicacao com antecedente falso e verdadeira."),
        QE("Qual simbolo representa negacao logica?", "&&", "||", "!", "->", 3, "E o operador unario de negacao.")
    },
    {
        QE("Qual e a negacao de p && q?", "!p || !q", "!p && !q", "p || q", "p -> q", 1, "Aplique De Morgan."),
        QE("Qual e a negacao de p || q?", "!p && !q", "!p || !q", "p && q", "p <-> q", 1, "De Morgan novamente."),
        QE("Qual e a contrapositiva de p -> q?", "!p -> !q", "!q -> !p", "q -> p", "p -> !q", 2, "Troque e negue."),
        QE("Qual expressao equivale a p <-> q?", "(p -> q) && (q -> p)", "p || q", "p && !q", "!p || q", 1, "A bicondicional exige ida e volta."),
        QE("Se p=F, q=V, quanto vale (p || q) && !q?", "Verdadeiro", "Falso", "Tautologia", "Contradicao", 2, "A primeira parte vale V, mas !q vale F."),
        QE("A formula p || !p e:", "Contradicao", "Contingencia", "Tautologia", "Nao e formula", 3, "Sempre ha um dos lados verdadeiro."),
        QE("A formula p && !p e:", "Tautologia", "Contradicao", "Contingencia", "Equivalencia", 2, "Nunca pode ser verdadeira."),
        QE("Se p=V, q=F, r=V, quanto vale (p && r) || q?", "Verdadeiro", "Falso", "Contingente", "Contradicao", 1, "p && r vale V."),
        QE("Uma implicacao e falsa somente quando:", "p=F e q=V", "p=V e q=F", "p=F e q=F", "p=V e q=V", 2, "E o unico caso de falha."),
        QE("Qual equivalencia esta correta?", "p -> q equivale a !p || q", "p -> q equivale a p && q", "p -> q equivale a p || q", "p -> q equivale a !q", 1, "Reescreva a implicacao como disjuncao."),
        QE("Se p=F e q=V, quanto vale !(p -> q)?", "Verdadeiro", "Falso", "Tautologia", "Nao e formula", 2, "p -> q vale V nesse caso."),
        QE("A expressao !(p && q) && (p || q) representa:", "OU exclusivo", "Implicacao", "Bicondicional", "Contradicao", 1, "V quando exatamente uma e verdadeira."),
        QE("Se p=V e q=F, quanto vale (!p || q)?", "Verdadeiro", "Falso", "Contingente", "Indefinido", 2, "Os dois lados ficam falsos."),
        QE("Qual e a negacao de 'Todo A e B'?", "Todo A nao e B", "Existe A que nao e B", "Nenhum A e B", "Existe B que nao e A", 2, "Negar um quantificador universal cria um existencial."),
        QE("Qual e a negacao de 'Existe A que e B'?", "Todo A e B", "Nenhum A e B", "Existe A que nao e B", "Todo B e A", 2, "Negar um existencial cria um universal negativo."),
        QE("Qual lei aparece em !(p || q) <-> !p && !q?", "Contraposicao", "Associatividade", "De Morgan", "Idempotencia", 3, "Ela distribui a negacao."),
        QE("Se p=V, q=V, quanto vale (p -> q) && (q -> p)?", "Verdadeiro", "Falso", "Contradicao", "Impossivel saber", 1, "As duas implicacoes sao verdadeiras."),
        QE("A formula (p && q) -> p e:", "Contradicao", "Contingencia", "Tautologia", "Sem sentido", 3, "Se a conjuncao vale, p obrigatoriamente vale."),
        QE("A formula p -> (p || q) e:", "Tautologia", "Contradicao", "Falsa quando q=F", "Equivale a q", 1, "Se p e verdadeiro, a disjuncao tambem e."),
        QE("Se p=F, q=F, quanto vale (!p && !q) -> p?", "Verdadeiro", "Falso", "Tautologia", "Nao e proposicao", 2, "O antecedente vale V e o consequente vale F.")
    },
    {
        QE("Todo professor estuda logica. Ana e professora. Logo, Ana estuda logica. O argumento e:", "Valido", "Invalido", "Paradoxo", "Circular", 1, "E um caso direto de silogismo."),
        QE("Se estudo, entao passo. Eu estudo. Logo, passo. O nome do esquema e:", "Modus tollens", "Afirmacao do consequente", "Modus ponens", "Dilema", 3, "Parte da implicacao e confirma o antecedente."),
        QE("Se estudo, entao passo. Nao passei. Logo, nao estudei. O nome do esquema e:", "Modus tollens", "Modus ponens", "Falacia da negacao do antecedente", "Equivoco", 1, "Nega o consequente e conclui pela negacao do antecedente."),
        QE("Se programo, entao treino logica. Eu treino logica. Logo, programo. O argumento e:", "Valido", "Invalido", "Tautologico", "Bicondicional", 2, "Isso e afirmacao do consequente."),
        QE("A negacao de 'Nenhum aluno faltou' e:", "Todo aluno faltou", "Existe aluno que faltou", "Nenhum aluno nao faltou", "Alguns alunos estudaram", 2, "Negar um universal negativo produz um existencial positivo."),
        QE("A negacao de 'Algum codigo compilou' e:", "Todo codigo falhou", "Nenhum codigo compilou", "Algum codigo nao compilou", "Todo codigo compilou", 2, "Negar um existencial produz um universal negativo."),
        QE("Todo felino e mamifero. Todo tigre e felino. Logo, todo tigre e mamifero. O argumento e:", "Invalido", "Valido", "Circular", "Ambiguo", 2, "As premissas encadeiam corretamente."),
        QE("Todo numero par e divisivel por 2. 9 nao e divisivel por 2. Logo, 9 nao e par. O argumento e:", "Valido", "Invalido", "Paradoxo", "Sem forma", 1, "E uma aplicacao da contrapositiva."),
        QE("Se a porta abre, a luz acende. A luz nao acendeu. Logo, a porta nao abriu. O argumento e:", "Modus ponens", "Afirmacao do consequente", "Modus tollens", "Falacia formal", 3, "Negou o consequente."),
        QE("Todo livro da estante e azul. Este objeto nao e azul. Logo, ele nao e livro da estante. O argumento e:", "Valido", "Invalido", "Circular", "Paradoxo", 1, "Usa contrapositiva corretamente."),
        QE("A frase 'Existe aluno que nao estuda' contradiz:", "Todo aluno estuda", "Algum aluno estuda", "Nenhum aluno faltou", "Existe aluno atento", 1, "Uma existencia negativa bate de frente com um universal positivo."),
        QE("A frase 'Todo A e B' e logicamente equivalente a:", "Existe A que nao e B", "Nenhum A e B", "Nao existe A que nao e B", "Existe B que nao e A", 3, "Reescreva o universal como ausencia de contraexemplo."),
        QE("A frase 'Nenhum A e B' e equivalente a:", "Todo A e B", "Nao existe A que seja B", "Existe A que nao e B", "Todo B nao e A", 2, "Universal negativo vira inexistencia de intersecao."),
        QE("Se chove, levo guarda-chuva. Levei guarda-chuva. Logo, choveu. Isso e:", "Modus ponens", "Modus tollens", "Afirmacao do consequente", "Silogismo valido", 3, "O consequente sozinho nao garante o antecedente."),
        QE("Se treino, melhoro. Nao treinei. Logo, nao melhorei. Isso e:", "Falacia da negacao do antecedente", "Modus ponens", "Modus tollens", "Equivalencia", 1, "Negar o antecedente nao permite essa conclusao."),
        QE("Todo algoritmo termina. Este processo nao termina. Logo, nao e algoritmo. O argumento e:", "Valido", "Invalido", "Paradoxal", "Circular", 1, "Contraexemplo ao consequente invalida o antecedente."),
        QE("Se p -> q e q -> r, entao p -> r expressa:", "Transitividadeda implicacao", "Associatividade", "Idempotencia", "Distributividade", 1, "Encadeia implicacoes."),
        QE("A negacao de 'Todo programador gosta de cafe' e:", "Nenhum programador gosta de cafe", "Existe programador que nao gosta de cafe", "Todo cafe gosta de programador", "Existe cafe sem programador", 2, "Basta um contraexemplo."),
        QE("A conclusao 'Algum aluno estuda' segue de 'Todo aluno estuda' apenas se:", "Existir ao menos um aluno", "Nao existir aluno", "O predicado for falso", "A frase for imperativa", 1, "Precisa haver elemento no conjunto."),
        QE("Quando duas proposicoes sempre possuem o mesmo valor logico, elas sao:", "Incompativeis", "Equivalentes", "Contraditorias", "Independentes", 2, "Mesmo comportamento em toda valoracao.")
    },
    {
        QE("Quantas linhas da tabela-verdade de p && q resultam em verdadeiro?", "1", "2", "3", "4", 1, "Somente V,V satisfaz a conjuncao."),
        QE("Quantas linhas da tabela-verdade de p || q resultam em falso?", "0", "1", "2", "3", 2, "Apenas F,F."),
        QE("Qual expressao e equivalente a !(p -> q)?", "p && !q", "!p && q", "!p || q", "p || !q", 1, "Negar a implicacao isola o caso de falha."),
        QE("Se p=V, q=F, r=V, quanto vale (p -> q) || (q -> r)?", "Verdadeiro", "Falso", "Contradicao", "Indefinido", 1, "A primeira implicacao falha, a segunda vale."),
        QE("Qual formula e uma tautologia?", "(p && q) -> p", "p && !p", "!(p || !p)", "p -> !p", 1, "A conclusao ja esta contida no antecedente."),
        QE("Qual formula e uma contradicao?", "p || !p", "(p && q) -> p", "p && !p", "!p -> q", 3, "Nunca pode ser verdadeira."),
        QE("Qual e a negacao de '(Todo A e B) e (Todo B e C)'?", "Existe A que nao e B ou existe B que nao e C", "Todo A nao e B e todo B nao e C", "Existe A que nao e C", "Nenhuma das alternativas", 1, "Negue a conjuncao e depois cada universal."),
        QE("Se p -> q e verdadeiro e q -> r e falso, o que obrigatoriamente vale?", "q=F e r=V", "q=V e r=F", "p=V e q=V", "p=F e r=F", 2, "A implicacao q -> r so e falsa em V -> F."),
        QE("A equivalencia (p || q) && !q <-> p && !q e um caso de:", "Absorcao", "Distributividade", "Idempotencia", "Negacao dupla", 2, "Distribua o && sobre o ||."),
        QE("Em logica, uma condicao necessaria para q em p -> q e:", "p", "q", "!p", "p && q", 2, "Se q falha, a implicacao pode cair."),
        QE("Em logica, uma condicao suficiente para q em p -> q e:", "q", "!q", "p", "p <-> q", 3, "Bastando p, obtem-se q."),
        QE("Qual e a negacao correta de 'Existe x tal que P(x) e Q(x)'?", "Para todo x, !P(x) && !Q(x)", "Para todo x, !P(x) || !Q(x)", "Existe x, !P(x) || !Q(x)", "Nenhum x satisfaz P ou Q", 2, "Negue o existencial e a conjuncao."),
        QE("Qual e a negacao correta de 'Para todo x, P(x) -> Q(x)'?", "Existe x tal que P(x) && !Q(x)", "Existe x tal que !P(x) && Q(x)", "Para todo x, P(x) && !Q(x)", "Existe x tal que !P(x) || Q(x)", 1, "Negue a implicacao ponto a ponto."),
        QE("Se uma formula e falsa em pelo menos uma linha e verdadeira em outra, ela e:", "Tautologia", "Contradicao", "Contingencia", "Equivalencia", 3, "Nem sempre verdadeira, nem sempre falsa."),
        QE("Qual argumento e valido?", "Se p->q, q, logo p", "Se p->q, !p, logo !q", "Se p->q, !q, logo !p", "Se p->q, p->r, logo q->r", 3, "E modus tollens."),
        QE("A expressao (p -> q) && (p -> !q) implica:", "p", "!p", "q", "p || q", 2, "Se p fosse verdadeiro, q e !q surgiriam juntos."),
        QE("Qual formula expressa OU exclusivo entre p e q?", "(p || q) && !(p && q)", "(p && q) || (!p && !q)", "p -> q", "p <-> q", 1, "Exige exatamente uma verdadeira."),
        QE("Em 'p somente se q', q e:", "Condicao suficiente", "Condicao necessaria", "Contradicao", "Conclusao invalida", 2, "Somente se aponta necessidade."),
        QE("Em 'p se q', q e:", "Condicao suficiente", "Condicao necessaria", "Negacao de p", "Equivalente a p", 1, "q garante p."),
        QE("Qual e a forma normal conjuntiva de p -> q?", "(!p || q)", "(p && q)", "(!p && q)", "(p || !q)", 1, "A implicacao vira disjuncao.")
    },
    {
        QE("Qual e a negacao de 'Para todo x, existe y tal que P(x,y)'?", "Existe x tal que para todo y, !P(x,y)", "Para todo x, para todo y, !P(x,y)", "Existe y tal que para todo x, !P(x,y)", "Nao existe x nem y", 1, "Negue os quantificadores em cascata."),
        QE("Se (p -> q), (q -> r) e p sao verdadeiros, o que se conclui?", "q e r", "Apenas q", "Apenas r", "!r", 1, "Use modus ponens duas vezes."),
        QE("Qual formula e equivalente a !(p <-> q)?", "(p && q) || (!p && !q)", "(p || q) && !(p && q)", "p -> !q", "!p -> q", 2, "Negar a bicondicional cria o OU exclusivo."),
        QE("A negacao de 'Existe x tal que para todo y, P(x,y)' e:", "Para todo x, existe y tal que !P(x,y)", "Existe y tal que para todo x, !P(x,y)", "Para todo y, existe x tal que !P(x,y)", "Nenhuma", 1, "Troque a ordem e negue cada bloco."),
        QE("Qual argumento e formalmente valido?", "p->q, q->r, !r, logo !p", "p->q, q, logo p", "p||q, p, logo !q", "!p->q, q, logo !p", 1, "Encadeie e use modus tollens."),
        QE("Se p e necessario e suficiente para q, entao:", "p -> q apenas", "q -> p apenas", "p <-> q", "!p <-> q", 3, "Necessidade e suficiencia formam bicondicional."),
        QE("A formula ((p -> q) && (q -> p)) -> (p <-> q) e:", "Contradicao", "Contingencia", "Tautologia", "Falsa", 3, "O antecedente ja descreve a bicondicional."),
        QE("Qual expressao e equivalente a (p -> q) && (r -> q)?", "(!p || q) && (!r || q)", "(p && r) -> q", "q -> (p || r)", "(!q || p) && (!q || r)", 1, "Expanda cada implicacao."),
        QE("Na frase 'Somente se eu estudar, eu passo', estudar e:", "Condicao suficiente para passar", "Condicao necessaria para passar", "Conclusao invalida", "Equivale a nao passar", 2, "Somente se aponta requisito."),
        QE("Se uma formula tem exatamente 3 linhas verdadeiras em 8, ela e:", "Tautologia", "Contradicao", "Contingencia", "Equivalencia", 3, "Ha mistura de valores."),
        QE("A negacao de 'Nenhum A que e B deixa de ser C' e:", "Existe A que e B e nao e C", "Todo A que e B e C", "Nenhum A e C", "Existe C que nao e A", 1, "Universal negativo vira exemplo positivo."),
        QE("Qual das alternativas expressa p XOR q?", "(p || q) && !(p && q)", "(p && q) || (!p && !q)", "p <-> q", "!p || q", 1, "Exatamente uma proposicao verdadeira."),
        QE("Qual e o valor de ((p -> q) && p) -> q?", "Sempre verdadeiro", "Sempre falso", "Verdadeiro so se p=V", "Falso so se q=F", 1, "E uma forma do modus ponens encapsulada."),
        QE("Se 'Todos os A sao B' e 'Alguns A sao C', qual conclusao e garantida?", "Alguns B sao C", "Todos B sao C", "Nenhum C e B", "Todos C sao A", 1, "Os A citados pertencem a B."),
        QE("A negacao correta de 'Para todo x, P(x) ou Q(x)' e:", "Existe x tal que !P(x) && !Q(x)", "Existe x tal que !P(x) || !Q(x)", "Para todo x, !P(x) && !Q(x)", "Para todo x, !P(x) || !Q(x)", 1, "Negue o universal e a disjuncao."),
        QE("Qual formula representa 'se e somente se'?", "p -> q", "p || q", "p <-> q", "p && q", 3, "Ela exige ida e volta."),
        QE("Em um argumento valido, se as premissas sao verdadeiras, a conclusao:", "Pode ser falsa", "Tambem deve ser verdadeira", "Nunca importa", "Vira contingente", 2, "Essa e a definicao de validade."),
        QE("Qual inferencia e invalida?", "p->q, !q, logo !p", "p->q, p, logo q", "p||q, !p, logo q", "p->q, q, logo p", 4, "E afirmacao do consequente."),
        QE("A frase 'Existe exatamente um x tal que P(x)' combina:", "Existencia e unicidade", "Somente universal", "Somente negacao", "Somente bicondicional", 1, "Afirma que ha um e apenas um."),
        QE("Se p -> q e q -> r, a negacao de p -> r ocorre quando:", "p=F e r=F", "p=V e r=F", "p=F e r=V", "p=V e r=V", 2, "A negacao da implicacao busca o caso de falha.")
    }
};

static const RayQuizQuestionEntry PROG_BANK[RAY_QUIZ_DIFFICULTY_COUNT][RAY_QUIZ_QUESTION_COUNT] = {
    {
        QE("Qual cabecalho declara printf?", "stdlib.h", "stdio.h", "string.h", "math.h", 2, "E a biblioteca de entrada e saida."),
        QE("Qual tipo costuma guardar um caractere?", "double", "char", "int*", "long double", 2, "E o menor tipo textual da linguagem."),
        QE("Qual operador compara igualdade em C?", "=", "==", "!=", "&&", 2, "Nao confunda com atribuicao."),
        QE("Qual comando repete enquanto a condicao for verdadeira?", "if", "while", "typedef", "switch", 2, "E um laco de repeticao."),
        QE("Qual funcao gera numeros pseudoaleatorios?", "fgets", "rand", "printf", "sqrt", 2, "Ela fica em stdlib.h."),
        QE("Qual funcao abre um arquivo?", "scanf", "fopen", "malloc", "puts", 2, "Recebe caminho e modo."),
        QE("Qual especificador imprime int em printf?", "%s", "%f", "%d", "%c", 3, "Use o de decimal inteiro."),
        QE("Qual valor inteiro representa falso em C?", "0", "1", "-1", "2", 1, "Zero e o valor falso padrao."),
        QE("Para que serve #include?", "Apagar memoria", "Importar cabecalhos", "Encerrar o programa", "Declarar vetor", 2, "Ele traz declaracoes externas."),
        QE("Qual operador retorna o resto da divisao inteira?", "/", "*", "%", "&&", 3, "Exemplo: 7 %% 3."),
        QE("Qual palavra-chave define uma estrutura?", "enum", "while", "struct", "break", 3, "Agrupa campos relacionados."),
        QE("Qual laco costuma usar contador explicito?", "for", "if", "switch", "goto", 1, "Tem inicio, condicao e passo."),
        QE("Qual e o retorno padrao de main?", "char", "void*", "int", "float", 3, "Geralmente retorna 0."),
        QE("Qual operador logico representa E?", "||", "&&", "!", "^", 2, "Ambas as partes precisam valer."),
        QE("Qual operador logico representa OU?", "||", "&&", "&", "!", 1, "Basta uma condicao verdadeira."),
        QE("O que x++ faz?", "Decrementa x", "Multiplica x por 2", "Incrementa x em 1", "Zera x", 3, "Adiciona uma unidade."),
        QE("Qual simbolo acessa um elemento de vetor?", "()", "[]", "{}", "<>", 2, "Exemplo: v[2]."),
        QE("Qual comando sai imediatamente de um laco?", "continue", "break", "sizeof", "typedef", 2, "Interrompe a repeticao."),
        QE("Qual comando pula para a proxima iteracao?", "exit", "goto", "continue", "return", 3, "Ele nao encerra o laco."),
        QE("Qual funcao le uma linha inteira com limite de tamanho?", "gets", "fgets", "puts", "atoi", 2, "Ela recebe o tamanho do buffer.")
    },
    {
        QE("O que acontece com um array ao ser passado para funcao em C?", "Vira string", "Decai para ponteiro", "Perde todos os valores", "Muda para struct", 2, "O nome do array passa o endereco inicial."),
        QE("Qual funcao compara duas strings pelo conteudo?", "strcpy", "strlen", "strcmp", "strcat", 3, "Retorna 0 quando sao iguais."),
        QE("Qual funcao mede o tamanho util de uma string?", "sizeof", "strlen", "memcmp", "puts", 2, "Nao conta o terminador nulo."),
        QE("Em C, a expressao v[2] acessa:", "O terceiro elemento", "O segundo elemento", "O quarto elemento", "O ultimo elemento", 1, "Arrays comecam no indice 0."),
        QE("Qual palavra-chave preserva o valor de uma variavel local entre chamadas?", "extern", "volatile", "static", "signed", 3, "Ela mantem armazenamento estatico."),
        QE("Qual e o resultado de 7 / 2 com ambos operandos int?", "3", "3.5", "4", "2.5", 1, "A divisao inteira descarta a fracao."),
        QE("Qual e o risco de usar gets?", "Nao le espacos", "Overflow de buffer", "Nao le numeros", "Fecha o arquivo", 2, "Nao ha controle de tamanho."),
        QE("Qual cabecalho declara malloc e free?", "time.h", "math.h", "stdlib.h", "assert.h", 3, "Memoria dinamica fica nele."),
        QE("O que p + 1 faz quando p e int*?", "Avanca 1 byte", "Avanca 1 inteiro", "Volta uma posicao", "Zera o ponteiro", 2, "A aritmetica usa o tipo apontado."),
        QE("O que scanf(\"%d\", &x) recebe em &x?", "O valor de x", "O endereco de x", "O tipo de x", "O tamanho de x", 2, "scanf precisa saber onde escrever."),
        QE("Qual modo de fopen anexa no fim do arquivo?", "\"r\"", "\"w\"", "\"a\"", "\"rb\"", 3, "Serve para acrescentar sem apagar o conteudo."),
        QE("Qual funcao realoca um bloco dinamico?", "calloc", "realloc", "reset", "resize", 2, "Pode aumentar ou reduzir o tamanho."),
        QE("Qual problema ocorre ao esquecer free apos malloc em loop longo?", "Stack overflow", "Memory leak", "Erro lexico", "Deadlock", 2, "A memoria fica retida."),
        QE("Qual operador testa se dois valores sao diferentes?", "==", "!=", ">=", "++", 2, "Ele usa ponto de exclamacao."),
        QE("Se int x=5; int *p=&x; (*p)++; quanto vale x?", "4", "5", "6", "7", 3, "O ponteiro altera x diretamente."),
        QE("Qual estrutura e mais natural para LIFO?", "Fila", "Pilha", "Grafo", "Matriz", 2, "Ultimo a entrar, primeiro a sair."),
        QE("Qual funcao converte string para int?", "itoa", "atoi", "atof", "strcmp", 2, "Ela interpreta caracteres numericos."),
        QE("Quando uma funcao retorna antes do fim usando return, ela:", "Continua no bloco seguinte", "Encerra a execucao da funcao", "Reinicia a funcao", "Libera toda memoria", 2, "Return sai da funcao."),
        QE("Qual expressao acessa o valor apontado por p?", "&p", "*p", "p&", "p*", 2, "E o operador de dereferencia."),
        QE("Qual e o papel do terminador '\\0' em strings C?", "Iniciar a string", "Separar palavras", "Marcar o fim da string", "Converter para inteiro", 3, "As funcoes de string dependem dele.")
    },
    {
        QE("Qual afirmacao sobre calloc esta correta?", "Nao inicializa memoria", "Inicializa a memoria com zero", "So aloca chars", "Sempre falha em arrays", 2, "Diferenca importante para malloc."),
        QE("Qual palavra-chave cria um alias para tipo?", "struct", "typedef", "define", "union", 2, "Ela evita repetir nomes longos."),
        QE("O que uma struct passada por valor para funcao normalmente faz?", "Compartilha o mesmo espaco automaticamente", "Cria uma copia", "Vira ponteiro", "Perde os campos", 2, "A passagem copia os dados."),
        QE("Qual e a saida de printf(\"%d\", (3>2) && (2>1));?", "0", "1", "2", "3", 2, "As duas comparacoes sao verdadeiras."),
        QE("Qual funcao busca um caractere em string?", "strlen", "strchr", "strncpy", "strspn", 2, "Ela devolve ponteiro para a ocorrencia."),
        QE("Em recursao, o caso-base serve para:", "Aumentar a pilha", "Encerrar as chamadas", "Criar memoria dinamica", "Ordenar vetores", 2, "Sem ele, a funcao nao para."),
        QE("Qual problema e comum em recursao sem caso-base?", "Buffer flush", "Stack overflow", "Implicit cast", "Memory align", 2, "As chamadas se acumulam."),
        QE("Qual operador tem maior precedencia em a + b * c?", "+", "*", "=", "||", 2, "Multiplicacao vem antes."),
        QE("Qual diretiva cria uma constante textual no preprocessador?", "#if", "#include", "#define", "#pragma", 3, "Ela substitui texto antes da compilacao."),
        QE("O que faz o operador sizeof?", "Executa funcao", "Retorna bytes ocupados por um tipo ou objeto", "Converte string em numero", "Aloca memoria", 2, "E resolvido em tempo de compilacao na maioria dos casos."),
        QE("O que acontece ao usar free duas vezes no mesmo ponteiro valido?", "Nada muda", "Comportamento indefinido", "A memoria duplica", "O ponteiro vira NULL automaticamente", 2, "Double free e um erro serio."),
        QE("Qual cabecalho traz memset e memcpy?", "math.h", "string.h", "signal.h", "limits.h", 2, "Funcoes de manipulacao de memoria."),
        QE("Qual estrutura escolhe entre varios valores inteiros com case?", "if", "switch", "while", "for", 2, "A palavra-chave case aparece nela."),
        QE("Qual e o efeito de continue em um for?", "Sai do laco", "Pula para a proxima iteracao", "Reinicia o programa", "Remove o incremento", 2, "O fluxo segue no proximo ciclo."),
        QE("Qual e a vantagem de usar enum?", "Cria memoria dinamica", "Dar nomes simbolicos a inteiros", "Eliminar includes", "Converter ponteiros", 2, "Melhora legibilidade."),
        QE("Ao abrir arquivo com modo \"w\", o que acontece se ele ja existir?", "O conteudo e preservado", "O ponteiro vai para o fim", "O arquivo e truncado", "O programa fecha", 3, "Esse modo recria o conteudo."),
        QE("Qual e o resultado de strcmp quando duas strings sao iguais?", "-1", "0", "1", "EOF", 2, "Igualdade retorna zero."),
        QE("Qual a funcao de fflush(stdout) em programas interativos?", "Fechar stdout", "Forcar a escrita do buffer", "Limpar stdin", "Mover cursor de arquivo", 2, "Ela descarrega o buffer de saida."),
        QE("Qual operador acessa campo de struct via ponteiro?", ".", "::", "->", "&.", 3, "Ele combina dereferencia e acesso."),
        QE("Qual desses cenarios descreve aliasing?", "Dois ponteiros apontam para o mesmo dado", "Um array perde tamanho", "Uma struct muda de tipo", "Um if vira while", 1, "Alterar por um afeta o outro.")
    },
    {
        QE("Qual cabecalho declara FILE?", "ctype.h", "stdio.h", "locale.h", "setjmp.h", 2, "Arquivos vivem na biblioteca padrao de E/S."),
        QE("Qual funcao fecha um arquivo aberto?", "fflush", "fclose", "fend", "closefile", 2, "Ela encerra o stream."),
        QE("O que indica EOF ao ler arquivo texto?", "Fim de arquivo", "Arquivo vazio", "Erro de sintaxe", "Fim de string", 1, "Usado por varias funcoes de leitura."),
        QE("Qual operacao usa bits para deslocar a esquerda?", "<<", ">>", "&&", "||", 1, "Serve para shift a esquerda."),
        QE("Qual expressao testa o bit menos significativo de x?", "x && 1", "x | 1", "x & 1", "x ^ 1", 3, "Use AND bit a bit."),
        QE("Qual palavra-chave impede modificacao do objeto pela referencia declarada?", "volatile", "register", "const", "signed", 3, "A intencao e leitura apenas."),
        QE("O que faz a diretiva #ifndef em include guards?", "Executa o header duas vezes", "Evita inclusoes repetidas", "Abre arquivo externo", "Cria funcao inline", 2, "Protege contra redefinicoes."),
        QE("Qual afirmacao sobre realloc e correta?", "Sempre move o bloco", "Pode devolver o mesmo endereco", "Sempre zera os bytes novos", "Nao pode falhar", 2, "Ele tenta reaproveitar o bloco existente."),
        QE("Qual e o risco de acessar v[10] em um vetor com 10 elementos?", "Truncamento", "Comportamento indefinido", "Promocao inteira", "Overflow aritmetico garantido", 2, "O ultimo indice valido e 9."),
        QE("Qual e o uso tipico de fseek?", "Comparar arquivos", "Mover o cursor do arquivo", "Apagar o arquivo", "Converter arquivo em string", 2, "Reposiciona a leitura/escrita."),
        QE("Qual macro do preprocessador representa o arquivo atual?", "__DATE__", "__FILE__", "__LINE__", "__TIME__", 2, "Ela expande para o nome do arquivo."),
        QE("Qual funcao le bytes brutos de um arquivo binario?", "fprintf", "fread", "fputs", "scanf", 2, "Ela trabalha com blocos de memoria."),
        QE("Qual funcao grava bytes brutos em arquivo binario?", "fwrite", "fgets", "puts", "strcpy", 1, "Pareada com fread."),
        QE("O que significa UB em C?", "Undefined Behavior", "Universal Buffer", "Unsigned Block", "User Branch", 1, "Quando a linguagem nao define o resultado."),
        QE("Qual cuidado e importante ao usar ponteiros apos free?", "Somar 0 ao ponteiro", "Nao reutiliza-lo sem nova atribuicao valida", "Trocar o tipo", "Incrementa-lo", 2, "Depois de free ele fica pendente."),
        QE("Qual dessas expressoes faz XOR bit a bit?", "&", "|", "^", "~", 3, "Retorna 1 nos bits diferentes."),
        QE("Para que serve volatile em geral?", "Criar variavel global", "Evitar que o compilador assuma ausencia de mudancas externas", "Liberar memoria", "Otimizar loops automaticamente", 2, "E util em hardware e concorrencia simples."),
        QE("Qual funcao transforma ponteiro de void* retornado por malloc em desnecessario cast em C puro?", "Nenhuma, o cast nao e necessario", "atoi", "sizeof", "strcmp", 1, "C converte void* implicitamente."),
        QE("Qual problema ocorre ao escrever em memoria alem do buffer?", "Alias seguro", "Buffer overflow", "Link-time error", "Tail recursion", 2, "Pode corromper dados."),
        QE("Qual dessas situacoes pode causar dangling pointer?", "Declarar int x", "Liberar memoria e manter o ponteiro antigo", "Usar const", "Abrir arquivo em leitura", 2, "O endereco fica sem dono valido.")
    },
    {
        QE("Qual resultado e possivel ao usar ponteiro nao inicializado?", "Sempre 0", "Comportamento indefinido", "Erro de sintaxe", "Conversao para NULL", 2, "O ponteiro pode apontar para qualquer lugar."),
        QE("Em C, qual cuidado e correto ao usar realloc em p?", "p = realloc(p,n) sem verificar nada", "Guardar o retorno em ponteiro temporario", "Liberar p antes de chamar realloc", "Usar somente em arrays de char", 2, "Se falhar, o bloco antigo continua valido."),
        QE("Qual afirmacao sobre variavel static local e correta?", "E recriada a cada chamada", "Mantem valor entre chamadas e escopo local", "So pode guardar int", "Fica na stack", 2, "Armazenamento estatico, visibilidade local."),
        QE("Qual e o efeito de modificar string literal em char *s = \"abc\"; s[0]='A';?", "Sempre funciona", "Comportamento indefinido", "Erro obrigatorio em tempo de compilacao", "A string vira dinamica", 2, "Literais nao devem ser modificados."),
        QE("Qual trecho evita leak ao falhar realloc?", "p = realloc(p,n); if(!p) free(p);", "tmp = realloc(p,n); if(tmp) p = tmp;", "free(p); p = realloc(p,n);", "realloc nao falha", 2, "Use ponteiro temporario."),
        QE("Ao fazer memcpy entre areas sobrepostas, a funcao correta e:", "memcpy", "strcpy", "memmove", "memcmp", 3, "Ela lida com sobreposicao."),
        QE("Qual e o risco de retornar endereco de variavel local automatica?", "Retorna memoria invalida apos a funcao", "Nada de especial", "O valor vira const", "A funcao passa a ser inline", 1, "A stack sai de escopo."),
        QE("Em qsort, a funcao de comparacao deve retornar:", "Apenas 0 ou 1", "Negativo, zero ou positivo", "Somente ponteiros", "Nada", 2, "Ela indica ordem relativa."),
        QE("Qual propriedade precisa valer para usar bsearch corretamente?", "O vetor deve estar ordenado segundo o mesmo criterio", "O vetor deve ter tamanho primo", "O vetor deve ser de int", "A busca precisa ser recursiva", 1, "Busca binaria depende de ordenacao."),
        QE("Qual efeito tem setvbuf em streams?", "Fecha o stream", "Configura o buffering", "Converte arquivo para binario", "Reseta EOF", 2, "Permite ajustar o modo e o buffer."),
        QE("Qual e a saida de sizeof(\"abc\")?", "3", "4", "8", "Depende do compilador", 2, "Conta tambem o terminador nulo."),
        QE("Qual e a diferenca central entre stack e heap?", "Stack e manual e heap e automatica", "Stack costuma ser automatica e heap manual", "Nao ha diferenca", "Heap so guarda arquivos", 2, "A gestao de vida util difere."),
        QE("Qual e o resultado de comparar duas strings com ==?", "Compara conteudo sempre", "Compara enderecos", "Converte para int", "Ordena lexicograficamente", 2, "Para conteudo use strcmp."),
        QE("Qual cuidado ao usar fgets seguido de scanf e importante?", "Nada, nunca interagem", "Considerar o \\n restante no buffer", "Usar malloc obrigatoriamente", "Apagar stdout", 2, "Entrada misturada pode surpreender."),
        QE("Qual e a vantagem de const int *p?", "Impede mudar p", "Impede mudar o int apontado por p via esse ponteiro", "Impede ler o valor", "Faz copia do inteiro", 2, "O dado fica somente leitura por essa visao."),
        QE("Qual declaracao representa ponteiro para funcao que retorna int e recebe void?", "int *f(void)", "int (*f)(void)", "int (f*)(void)", "int f*(void)", 2, "Os parenteses prendem o ponteiro."),
        QE("Qual e o efeito de free(NULL)?", "Erro fatal", "Comportamento indefinido", "Nenhum efeito", "Libera toda a heap", 3, "A linguagem permite essa chamada."),
        QE("Qual risco existe ao converter ponteiro inadequado e dereferenciar depois?", "Sempre compila e funciona", "Comportamento indefinido por aliasing/alinhamento", "Apenas warning estetico", "Vira ponteiro nulo", 2, "O tipo do acesso importa."),
        QE("Quando um ponteiro e dito wild pointer?", "Quando aponta para string literal", "Quando nao foi inicializado para endereco valido", "Quando aponta para NULL", "Quando vem de malloc", 2, "Ele nao referencia alvo confiavel."),
        QE("Qual pratica reduz bugs com memoria dinamica?", "Escrever sem checar NULL", "Inicializar ponteiros e zerar ou reapontar apos free", "Usar casts em todos mallocs", "Evitar qualquer struct", 2, "Ajuda a evitar dangling pointers.")
    }
};

static const RayQuizQuestionEntry PORT_BANK[RAY_QUIZ_DIFFICULTY_COUNT][RAY_QUIZ_QUESTION_COUNT] = {
    {
        QE("Qual palavra e mais proxima de 'bonito'?", "Formoso", "Tacanho", "Lugubre", "Avaro", 1, "Pense em beleza."),
        QE("Qual palavra e mais proxima de 'calmo'?", "Placido", "Aspero", "Arisco", "Mordaz", 1, "Ideia de serenidade."),
        QE("Qual palavra e mais proxima de 'rapido'?", "Celere", "Moroso", "Taciturno", "Futil", 1, "Lembra celeridade."),
        QE("Qual palavra e mais proxima de 'antigo'?", "Vetusto", "Tenro", "Hirsuto", "Prolixo", 1, "Termo para algo muito velho."),
        QE("Qual palavra e mais proxima de 'alegre'?", "Jovial", "Soturno", "Inerte", "Tacanho", 1, "Tem clima leve."),
        QE("Qual palavra e mais proxima de 'triste'?", "Lacrimoso", "Ledo", "Arguto", "Farto", 1, "Relaciona-se a lagrimas."),
        QE("Qual palavra e mais proxima de 'medo'?", "Temor", "Louvor", "Relevo", "Amago", 1, "Muito usada em contexto formal."),
        QE("Qual palavra e mais proxima de 'forca'?", "Vigor", "Tedio", "Parcela", "Fissura", 1, "Tambem indica energia."),
        QE("Qual palavra e mais proxima de 'fala'?", "Locucao", "Miragem", "Querela", "Sina", 1, "Relaciona-se a expressao verbal."),
        QE("Qual palavra e mais proxima de 'caminho'?", "Vereda", "Rixa", "Tensao", "Maestria", 1, "Muito usada para trilha."),
        QE("Qual palavra e mais proxima de 'cheiro'?", "Aroma", "Brio", "Estio", "Lavor", 1, "Pode ser agradavel."),
        QE("Qual palavra e mais proxima de 'raiva'?", "Colera", "Candura", "Placidez", "Ardil", 1, "Tambem nomeia uma doenca."),
        QE("Qual palavra e mais proxima de 'leve'?", "Ligeiro", "Gravoso", "Taciturno", "Perecivel", 1, "Pode indicar baixa carga ou rapidez."),
        QE("Qual palavra e mais proxima de 'duvida'?", "Hesitacao", "Jubilo", "Feito", "Fastio", 1, "Relaciona-se a indecisao."),
        QE("Qual palavra e mais proxima de 'barulho'?", "Alarido", "Amago", "Nuvem", "Nexa", 1, "Sugere som espalhafatoso."),
        QE("Qual palavra e mais proxima de 'brilho'?", "Fulgor", "Torpor", "Lacuna", "Avido", 1, "Muito usada em poesia."),
        QE("Qual palavra e mais proxima de 'fome'?", "Inedia", "Melopeia", "Brandura", "Nobreza", 1, "Termo formal para privacao alimentar."),
        QE("Qual palavra e mais proxima de 'sono'?", "Modorra", "Apendice", "Alvoroco", "Vereda", 1, "Da ideia de sonolencia."),
        QE("Qual palavra e mais proxima de 'cuidado'?", "Zelo", "Traje", "Falacia", "Arruaca", 1, "Tambem pode ser atencao."),
        QE("Qual palavra e mais proxima de 'engano'?", "Equivoco", "Remanso", "Candura", "Pudor", 1, "Erro de avaliacao.")
    },
    {
        QE("Qual palavra e mais proxima de 'amizade'?", "Camaradagem", "Acrimonia", "Cizania", "Penuria", 1, "Convivencia amistosa."),
        QE("Qual palavra e mais proxima de 'trabalho'?", "Labuta", "Melopeia", "Apendice", "Sutileza", 1, "Sugere esforco continuo."),
        QE("Qual palavra e mais proxima de 'grande'?", "Vasto", "Parco", "Raso", "Mudo", 1, "Indica ampla extensao."),
        QE("Qual palavra e mais proxima de 'breve'?", "Efemero", "Colossal", "Taciturno", "Probido", 1, "Dura pouco."),
        QE("Qual palavra e mais proxima de 'duro' no sentido severo?", "Acerbo", "Ludico", "Lasso", "Rutilo", 1, "Tambem pode significar amargo."),
        QE("Qual palavra e mais proxima de 'preguica'?", "Lassidao", "Tenacidade", "Fulgor", "Candura", 1, "Traz ideia de moleza."),
        QE("Qual palavra e mais proxima de 'orgulho exagerado'?", "Soberba", "Brandura", "Laconismo", "Melopeia", 1, "Peca capital."),
        QE("Qual palavra e mais proxima de 'confusao'?", "Emaranhado", "Plenitude", "Desvelo", "Alvura", 1, "Algo intrincado."),
        QE("Qual palavra e mais proxima de 'silencio'?", "Taciturnidade", "Rumor", "Estridor", "Acrimonia", 1, "Relaciona-se a ficar calado."),
        QE("Qual palavra e mais proxima de 'frio' emocional?", "Glacial", "Rutilante", "Prisco", "Mordaz", 1, "Parece gelo."),
        QE("Qual palavra e mais proxima de 'sabedoria'?", "Sapiencia", "Molicia", "Frivolidade", "Empafia", 1, "Forma culta bastante usada."),
        QE("Qual palavra e mais proxima de 'erro pequeno'?", "Lapso", "Fastio", "Hegemonia", "Bravata", 1, "Pode ser de memoria."),
        QE("Qual palavra e mais proxima de 'vontade intensa'?", "Avididade", "Parcimonia", "Pusilanimidade", "Laxismo", 1, "Desejo forte."),
        QE("Qual palavra e mais proxima de 'amparo'?", "Esteio", "Estorvo", "Calunia", "Melopeia", 1, "Aquilo que sustenta."),
        QE("Qual palavra e mais proxima de 'clareza'?", "Nitidez", "Lascivia", "Penuria", "Insania", 1, "Algo bem definido."),
        QE("Qual palavra e mais proxima de 'cansaco'?", "Fatigacao", "Temeridade", "Placidez", "Diletancia", 1, "Ligado a fadiga."),
        QE("Qual palavra e mais proxima de 'desordem'?", "Balburdia", "Plenitude", "Lenitivo", "Tino", 1, "Muito usada em debates."),
        QE("Qual palavra e mais proxima de 'remedio' no sentido alivio?", "Lenitivo", "Laconismo", "Acrasia", "Vituperio", 1, "Serve para suavizar."),
        QE("Qual palavra e mais proxima de 'pouco'?", "Parco", "Vasto", "Opimo", "Hirto", 1, "Oposto de abundante."),
        QE("Qual palavra e mais proxima de 'abundante'?", "Opimo", "Parco", "Exiguo", "Tacanho", 1, "Algo rico ou farto.")
    },
    {
        QE("Qual palavra e mais proxima de 'corajoso'?", "Intrepido", "Moroso", "Pusilanime", "Lasso", 1, "Nao teme facilmente."),
        QE("Qual palavra e mais proxima de 'medroso'?", "Pusilanime", "Altivo", "Proficuo", "Sublime", 1, "Falta-lhe coragem."),
        QE("Qual palavra e mais proxima de 'discurso longo e cansativo'?", "Prolixidade", "Candura", "Argucia", "Pujanca", 1, "Excesso de palavras."),
        QE("Qual palavra e mais proxima de 'esperteza'?", "Argucia", "Apatia", "Languidez", "Penuria", 1, "Inteligencia afiada."),
        QE("Qual palavra e mais proxima de 'fraqueza moral'?", "Acrasia", "Resiliencia", "Pujanca", "Jovialidade", 1, "Falta de dominio de si."),
        QE("Qual palavra e mais proxima de 'insulto severo'?", "Vituperio", "Esteio", "Candura", "Sapiencia", 1, "Palavra de reprovacao."),
        QE("Qual palavra e mais proxima de 'insanidade'?", "Desvario", "Fidalguia", "Parcimonia", "Diletancia", 1, "Perda de juizo."),
        QE("Qual palavra e mais proxima de 'economia excessiva'?", "Parcimonia", "Pujanca", "Opulencia", "Diligencia", 1, "Contencao extrema."),
        QE("Qual palavra e mais proxima de 'nobreza de trato'?", "Fidalguia", "Penuria", "Lascivia", "Acrimonia", 1, "Ligada a fidalgo."),
        QE("Qual palavra e mais proxima de 'energia vigorosa'?", "Pujanca", "Languidez", "Inedia", "Molicia", 1, "Forca em expansao."),
        QE("Qual palavra e mais proxima de 'util, proveitoso'?", "Proficuo", "Tacanho", "Exiguo", "Tepido", 1, "Produz bom resultado."),
        QE("Qual palavra e mais proxima de 'tendencia ao prazer mole'?", "Molicia", "Tenacidade", "Rudeza", "Sapiencia", 1, "Termo ligado a comodismo."),
        QE("Qual palavra e mais proxima de 'falta, espaco vazio'?", "Lacuna", "Fulgor", "Esteio", "Bravata", 1, "Muito usada para ausencia de conhecimento."),
        QE("Qual palavra e mais proxima de 'fala afetada e vaidosa'?", "Empafia", "Candura", "Brandura", "Argucia", 1, "Ar de soberba."),
        QE("Qual palavra e mais proxima de 'disciplina cuidadosa'?", "Diligencia", "Acrasia", "Fastio", "Insania", 1, "Sugere zelo ativo."),
        QE("Qual palavra e mais proxima de 'tristeza languida'?", "Languidez", "Jovialidade", "Avididade", "Sapiencia", 1, "Abatimento sem energia."),
        QE("Qual palavra e mais proxima de 'excessiva sensibilidade moral'?", "Melindre", "Esteio", "Opimo", "Pujanca", 1, "Suscetibilidade facil."),
        QE("Qual palavra e mais proxima de 'rigidez, dureza'?", "Aspereza", "Candura", "Lenitivo", "Fidalguia", 1, "Textura ou trato pouco suave."),
        QE("Qual palavra e mais proxima de 'fala breve e concisa'?", "Laconismo", "Prolixidade", "Balburdia", "Vituperio", 1, "Oposto de discurso longo."),
        QE("Qual palavra e mais proxima de 'ardor para fazer algo'?", "Empenho", "Molicia", "Torpor", "Taciturnidade", 1, "Ideia de dedicacao.")
    },
    {
        QE("Qual palavra e mais proxima de 'descanso tranquilo'?", "Remanso", "Vituperio", "Pujanca", "Argucia", 1, "Tambem pode ser trecho de rio calmo."),
        QE("Qual palavra e mais proxima de 'amargura no trato'?", "Acrimonia", "Candura", "Languidez", "Fidalguia", 1, "Aspereza verbal."),
        QE("Qual palavra e mais proxima de 'brancura intensa'?", "Alvura", "Penuria", "Empafia", "Acrasia", 1, "Muito clara."),
        QE("Qual palavra e mais proxima de 'falta extrema de recursos'?", "Penuria", "Esteio", "Pujanca", "Jovialidade", 1, "Escassez severa."),
        QE("Qual palavra e mais proxima de 'descuido moral'?", "Descaso", "Diligencia", "Sapiencia", "Lenitivo", 1, "Ausencia de zelo."),
        QE("Qual palavra e mais proxima de 'agudeza intelectual'?", "Acume", "Torpor", "Melopeia", "Modorra", 1, "Percepcao fina."),
        QE("Qual palavra e mais proxima de 'fala harmoniosa ou canto repetitivo'?", "Melopeia", "Balburdia", "Bravata", "Acrimonia", 1, "Ligada a musicalidade verbal."),
        QE("Qual palavra e mais proxima de 'fanfarronice'?", "Bravata", "Candura", "Remanso", "Lenitivo", 1, "Ato de se gabar."),
        QE("Qual palavra e mais proxima de 'fraqueza de vontade'?", "Pusilanimidade", "Tenacidade", "Diligencia", "Placidez", 1, "Falta de animo para agir."),
        QE("Qual palavra e mais proxima de 'falta de apetite ou jejum prolongado'?", "Inedia", "Opulencia", "Acrasia", "Pujanca", 1, "Privacao alimentar."),
        QE("Qual palavra e mais proxima de 'quietude abatida'?", "Torpor", "Fulgor", "Argucia", "Vigor", 1, "Entorpecimento lento."),
        QE("Qual palavra e mais proxima de 'elogio exagerado a si mesmo'?", "Jactancia", "Candura", "Parcimonia", "Laconismo", 1, "Autoelogio pomposo."),
        QE("Qual palavra e mais proxima de 'atenuado, morno'?", "Tepido", "Glacial", "Acerbo", "Opimo", 1, "Nem frio nem quente."),
        QE("Qual palavra e mais proxima de 'falta de vivacidade'?", "Apatia", "Pujanca", "Empenho", "Argucia", 1, "Indiferenca sem energia."),
        QE("Qual palavra e mais proxima de 'razao sensata'?", "Tino", "Balburdia", "Penuria", "Desvario", 1, "Bom senso."),
        QE("Qual palavra e mais proxima de 'insistencia pouco elegante em detalhes'?", "Minucia", "Fidalguia", "Lenitivo", "Pujanca", 1, "Atencao excessiva ao pequeno."),
        QE("Qual palavra e mais proxima de 'pessoa de linguagem agressiva'?", "Mordaz", "Placido", "Ledo", "Taciturno", 1, "Ataca com palavras."),
        QE("Qual palavra e mais proxima de 'desvio da verdade'?", "Falacia", "Nitidez", "Esteio", "Camaradagem", 1, "Raciocinio enganoso."),
        QE("Qual palavra e mais proxima de 'impureza moral ou intelectual'?", "Macula", "Fulgor", "Zelo", "Argucia", 1, "Marca negativa."),
        QE("Qual palavra e mais proxima de 'tranquilidade suave'?", "Brandura", "Acrimonia", "Vituperio", "Jactancia", 1, "Gentileza sem rigidez.")
    },
    {
        QE("Qual palavra e mais proxima de 'resignacao dolorosa'?", "Comiseracao", "Estupor", "Acatamento", "Lassidao", 3, "Aceitacao com sofrimento."),
        QE("Qual palavra e mais proxima de 'impossibilidade de corrigir'?", "Irreversibilidade", "Parcimonia", "Esmero", "Argucia", 1, "Nao pode voltar ao estado anterior."),
        QE("Qual palavra e mais proxima de 'desprezo altivo'?", "Desdem", "Candura", "Lenitivo", "Placidez", 1, "Olhar superior e frio."),
        QE("Qual palavra e mais proxima de 'excesso de luxo e ostentacao'?", "Fausto", "Penuria", "Acrasia", "Laconismo", 1, "Brilho e riqueza abundantes."),
        QE("Qual palavra e mais proxima de 'suspeita inquieta'?", "Cisma", "Alvura", "Fidalguia", "Pujanca", 1, "Desconfianca persistente."),
        QE("Qual palavra e mais proxima de 'qualidade do que e digno de louvor'?", "Merito", "Inedia", "Torpor", "Bravata", 1, "Aquilo que justifica reconhecimento."),
        QE("Qual palavra e mais proxima de 'razao ou fundamento de algo'?", "Arrimo", "Motivo", "Candura", "Pudor", 2, "Aqui vale no sentido de justificativa."),
        QE("Qual palavra e mais proxima de 'inclinado a zombar'?", "Sarcastico", "Placido", "Ledo", "Proficuo", 1, "Tom de ironia cortante."),
        QE("Qual palavra e mais proxima de 'firmeza de espirito'?", "Fortaleza", "Molicia", "Acrasia", "Lassidao", 1, "Resistencia interior."),
        QE("Qual palavra e mais proxima de 'falta de medida, excesso'?", "Descomedimento", "Parcimonia", "Brandura", "Nitidez", 1, "Ausencia de moderacao."),
        QE("Qual palavra e mais proxima de 'fala rude e violenta'?", "Aresta", "Vituperio", "Laconismo", "Esteio", 2, "Insulto ou repreensao dura."),
        QE("Qual palavra e mais proxima de 'propensao a perdoar'?", "Clemencia", "Acrimonia", "Empafia", "Torpor", 1, "Benevolencia diante da falta."),
        QE("Qual palavra e mais proxima de 'semelhanca enganosa'?", "Simulacro", "Arrimo", "Penuria", "Argucia", 1, "Algo que imita sem ser o original."),
        QE("Qual palavra e mais proxima de 'desanimo profundo'?", "Prostracao", "Pujanca", "Jovialidade", "Esmero", 1, "Abatimento intenso."),
        QE("Qual palavra e mais proxima de 'vontade de fazer o bem'?", "Benquerenca", "Acrasia", "Fastio", "Falacia", 1, "Boa disposicao afetiva."),
        QE("Qual palavra e mais proxima de 'orgulho nobre e digno'?", "Altivez", "Jactancia", "Macula", "Penuria", 1, "Elevacao sem baixeza."),
        QE("Qual palavra e mais proxima de 'clareza de raciocinio'?", "Lucidez", "Apatia", "Melopeia", "Lassidao", 1, "Pensamento nitido."),
        QE("Qual palavra e mais proxima de 'rigor exagerado'?", "Severidade", "Brandura", "Candura", "Lenitivo", 1, "Dureza no julgamento."),
        QE("Qual palavra e mais proxima de 'virtude de quem e discreto e envergonhado'?", "Pudor", "Pujanca", "Acrasia", "Torpor", 1, "Sentimento de recato."),
        QE("Qual palavra e mais proxima de 'ato de renunciar'?", "Abdicacao", "Pujanca", "Jactancia", "Acume", 1, "Abrir mao de algo.")
    }
};

static const RayQuizQuestionEntry MATH_BANK[RAY_QUIZ_DIFFICULTY_COUNT][RAY_QUIZ_QUESTION_COUNT] = {
    {
        QE("Se x + 7 = 12, quanto vale x?", "3", "4", "5", "6", 3, "Isole x."),
        QE("Se 2x = 18, quanto vale x?", "7", "8", "9", "10", 3, "Divida por 2."),
        QE("Resolva: 3x - 5 = 10.", "3", "4", "5", "6", 2, "Some 5 antes de dividir."),
        QE("Se x / 4 = 6, quanto vale x?", "18", "20", "24", "28", 3, "Multiplique por 4."),
        QE("Resolva: 2x + 3 = 11.", "2", "3", "4", "5", 3, "Subtraia 3 e divida por 2."),
        QE("Quanto e 20% de 50?", "5", "10", "15", "20", 2, "Um quinto de 50."),
        QE("A media de 4, 6 e 8 e:", "5", "6", "7", "8", 2, "Some e divida por 3."),
        QE("Quanto vale 5^2?", "10", "20", "25", "30", 3, "Cinco ao quadrado."),
        QE("Quanto vale a raiz quadrada de 81?", "7", "8", "9", "10", 3, "Quadrado perfeito."),
        QE("A area de um retangulo 5 por 3 e:", "8", "12", "15", "18", 3, "Base vezes altura."),
        QE("O perimetro de um quadrado de lado 4 e:", "8", "12", "16", "20", 3, "Soma dos quatro lados."),
        QE("Se 3/4 de um numero e 12, esse numero e:", "14", "15", "16", "18", 3, "Multiplique por 4/3."),
        QE("Se f(x)=2x+1, quanto vale f(3)?", "5", "6", "7", "8", 3, "Substitua x por 3."),
        QE("Se y=3x, quanto vale y quando x=4?", "7", "10", "12", "14", 3, "Substitua x."),
        QE("Quanto e 30% de 200?", "40", "50", "60", "70", 3, "Tres decimos de 200."),
        QE("Se um produto custa 80 e sobe 10%, passa a custar:", "84", "86", "88", "90", 3, "10% de 80 e 8."),
        QE("Se 4x = 4, entao x vale:", "0", "1", "2", "4", 2, "Divida por 4."),
        QE("Quanto vale 9 + 6 / 3?", "5", "9", "11", "15", 3, "A divisao vem antes da soma."),
        QE("Se 7 + 8 = 15, o dobro desse resultado e:", "20", "24", "30", "32", 3, "Multiplique 15 por 2."),
        QE("Qual e a solucao de x + 2 > 5?", "x > 3", "x < 3", "x = 3", "x >= 2", 1, "Isole x na inequacao.")
    },
    {
        QE("Qual e a solucao de 2x <= 10?", "x <= 10", "x <= 5", "x >= 5", "x >= 10", 2, "Divida por 2."),
        QE("A funcao y = x + 2 e de qual tipo?", "Quadratica", "Linear", "Exponencial", "Trigonometrica", 2, "Expoente maximo igual a 1."),
        QE("O grafico de y = 5 e:", "Reta horizontal", "Reta vertical", "Parabola", "Circulo", 1, "y nao muda."),
        QE("Se f(x)=x-4, qual e a raiz?", "2", "3", "4", "5", 3, "A raiz zera a funcao."),
        QE("A derivada de x^2 e:", "x", "2x", "x^3", "2", 2, "Regra da potencia."),
        QE("Se um triangulo retangulo tem catetos 6 e 8, a hipotenusa e:", "9", "10", "11", "12", 2, "Use Pitagoras."),
        QE("Se 2^x = 32, quanto vale x?", "4", "5", "6", "7", 2, "Pense nas potencias de 2."),
        QE("A soma das raizes de x^2 - 7x + 10 = 0 e:", "5", "7", "10", "12", 2, "Use -b/a."),
        QE("O coeficiente angular da reta y = -3x + 8 e:", "-3", "3", "8", "-8", 1, "E o numero que acompanha x."),
        QE("Se a1=4 e r=3 em uma PA, qual e a4?", "10", "11", "12", "13", 4, "Some a razao tres vezes."),
        QE("A media ponderada de 7 e 9 com pesos 1 e 3 e:", "8", "8,5", "8,75", "9", 3, "O 9 pesa mais."),
        QE("Se x^2 = 49, quais as solucoes reais?", "Apenas 7", "Apenas -7", "7 e -7", "Nenhuma", 3, "Quadrado de positivo e negativo pode coincidir."),
        QE("Em inequacoes, ao multiplicar por numero negativo, o sinal:", "Permanece", "Inverte", "Some", "Vira igualdade", 2, "Regra basica de inequacoes."),
        QE("Se sen^2 t + cos^2 t = ?", "0", "1", "2", "Depende de t", 2, "Identidade fundamental."),
        QE("Em uma PG com a1=2 e razao 3, a3 vale:", "6", "9", "18", "27", 3, "Multiplique duas vezes pela razao."),
        QE("A derivada de uma constante e:", "0", "1", "A propria constante", "x", 1, "Constante nao varia."),
        QE("Se f(x)=|x|, quanto vale f(-7)?", "-7", "0", "7", "14", 3, "Valor absoluto remove o sinal."),
        QE("O sistema x+y=10 e x-y=2 tem solucao:", "(6,4)", "(4,6)", "(5,5)", "(8,2)", 1, "Some as equacoes."),
        QE("Se o discriminante de uma quadratica e negativo, as raizes reais sao:", "Duas distintas", "Iguais", "Inexistentes", "Sempre inteiras", 3, "Nao ha corte no eixo x."),
        QE("Multiplicar por 1,25 equivale a aumento de:", "12,5%", "20%", "25%", "125%", 3, "1,25 = 1 + 0,25.")
    },
    {
        QE("Se f(x)=x^2 - 5x + 6, quais sao as raizes?", "1 e 6", "2 e 3", "3 e 4", "2 e 6", 2, "Fatore a expressao."),
        QE("A solucao de 3x - 7 >= 11 e:", "x >= 6", "x >= 5", "x <= 6", "x <= 5", 1, "Some 7 e divida por 3."),
        QE("Se g(x)=2x-1, qual e g^-1(x)?", "(x-1)/2", "(x+1)/2", "2x+1", "1-2x", 2, "Troque x por y e isole."),
        QE("Qual e o vertice da parabola y = x^2 - 4x + 3?", "(2,-1)", "(2,1)", "(4,-3)", "(-2,1)", 1, "Use -b/2a."),
        QE("A derivada de 3x^2 + 2x - 5 e:", "6x + 2", "3x + 2", "6x - 5", "2x + 3", 1, "Derive termo a termo."),
        QE("A integral de 2x dx e:", "x^2 + C", "2x + C", "x + C", "x^3 + C", 1, "Inversa da derivada."),
        QE("Se h(x)=1/x, quanto vale h(1/2)?", "1/4", "1/2", "2", "4", 3, "Dividir por meio equivale a multiplicar por 2."),
        QE("Qual e o dominio real de 1/(x-3)?", "Todos os reais", "Todos os reais exceto 0", "Todos os reais exceto 3", "Somente x > 3", 3, "O denominador nao pode zerar."),
        QE("A funcao y = -x^2 + 4 e concava para:", "Cima", "Baixo", "Ambos", "Nenhum", 2, "Coeficiente de x^2 negativo."),
        QE("Qual e a imagem de x=-1 na funcao y=x^3?", "-3", "-1", "1", "3", 2, "Expoente impar preserva o sinal."),
        QE("Se log10(x)=2, entao x vale:", "20", "100", "1000", "2", 2, "10^2."),
        QE("Se a soma de dois numeros e 14 e a diferenca e 2, o maior e:", "6", "7", "8", "9", 3, "Monte um sistema simples."),
        QE("Se um angulo interno de triangulo mede 40 e outro 65, o terceiro mede:", "55", "65", "75", "85", 3, "A soma interna vale 180."),
        QE("Na funcao y=3x-2, qual e a imagem de x=5?", "11", "12", "13", "14", 3, "Substitua x."),
        QE("Se x^2 - 9 = 0, entao x pode ser:", "3 apenas", "-3 apenas", "3 ou -3", "Nenhum", 3, "Produto notavel."),
        QE("Em uma PA com a1=10 e r=-2, o quinto termo e:", "0", "2", "4", "6", 2, "Subtraia a razao quatro vezes."),
        QE("Qual e a soma dos 5 primeiros termos da PA 2,4,6...?", "20", "25", "30", "35", 3, "2+4+6+8+10."),
        QE("A distancia entre 0 e -8 na reta real e:", "8", "-8", "0", "16", 1, "Distancia e modulo da diferenca."),
        QE("Se cos(0) = ?", "0", "1", "-1", "Nao existe", 2, "Valor trigonometrico basico."),
        QE("Se uma reta tem inclinacao zero, ela e:", "Vertical", "Horizontal", "Parabola", "Obliqua crescente", 2, "y constante.")
    },
    {
        QE("O determinante da matriz [[2,1],[3,4]] e:", "5", "8", "11", "13", 1, "ad - bc."),
        QE("Quantas combinacoes de 2 elementos podem ser feitas com 5 elementos?", "5", "10", "15", "20", 2, "C(5,2)."),
        QE("Qual e o limite de (2x+1) quando x tende a 3?", "5", "6", "7", "8", 3, "Substituicao direta."),
        QE("A derivada de x^2 * x e:", "2x", "3x^2", "x^3", "2x^2", 2, "Simplifique primeiro."),
        QE("Se |x-2| < 3, entao x pertence a:", "(-1,5)", "(2,5)", "(-3,3)", "(0,3)", 1, "Abra a desigualdade modular."),
        QE("Qual e o resto da divisao de x^2+3x+2 por x+1?", "0", "1", "2", "3", 1, "Use o teorema do resto em x=-1."),
        QE("Se ln(e^4) = ?", "1", "2", "4", "e", 3, "Funcoes inversas."),
        QE("Qual e a probabilidade de sair cara em moeda justa?", "1/4", "1/3", "1/2", "2/3", 3, "Duas faces equiprovaveis."),
        QE("Em um triangulo retangulo, se a hipotenusa e 13 e um cateto e 5, o outro e:", "8", "10", "12", "14", 3, "Pitagoras: 13^2 - 5^2."),
        QE("Qual e a solucao de x^2 - 4x - 5 = 0?", "1 e 5", "-1 e 5", "-5 e 1", "2 e -2", 2, "Fatoracao por soma e produto."),
        QE("Se log2(32)=?", "4", "5", "6", "8", 2, "2 elevado a 5."),
        QE("A area de um circulo de raio 3 e:", "6pi", "9pi", "12pi", "18pi", 2, "pi r^2."),
        QE("A soma dos angulos internos de um quadrilatero e:", "180", "270", "360", "540", 3, "Pode ser dividido em dois triangulos."),
        QE("Se det(A)=5 e det(B)=2, entao det(AB)=?", "3", "7", "10", "25", 3, "Determinantes multiplicam."),
        QE("A derivada de sen(x) e:", "cos(x)", "-cos(x)", "sen(x)", "-sen(x)", 1, "Derivada trigonometrica basica."),
        QE("Se uma funcao e crescente e injetora, sua inversa e:", "Constante", "Tambem injetora", "Necessariamente quadratica", "Indefinida", 2, "Injetividade garante inversa funcional."),
        QE("Quantos anagramas a palavra SOL possui?", "3", "6", "9", "12", 2, "3!"),
        QE("Qual e o valor de 0! ?", "0", "1", "Nao existe", "2", 2, "Fatorial de zero e definido."),
        QE("Se p(A)=0,4 e p(B)=0,5 para eventos independentes, p(A e B) = ?", "0,2", "0,45", "0,9", "0,1", 1, "Independencia multiplica probabilidades."),
        QE("A mediana do conjunto 2,4,7,9,11 e:", "4", "7", "8", "9", 2, "Valor central da lista ordenada.")
    },
    {
        QE("Qual e o limite de (x^2-1)/(x-1) quando x tende a 1?", "0", "1", "2", "Indefinido", 3, "Fatore antes de substituir."),
        QE("Em uma PG infinita de a1=3 e q=1/2, a soma e:", "3", "4", "5", "6", 4, "Use a1/(1-q)."),
        QE("Quantas permutacoes distintas tem a palavra CASA?", "12", "24", "6", "4", 1, "4!/2! por causa do A repetido."),
        QE("Se f(x)= (3x-2)/(x+1), qual e f^-1(1)?", "1", "3/2", "Nao existe", "0", 3, "Procure x tal que f(x)=1."),
        QE("A inequacao x^2 - 5x + 6 < 0 vale para:", "x<2 ou x>3", "2<x<3", "x>6", "x<0", 2, "Analise o sinal entre as raizes."),
        QE("Qual e a probabilidade de tirar um As de um baralho comum?", "1/4", "1/13", "1/26", "4/13", 2, "Ha 4 ases em 52 cartas."),
        QE("O determinante da matriz [[1,2],[5,7]] e:", "-3", "-2", "3", "2", 1, "ad - bc."),
        QE("Se z = 3 + 4i, o modulo de z e:", "4", "5", "7", "25", 2, "Raiz de 3^2 + 4^2."),
        QE("Qual e a derivada de ln(x)?", "1/x", "ln(x)", "x", "e^x", 1, "Regra basica."),
        QE("Se uma amostra tem media 10 e desvio padrao 0, entao:", "Todos os valores sao 10", "Metade vale 10", "A media esta errada", "Nao ha moda", 1, "Nao existe dispersao."),
        QE("A combinacao C(6,3) vale:", "18", "20", "24", "36", 2, "6!/(3!3!)."),
        QE("Se sin(x)=1, um valor possivel para x em radianos e:", "0", "pi/2", "pi", "3pi/2", 2, "Pico superior do seno."),
        QE("A matriz identidade 2x2 tem determinante:", "0", "1", "2", "-1", 2, "Produto da diagonal principal."),
        QE("Qual e a raiz positiva de x^2 = 18?", "3", "3raiz2", "2raiz3", "9", 2, "Simplifique a raiz."),
        QE("O volume de um cubo de aresta 4 e:", "16", "32", "64", "128", 3, "a^3."),
        QE("Se p(A)=0,7 e p(B)=0,4, qual o menor valor possivel de p(A inter B)?", "0", "0,1", "0,3", "0,4", 2, "Use maximo entre 0 e p(A)+p(B)-1."),
        QE("A derivada de e^x e:", "x e^(x-1)", "e^x", "1/x", "0", 2, "Ela se conserva."),
        QE("Se uma funcao tem derivada sempre positiva em um intervalo, ela e:", "Crescente", "Decrescente", "Constante", "Periodica", 1, "Sinal da derivada indica monotonia."),
        QE("Qual e o valor de cos(pi)?", "1", "0", "-1", "Nao existe", 3, "Ponto extremo esquerdo do circulo trigonometrico."),
        QE("Em uma distribuicao simetrica, media e mediana:", "Sempre diferem", "Podem coincidir", "Nunca existem", "Valem zero", 2, "Muitas vezes ocupam o mesmo centro.")
    }
};

static const char *ray_quiz_category_name(RayQuizCategory category) {
    switch (category) {
        case RAY_QUIZ_CATEGORY_LOGIC_CLASSIC: return "Logica Classica";
        case RAY_QUIZ_CATEGORY_MATH_LOGIC: return "Logica Matematica";
        case RAY_QUIZ_CATEGORY_C_PROGRAMMING: return "Logica de Prog. em C";
        case RAY_QUIZ_CATEGORY_PORTUGUESE: return "Significados em Portugues";
        default: return "Categoria";
    }
}

static const char *ray_quiz_difficulty_name(RayQuizDifficulty difficulty) {
    switch (difficulty) {
        case RAY_QUIZ_DIFFICULTY_FACIL: return "Facil";
        case RAY_QUIZ_DIFFICULTY_AVANCADA: return "Avancada";
        case RAY_QUIZ_DIFFICULTY_INTERMEDIARIA: return "Intermediaria";
        case RAY_QUIZ_DIFFICULTY_DIFICIL: return "Dificil";
        case RAY_QUIZ_DIFFICULTY_MUITO_DIFICIL: return "Muito Dificil";
        default: return "Dificuldade";
    }
}

static RayQuizDifficulty ray_quiz_difficulty_from_bet(int bet) {
    if (bet <= 2) return RAY_QUIZ_DIFFICULTY_FACIL;
    if (bet <= 4) return RAY_QUIZ_DIFFICULTY_AVANCADA;
    if (bet <= 6) return RAY_QUIZ_DIFFICULTY_INTERMEDIARIA;
    return RAY_QUIZ_DIFFICULTY_DIFICIL;
}

static const RayQuizQuestionEntry *ray_quiz_bank_entry(RayQuizCategory category, RayQuizDifficulty difficulty, int index) {
    if (index < 0) index = 0;
    index %= RAY_QUIZ_QUESTION_COUNT;
    switch (category) {
        case RAY_QUIZ_CATEGORY_LOGIC_CLASSIC:
            return &LOGIC_BANK[difficulty][index];
        case RAY_QUIZ_CATEGORY_MATH_LOGIC:
            return &MATH_BANK[difficulty][index];
        case RAY_QUIZ_CATEGORY_C_PROGRAMMING:
            return &PROG_BANK[difficulty][index];
        case RAY_QUIZ_CATEGORY_PORTUGUESE:
            return &PORT_BANK[difficulty][index];
        default:
            return &LOGIC_BANK[RAY_QUIZ_DIFFICULTY_FACIL][0];
    }
}

static RayQuizCategory ray_quiz_category_from_mask_pick(int question_mask) {
    RayQuizCategory available[RAY_QUIZ_CATEGORY_COUNT];
    int count = 0;

    if (question_mask & PLAYER_MASK_LOGIC) available[count++] = RAY_QUIZ_CATEGORY_LOGIC_CLASSIC;
    if (question_mask & PLAYER_MASK_MAT) available[count++] = RAY_QUIZ_CATEGORY_MATH_LOGIC;
    if (question_mask & PLAYER_MASK_PROG) available[count++] = RAY_QUIZ_CATEGORY_C_PROGRAMMING;
    if (question_mask & PLAYER_MASK_PORT) available[count++] = RAY_QUIZ_CATEGORY_PORTUGUESE;

    if (count <= 0) return (RayQuizCategory)(rand() % RAY_QUIZ_CATEGORY_COUNT);
    return available[rand() % count];
}

static RayQuizQuestion ray_quiz_draw(int question_mask, RayQuizDifficulty difficulty) {
    RayQuizCategory category = ray_quiz_category_from_mask_pick(question_mask);
    const RayQuizQuestionEntry *entry = ray_quiz_bank_entry(category, difficulty, rand() % RAY_QUIZ_QUESTION_COUNT);
    RayQuizQuestion question;

    question.question = entry->question;
    for (int i = 0; i < 4; i++) question.options[i] = entry->options[i];
    question.answer = entry->answer;
    question.hint = entry->hint;
    question.category = category;
    question.difficulty = difficulty;
    return question;
}

#undef QE

#endif
