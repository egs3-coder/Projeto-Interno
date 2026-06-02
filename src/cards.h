#ifndef CARDS_H
#define CARDS_H

#include <stdbool.h>

#define DECK_SIZE 52
#define HAND_SIZE 8
#define MAX_PLAY 5

#define SUIT_COPAS 0
#define SUIT_OUROS 1
#define SUIT_ESPADAS 2
#define SUIT_PAUS 3

typedef enum {
    HAND_HIGH_CARD = 0,
    HAND_PAIR,
    HAND_TWO_PAIR,
    HAND_THREE,
    HAND_STRAIGHT,
    HAND_FLUSH,
    HAND_FULL_HOUSE,
    HAND_FOUR,
    HAND_STRAIGHT_FLUSH
} HandType;

typedef enum {
    ENHANCEMENT_NONE = 0,
    ENHANCEMENT_BONUS,
    ENHANCEMENT_MULT,
    ENHANCEMENT_LUCKY,
    ENHANCEMENT_GLASS,
    ENHANCEMENT_WILD
} CardEnhancement;

typedef struct {
    int rank;
    int suit;
    CardEnhancement enhancement;
} Card;

typedef struct {
    Card cards[DECK_SIZE];
    int top;
} Deck;

typedef struct {
    HandType type;
    char name[40];
    int base_score;
    int multiplier;
    int rank_sum;
} HandEval;

void deck_init(Deck *deck);
void deck_shuffle(Deck *deck);
Card deck_draw(Deck *deck);
bool deck_has_cards(const Deck *deck);
const char *rank_name(int rank);
const char *suit_name(int suit);
const char *enhancement_name(CardEnhancement enhancement);
void print_card(Card card);
void print_hand(Card hand[], int hand_count);
int parse_indices(const char *line, int out[], int max_out, int hand_count);
HandEval evaluate_cards(Card selected[], int count);

#endif
