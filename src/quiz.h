#ifndef QUIZ_H
#define QUIZ_H

typedef struct {
    int correct;
    int coins;
    int apply_debuff;
} QuizResult;

QuizResult ask_quiz(int hard_mode);

#endif
