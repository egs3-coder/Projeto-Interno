#ifndef CARDS_H
#define CARDS_H

#include <stdbool.h>

#define DECK_SIZE 52
#define MAX_DECK_CARDS 60
#define HAND_SIZE 8
#define MAX_PLAY 5

#define SUIT_COPAS 0
#define SUIT_OUROS 1
#define SUIT_ESPADAS 2
#define SUIT_PAUS 3

typedef enum {
    SEAL_NONE = 0,
    SEAL_GOLD,
    SEAL_RED,
    SEAL_BLUE,
    SEAL_PURPLE
} SealType;

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

typedef struct {
    int rank; /* 1=A, 2..10, 11=J, 12=Q, 13=K */
    int suit; /* 0..3 */
    int bonus_chips;
    int bonus_mult;
    int wild_suit;
    SealType seal;
} Card;

typedef struct {
    Card *cards[MAX_DECK_CARDS];
    int count;
    int top;
} Deck;

typedef struct {
    HandType type;
    char name[40];
    int base_score;
    int multiplier;
    int rank_sum;
} HandEval;

void card_init(Card *card, int rank, int suit);
void build_standard_deck(Card out[], int *count);
void deck_from_cards(Deck *deck, Card source[], int count);
void deck_shuffle(Deck *deck);
Card *deck_draw(Deck *deck);
bool deck_has_cards(const Deck *deck);
int card_scoring_value(const Card *card);
const char *rank_name(int rank);
const char *suit_name(int suit);
const char *seal_name(SealType seal);
void describe_card(const Card *card, char *buffer, int buffer_size);
void print_card(const Card *card);
void print_hand(Card *hand[], int hand_count);
int parse_indices(const char *line, int out[], int max_out, int hand_count);
HandEval evaluate_cards(Card *selected[], int count);

#endif
