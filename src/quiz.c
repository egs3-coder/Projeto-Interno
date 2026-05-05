#include "quiz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *question;
    const char *options[4];
    int answer;
    const char *hint;
} Question;

static Question easy_questions[] = {
    {"Qual funcao gera numeros pseudoaleatorios em C?", {"printf", "rand", "fgets", "sqrt"}, 2, "Ela fica em stdlib.h."},
    {"Qual funcao abre arquivo texto em C?", {"fopen", "scanf", "malloc", "pow"}, 1, "Comeca com f."},
    {"Qual estrutura permite repetir comandos enquanto a condicao for verdadeira?", {"if", "struct", "while", "typedef"}, 3, "E um laco."},
    {"Qual valor representa verdadeiro em C quando usamos int?", {"0", "qualquer valor diferente de 0", "apenas -1", "apenas 100"}, 2, "Zero e falso."}
};

static Question hard_questions[] = {
    {"No projeto PIF, qual funcao pode calcular a raiz do desvio padrao?", {"sqrt", "rand", "strtok", "fgets"}, 1, "Fica em math.h."},
    {"Qual funcao e indicada para ler uma linha de arquivo com seguranca?", {"gets", "fgets", "fprintf", "srand"}, 2, "Tem um f no inicio e recebe tamanho do buffer."},
    {"Em recursao, o que evita chamadas infinitas?", {"ponteiro", "caso-base", "compilador", "arquivo"}, 2, "E a condicao de parada."},
    {"No formato do historico, os campos sao separados por qual caractere?", {"virgula", "ponto", "ponto e virgula", "barra"}, 3, "O CSV de palpites usa virgula, os campos usam outro separador."}
};

static int read_answer(void) {
    char line[64];
    while (1) {
        printf("Resposta (1-4): ");
        if (!fgets(line, sizeof(line), stdin)) return 0;
        char *endptr = NULL;
        long value = strtol(line, &endptr, 10);
        if (endptr == line || value < 1 || value > 4) {
            printf("Entrada invalida.\n");
            continue;
        }
        return (int)value;
    }
}

QuizResult ask_quiz(int hard_mode) {
    QuizResult result = {0, 0, 0};
    Question *pool = hard_mode ? hard_questions : easy_questions;
    int count = hard_mode ? (int)(sizeof(hard_questions) / sizeof(hard_questions[0]))
                          : (int)(sizeof(easy_questions) / sizeof(easy_questions[0]));
    Question q = pool[rand() % count];

    printf("\n========== QUIZ %s ==========" "\n", hard_mode ? "BOSS" : "RAPIDO");
    printf("%s\n", q.question);
    for (int i = 0; i < 4; i++) {
        printf("%d) %s\n", i + 1, q.options[i]);
    }

    int answer = read_answer();
    if (answer == q.answer) {
        result.correct = 1;
        result.coins = hard_mode ? 30 : 15;
        printf("Correto! +%d moedas.\n", result.coins);
    } else {
        result.correct = 0;
        result.coins = hard_mode ? 0 : 5;
        result.apply_debuff = hard_mode ? 1 : 0;
        printf("Errou. Dica: %s\n", q.hint);
        if (result.coins > 0) printf("Recompensa reduzida: +%d moedas.\n", result.coins);
        if (result.apply_debuff) printf("Debuff do Boss ativado nesta fase.\n");
    }
    return result;
}
