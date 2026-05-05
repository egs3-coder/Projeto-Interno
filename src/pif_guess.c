#include "pif_guess.h"
#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_int_between(const char *prompt, int min, int max) {
    char line[128];
    while (1) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) return min;
        char *endptr = NULL;
        long value = strtol(line, &endptr, 10);
        if (endptr == line || (*endptr != '\n' && *endptr != '\0')) {
            printf("Entrada invalida. Digite um numero inteiro.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Valor fora do intervalo. Digite entre %d e %d.\n", min, max);
            continue;
        }
        return (int)value;
    }
}

void play_guessing_game(const char *history_filename) {
    Session session;
    memset(&session, 0, sizeof(Session));
    make_timestamp(session.timestamp, sizeof(session.timestamp));
    session.target = 1 + rand() % 100;

    printf("\n========== MODO PIF: JOGO DE ADIVINHACAO ==========" "\n");
    printf("Adivinhe o numero entre 1 e 100.\n");

    while (session.guesses_count < MAX_GUESSES) {
        int guess = read_int_between("Palpite: ", 1, 100);
        session.guesses[session.guesses_count++] = guess;
        session.attempts++;

        if (guess < session.target) {
            session.low_count++;
            printf("Muito baixo.\n");
        } else if (guess > session.target) {
            session.high_count++;
            printf("Muito alto.\n");
        } else {
            printf("Acertou! Alvo %d em %d tentativa(s).\n", session.target, session.attempts);
            break;
        }
    }

    if (history_append(history_filename, &session)) {
        printf("Sessao salva em '%s'.\n", history_filename);
    } else {
        printf("ERRO: nao foi possivel abrir '%s' para escrita. A partida terminou normalmente.\n", history_filename);
    }
}
