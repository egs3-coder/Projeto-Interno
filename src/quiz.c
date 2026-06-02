#include "quiz.h"
#include "question_bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int count = 0;
    const QuizQuestion *pool = quiz_question_pool(hard_mode, &count);
    QuizQuestion q = pool[rand() % count];

    printf("\n========== QUIZ %s ==========" "\n", hard_mode ? "BOSS" : "RAPIDO");
    printf("%s\n", q.question);
    for (int i = 0; i < 4; i++) {
        printf("%d) %s\n", i + 1, q.options[i]);
    }

    int answer = read_answer();
    if (answer == q.answer) {
        result.correct = 1;
        result.coins = hard_mode ? 3 : 2;
        printf("Correto! +%d moedas.\n", result.coins);
    } else {
        result.correct = 0;
        result.coins = 0;
        result.apply_debuff = hard_mode ? 1 : 0;
        printf("Errou. Dica: %s\n", q.hint);
        if (result.apply_debuff) printf("Debuff do Boss ativado nesta fase.\n");
    }
    return result;
}
