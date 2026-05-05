#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void make_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    if (!local) {
        snprintf(buffer, size, "0000-00-00 00:00:00");
        return;
    }
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", local);
}

int history_append(const char *filename, const Session *session) {
    FILE *fp = fopen(filename, "a");
    if (!fp) return 0;

    fprintf(fp, "%s;%d;%d;%d;%d;", session->timestamp, session->target,
            session->attempts, session->low_count, session->high_count);
    for (int i = 0; i < session->guesses_count; i++) {
        fprintf(fp, "%d", session->guesses[i]);
        if (i + 1 < session->guesses_count) fprintf(fp, ",");
    }
    fprintf(fp, "\n");
    fclose(fp);
    return 1;
}

static int parse_guesses(char *csv, int guesses[], int max_guesses) {
    int count = 0;
    char *token = strtok(csv, ",\r\n");
    while (token && count < max_guesses) {
        guesses[count++] = atoi(token);
        token = strtok(NULL, ",\r\n");
    }
    return count;
}

int history_load(const char *filename, Session sessions[], int max_sessions) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char line[1024];
    int count = 0;
    while (fgets(line, sizeof(line), fp) && count < max_sessions) {
        char *timestamp = strtok(line, ";");
        char *target = strtok(NULL, ";");
        char *attempts = strtok(NULL, ";");
        char *low = strtok(NULL, ";");
        char *high = strtok(NULL, ";");
        char *guesses_csv = strtok(NULL, "\n");

        if (!timestamp || !target || !attempts || !low || !high || !guesses_csv) {
            continue;
        }

        Session s;
        memset(&s, 0, sizeof(Session));
        strncpy(s.timestamp, timestamp, sizeof(s.timestamp) - 1);
        s.target = atoi(target);
        s.attempts = atoi(attempts);
        s.low_count = atoi(low);
        s.high_count = atoi(high);
        s.guesses_count = parse_guesses(guesses_csv, s.guesses, MAX_GUESSES);
        sessions[count++] = s;
    }

    fclose(fp);
    return count;
}

void session_print(const Session *session) {
    printf("%s | alvo=%d | tentativas=%d | baixos=%d | altos=%d | palpites=",
           session->timestamp, session->target, session->attempts,
           session->low_count, session->high_count);
    for (int i = 0; i < session->guesses_count; i++) {
        printf("%d", session->guesses[i]);
        if (i + 1 < session->guesses_count) printf(",");
    }
    printf("\n");
}
