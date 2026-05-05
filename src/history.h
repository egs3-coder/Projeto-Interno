#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>

#define HISTORY_FILE "history.txt"
#define MAX_SESSIONS 1000
#define MAX_GUESSES 100

typedef struct {
    char timestamp[32];
    int target;
    int attempts;
    int low_count;
    int high_count;
    int guesses[MAX_GUESSES];
    int guesses_count;
} Session;

void make_timestamp(char *buffer, size_t size);
int history_append(const char *filename, const Session *session);
int history_load(const char *filename, Session sessions[], int max_sessions);
void session_print(const Session *session);

#endif
