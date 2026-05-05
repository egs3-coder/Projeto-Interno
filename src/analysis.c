#include "analysis.h"
#include "history.h"
#include <math.h>
#include <stdio.h>

static int rec_sum_attempts(Session sessions[], int n) {
    if (n <= 0) return 0;
    return sessions[n - 1].attempts + rec_sum_attempts(sessions, n - 1);
}

static int rec_min_attempts(Session sessions[], int n) {
    if (n == 1) return sessions[0].attempts;
    int prev = rec_min_attempts(sessions, n - 1);
    return sessions[n - 1].attempts < prev ? sessions[n - 1].attempts : prev;
}

static int rec_max_attempts(Session sessions[], int n) {
    if (n == 1) return sessions[0].attempts;
    int prev = rec_max_attempts(sessions, n - 1);
    return sessions[n - 1].attempts > prev ? sessions[n - 1].attempts : prev;
}

static double rec_sum_sq_diff(Session sessions[], int n, double mean) {
    if (n <= 0) return 0.0;
    double diff = sessions[n - 1].attempts - mean;
    return diff * diff + rec_sum_sq_diff(sessions, n - 1, mean);
}

static int rec_monotonic_steps(const int values[], int n, int direction) {
    if (n <= 1) return 0;
    int ok = 0;
    if (direction > 0 && values[n - 1] > values[n - 2]) ok = 1;
    if (direction < 0 && values[n - 1] < values[n - 2]) ok = 1;
    return ok + rec_monotonic_steps(values, n - 1, direction);
}

static double average_low_bias(Session sessions[], int n) {
    double total = 0.0;
    for (int i = 0; i < n; i++) {
        if (sessions[i].attempts > 0) total += (double)sessions[i].low_count / sessions[i].attempts;
    }
    return n > 0 ? total / n : 0.0;
}

static double average_high_bias(Session sessions[], int n) {
    double total = 0.0;
    for (int i = 0; i < n; i++) {
        if (sessions[i].attempts > 0) total += (double)sessions[i].high_count / sessions[i].attempts;
    }
    return n > 0 ? total / n : 0.0;
}

static int count_far_first_guesses(Session sessions[], int n) {
    int far = 0;
    for (int i = 0; i < n; i++) {
        if (sessions[i].guesses_count > 0) {
            int diff = sessions[i].guesses[0] - sessions[i].target;
            if (diff < 0) diff = -diff;
            if (diff >= 35) far++;
        }
    }
    return far;
}

static int count_monotonic_sessions(Session sessions[], int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        int inc = rec_monotonic_steps(sessions[i].guesses, sessions[i].guesses_count, 1);
        int dec = rec_monotonic_steps(sessions[i].guesses, sessions[i].guesses_count, -1);
        int limit = sessions[i].guesses_count - 1;
        if (limit > 0 && (inc >= limit - 1 || dec >= limit - 1)) total++;
    }
    return total;
}

void analyze_history_menu(const char *filename) {
    Session sessions[MAX_SESSIONS];
    int n = history_load(filename, sessions, MAX_SESSIONS);

    if (n <= 0) {
        printf("\nNenhum historico encontrado em '%s'. Jogue uma partida de adivinhacao primeiro.\n", filename);
        return;
    }

    int sum = rec_sum_attempts(sessions, n);
    double mean = (double)sum / n;
    int min = rec_min_attempts(sessions, n);
    int max = rec_max_attempts(sessions, n);
    double variance = rec_sum_sq_diff(sessions, n, mean) / n;
    double stddev = sqrt(variance);
    double low_bias = average_low_bias(sessions, n);
    double high_bias = average_high_bias(sessions, n);
    int far_first = count_far_first_guesses(sessions, n);
    int monotonic = count_monotonic_sessions(sessions, n);

    printf("\n========== RELATORIO ANALITICO - CAPSTONE 2 ==========" "\n");
    printf("Total de sessoes: %d\n", n);
    printf("Media de tentativas: %.2f\n", mean);
    printf("Melhor sessao: %d tentativa(s)\n", min);
    printf("Pior sessao: %d tentativa(s)\n", max);
    printf("Desvio padrao populacional: %.2f\n", stddev);
    printf("Vies medio para baixo: %.2f%%\n", low_bias * 100.0);
    printf("Vies medio para alto: %.2f%%\n", high_bias * 100.0);
    printf("Sessoes com primeiro palpite muito distante: %d\n", far_first);
    printf("Sessoes com tendencia monotona: %d\n", monotonic);

    printf("\nSugestoes de estrategia:\n");
    if (far_first > n / 3) {
        printf("- O chute inicial esta muito distante em muitas partidas. Comece perto de 50.\n");
    } else {
        printf("- O chute inicial esta razoavel na maior parte das partidas.\n");
    }

    if (monotonic > n / 3) {
        printf("- Ha sinais de busca linear. Tente cortar o intervalo pela metade a cada dica.\n");
    }

    if (mean <= 7.0) {
        printf("- Boa aproximacao da busca binaria para o intervalo 1..100.\n");
    } else if (stddev < 2.0 && mean > 9.0) {
        printf("- Media alta e desvio baixo: estrategia repetitiva e pouco eficiente.\n");
    } else {
        printf("- Ainda ha espaco para reduzir a media de tentativas com chutes mais centrais.\n");
    }

    if (low_bias > 0.60) {
        printf("- Voce tende a chutar baixo demais. Aumente mais agressivamente apos dica 'Muito baixo'.\n");
    }
    if (high_bias > 0.60) {
        printf("- Voce tende a chutar alto demais. Reduza mais agressivamente apos dica 'Muito alto'.\n");
    }

    printf("\nUltimas sessoes carregadas:\n");
    int start = n > 5 ? n - 5 : 0;
    for (int i = start; i < n; i++) {
        session_print(&sessions[i]);
    }
}
