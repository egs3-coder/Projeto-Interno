#include "analysis.h"
#include "game.h"
#include "history.h"
#include "pif_guess.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int read_main_choice(void) {
    char line[64];
    printf("Opcao: ");
    if (!fgets(line, sizeof(line), stdin)) return 0;
    return atoi(line);
}

int main(void) {
    srand((unsigned int)time(NULL));

    int running = 1;
    while (running) {
        printf("\n==============================================\n");
        printf(" PIF 2026.1 - Jogo de Cartas + Adivinhacao\n");
        printf("==============================================\n");
        printf("1) Jogar run de cartas do Projeto Interno\n");
        printf("2) Jogar adivinhacao PIF e salvar historico\n");
        printf("3) Analisar historico - Capstone 2\n");
        printf("0) Sair\n");

        int choice = read_main_choice();
        switch (choice) {
            case 1:
                play_card_run();
                break;
            case 2:
                play_guessing_game(HISTORY_FILE);
                break;
            case 3:
                analyze_history_menu(HISTORY_FILE);
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    }

    printf("Ate a proxima!\n");
    return 0;
}
