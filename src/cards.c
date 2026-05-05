#include "cards.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void card_init(Card *card, int rank, int suit) {
    card->rank = rank;
    card->suit = suit;
    card->bonus_chips = 0;
    card->bonus_mult = 0;
    card->wild_suit = 0;
    card->seal = SEAL_NONE;
}

void build_standard_deck(Card out[], int *count) {
    int idx = 0;
    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 1; rank <= 13; rank++) {
            card_init(&out[idx], rank, suit);
            idx++;
        }
    }
    if (count) *count = idx;
}

void deck_from_cards(Deck *deck, Card source[], int count) {
    if (count > MAX_DECK_CARDS) count = MAX_DECK_CARDS;
    deck->count = count;
    deck->top = 0;
    for (int i = 0; i < count; i++) {
        deck->cards[i] = &source[i];
    }
}

void deck_shuffle(Deck *deck) {
    for (int i = deck->count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card *tmp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = tmp;
    }
    deck->top = 0;
}

bool deck_has_cards(const Deck *deck) {
    return deck->top < deck->count;
}

Card *deck_draw(Deck *deck) {
    if (!deck_has_cards(deck)) return NULL;
    return deck->cards[deck->top++];
}

int card_scoring_value(const Card *card) {
    if (card->rank == 1) return 14;
    if (card->rank > 10) return 10;
    return card->rank;
}

const char *rank_name(int rank) {
    switch (rank) {
        case 1: return "A";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: break;
    }
    static char buf[3];
    snprintf(buf, sizeof(buf), "%d", rank);
    return buf;
}

const char *suit_name(int suit) {
    switch (suit) {
        case SUIT_COPAS: return "Copas";
        case SUIT_OUROS: return "Ouros";
        case SUIT_ESPADAS: return "Espadas";
        case SUIT_PAUS: return "Paus";
        default: return "?";
    }
}

const char *seal_name(SealType seal) {
    switch (seal) {
        case SEAL_GOLD: return "Selo Ouro";
        case SEAL_RED: return "Selo Vermelho";
        case SEAL_BLUE: return "Selo Azul";
        case SEAL_PURPLE: return "Selo Roxo";
        default: return "Sem selo";
    }
}

void describe_card(const Card *card, char *buffer, int buffer_size) {
    char extra[128] = "";
    if (card->wild_suit) strcat(extra, " | Naipe Coringa");
    if (card->bonus_chips > 0) {
        char tmp[32];
        snprintf(tmp, sizeof(tmp), " | +%d fichas", card->bonus_chips);
        strcat(extra, tmp);
    }
    if (card->bonus_mult > 0) {
        char tmp[32];
        snprintf(tmp, sizeof(tmp), " | +%d mult", card->bonus_mult);
        strcat(extra, tmp);
    }
    if (card->seal != SEAL_NONE) {
        strcat(extra, " | ");
        strcat(extra, seal_name(card->seal));
    }

    snprintf(buffer, (size_t)buffer_size, "%s de %s%s",
             rank_name(card->rank), suit_name(card->suit), extra);
}

void print_card(const Card *card) {
    char buffer[192];
    describe_card(card, buffer, sizeof(buffer));
    printf("%s", buffer);
}

void print_hand(Card *hand[], int hand_count) {
    for (int i = 0; i < hand_count; i++) {
        printf("[%d] ", i + 1);
        print_card(hand[i]);
        printf("\n");
    }
}

int parse_indices(const char *line, int out[], int max_out, int hand_count) {
    char copy[256];
    int count = 0;
    strncpy(copy, line, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';

    char *token = strtok(copy, " ,;\t\n");
    while (token && count < max_out) {
        char *endptr = NULL;
        long value = strtol(token, &endptr, 10);
        if (endptr == token || *endptr != '\0') return -1;
        if (value < 1 || value > hand_count) return -1;

        for (int i = 0; i < count; i++) {
            if (out[i] == (int)value - 1) return -1;
        }
        out[count++] = (int)value - 1;
        token = strtok(NULL, " ,;\t\n");
    }
    return count;
}

static int cmp_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return ia - ib;
}

static bool is_flush(Card *selected[], int count) {
    if (count < 5) return false;

    int reference_suit = -1;
    for (int i = 0; i < count; i++) {
        if (selected[i]->wild_suit) continue;
        if (reference_suit < 0) {
            reference_suit = selected[i]->suit;
            continue;
        }
        if (selected[i]->suit != reference_suit) return false;
    }
    return true;
}

static bool is_straight(Card *selected[], int count) {
    if (count < 5) return false;

    int ranks[MAX_PLAY];
    int original[MAX_PLAY];
    for (int i = 0; i < count; i++) {
        ranks[i] = selected[i]->rank;
        original[i] = selected[i]->rank;
        if (ranks[i] == 1) ranks[i] = 14;
    }
    qsort(ranks, count, sizeof(int), cmp_int);

    for (int i = 1; i < count; i++) {
        if (ranks[i] == ranks[i - 1]) return false;
    }

    bool normal = true;
    for (int i = 1; i < count; i++) {
        if (ranks[i] != ranks[i - 1] + 1) {
            normal = false;
            break;
        }
    }
    if (normal) return true;

    int wheel[5] = {1, 2, 3, 4, 5};
    qsort(original, count, sizeof(int), cmp_int);
    for (int i = 0; i < 5; i++) {
        if (original[i] != wheel[i]) return false;
    }
    return true;
}

HandEval evaluate_cards(Card *selected[], int count) {
    HandEval eval;
    eval.type = HAND_HIGH_CARD;
    strcpy(eval.name, "Carta alta");
    eval.base_score = 10;
    eval.multiplier = 1;
    eval.rank_sum = 0;

    if (count <= 0) return eval;

    int rank_count[14] = {0};
    for (int i = 0; i < count; i++) {
        rank_count[selected[i]->rank]++;
        eval.rank_sum += card_scoring_value(selected[i]);
    }

    int pairs = 0;
    bool three = false;
    bool four = false;
    for (int r = 1; r <= 13; r++) {
        if (rank_count[r] == 2) pairs++;
        if (rank_count[r] == 3) three = true;
        if (rank_count[r] == 4) four = true;
    }

    bool flush = is_flush(selected, count);
    bool straight = is_straight(selected, count);

    if (straight && flush) {
        eval.type = HAND_STRAIGHT_FLUSH;
        strcpy(eval.name, "Straight Flush");
        eval.base_score = 150;
        eval.multiplier = 5;
    } else if (four) {
        eval.type = HAND_FOUR;
        strcpy(eval.name, "Quadra");
        eval.base_score = 120;
        eval.multiplier = 4;
    } else if (three && pairs >= 1) {
        eval.type = HAND_FULL_HOUSE;
        strcpy(eval.name, "Full House");
        eval.base_score = 100;
        eval.multiplier = 4;
    } else if (flush) {
        eval.type = HAND_FLUSH;
        strcpy(eval.name, "Flush");
        eval.base_score = 80;
        eval.multiplier = 3;
    } else if (straight) {
        eval.type = HAND_STRAIGHT;
        strcpy(eval.name, "Sequencia");
        eval.base_score = 70;
        eval.multiplier = 3;
    } else if (three) {
        eval.type = HAND_THREE;
        strcpy(eval.name, "Trinca");
        eval.base_score = 50;
        eval.multiplier = 2;
    } else if (pairs >= 2) {
        eval.type = HAND_TWO_PAIR;
        strcpy(eval.name, "Dois pares");
        eval.base_score = 35;
        eval.multiplier = 2;
    } else if (pairs == 1) {
        eval.type = HAND_PAIR;
        strcpy(eval.name, "Par");
        eval.base_score = 20;
        eval.multiplier = 2;
    }

    return eval;
}
