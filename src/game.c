#include "game.h"
#include "cards.h"
#include "quiz.h"
#include "shop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PHASES 9

typedef struct {
    int phase;
    int goal;
    int score;
    int hands_left;
    int discards_left;
    int boss;
    int debuff_percent;
    int next_play_bonus_chips;
    int next_play_bonus_mult;
    double next_play_xmult;
    int discard_score_bonus;
    int discards_used;
    int six_sense_paid;
    int question_correct;
    int tarot_used_this_phase;
    int first_hand;
    int all_bonus_once;
    int all_mult_once;
    int all_lucky_once;
    int all_glass_once;
    int force_flush_once;
    int blessed_suit;
} PhaseState;

typedef struct {
    HandEval eval;
    int points;
    int coins;
    int lucky_triggered;
    int enhanced_count;
    int hand_level_before;
} PlayOutcome;

typedef struct {
    int chips;
    int mult;
    int threshold_base;
} HandGrowth;

static const HandGrowth HAND_GROWTH[MAX_HAND_LEVEL_TRACK] = {
    {6, 1, 4},   /* high card */
    {8, 1, 4},   /* pair */
    {10, 1, 5},  /* two pair */
    {12, 1, 5},  /* three */
    {14, 1, 6},  /* straight */
    {12, 1, 6},  /* flush */
    {15, 1, 7},  /* full house */
    {18, 1, 8},  /* four */
    {20, 1, 10}  /* straight flush */
};

static int min_int(int a, int b) {
    return a < b ? a : b;
}

static int blind_kind_from_phase(int phase_number) {
    return (phase_number - 1) % 3;
}

static int blind_reward_from_phase(int phase_number) {
    int kind = blind_kind_from_phase(phase_number);
    if (kind == 0) return 3;
    if (kind == 1) return 4;
    return 5;
}

static int hand_level_threshold(const PlayerBuild *build, HandType type) {
    int level = build->hand_levels[type];
    return HAND_GROWTH[type].threshold_base + (level - 1) * 2;
}

static const char *hand_type_name(HandType type) {
    switch (type) {
        case HAND_PAIR: return "Par";
        case HAND_TWO_PAIR: return "Dois pares";
        case HAND_THREE: return "Trinca";
        case HAND_STRAIGHT: return "Sequencia";
        case HAND_FLUSH: return "Flush";
        case HAND_FULL_HOUSE: return "Full House";
        case HAND_FOUR: return "Quadra";
        case HAND_STRAIGHT_FLUSH: return "Straight Flush";
        default: return "Carta alta";
    }
}

static void apply_hand_level_bonus(const PlayerBuild *build, HandEval *eval) {
    int level = build->hand_levels[eval->type];
    int extra_levels = level - 1;
    if (extra_levels <= 0) return;
    eval->base_score += HAND_GROWTH[eval->type].chips * extra_levels;
    eval->multiplier += HAND_GROWTH[eval->type].mult * extra_levels;
}

static void maybe_level_up_hand(PlayerBuild *build, HandType type) {
    build->hand_progress[type]++;
    while (build->hand_progress[type] >= hand_level_threshold(build, type)) {
        build->hand_progress[type] -= hand_level_threshold(build, type);
        build->hand_levels[type]++;
        printf("Evolucao da mao: %s agora esta no nivel %d.\n",
               hand_type_name(type), build->hand_levels[type]);
    }
}

static int read_choice(void) {
    char line[64];
    printf("Opcao: ");
    if (!fgets(line, sizeof(line), stdin)) return 0;
    return atoi(line);
}

static int read_selection(int indices[], int max, int hand_count) {
    char line[256];
    printf("Digite os indices das cartas separados por espaco (max %d): ", max);
    if (!fgets(line, sizeof(line), stdin)) return -1;
    int count = parse_indices(line, indices, max, hand_count);
    if (count <= 0) {
        printf("Selecao invalida.\n");
        return -1;
    }
    return count;
}

static void remove_and_draw(Card hand[], int *hand_count, Deck *deck, int indices[], int count) {
    int selected[HAND_SIZE] = {0};
    for (int i = 0; i < count; i++) {
        if (indices[i] >= 0 && indices[i] < *hand_count) selected[indices[i]] = 1;
    }

    Card new_hand[HAND_SIZE];
    int new_count = 0;
    for (int i = 0; i < *hand_count; i++) {
        if (!selected[i]) new_hand[new_count++] = hand[i];
    }
    while (new_count < HAND_SIZE && deck_has_cards(deck)) {
        new_hand[new_count++] = deck_draw(deck);
    }
    for (int i = 0; i < new_count; i++) hand[i] = new_hand[i];
    *hand_count = new_count;
}

static int has_joker(const PlayerBuild *build, JokerType type) {
    for (int i = 0; i < build->joker_count; i++) {
        if (build->jokers[i].active && build->jokers[i].type == type) return 1;
    }
    return 0;
}

static int count_joker_editions(const PlayerBuild *build) {
    int count = 0;
    for (int i = 0; i < build->joker_count; i++) {
        if (build->jokers[i].active && build->jokers[i].edition != EDITION_NONE) count++;
    }
    return count;
}

static int count_matching_suit(Card selected[], int count, int suit) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (selected[i].suit == suit) total++;
    }
    return total;
}

static int count_royals(Card selected[], int count) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (selected[i].rank >= 11 || selected[i].rank == 1) total++;
    }
    return total;
}

static int count_lows(Card selected[], int count) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (selected[i].rank >= 2 && selected[i].rank <= 4) total++;
    }
    return total;
}

static int count_specific_rank(Card selected[], int count, int rank) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (selected[i].rank == rank) total++;
    }
    return total;
}

static void fill_initial_hand(Deck *deck, Card hand[], int *hand_count) {
    *hand_count = 0;
    while (*hand_count < HAND_SIZE && deck_has_cards(deck)) {
        hand[(*hand_count)++] = deck_draw(deck);
    }
}

static void print_score_table(void) {
    printf("\nTabela base de pontuacao:\n");
    printf("Carta alta 10x1 | Par 20x2 | Dois pares 35x2 | Trinca 50x2\n");
    printf("Sequencia 70x3 | Flush 80x3 | Full House 100x4 | Quadra 120x4 | Straight Flush 150x5\n");
    printf("Melhorias de carta: Bonus +20 chips | Multi +4 mult | Sorte 25%% de +20 mult | Vidro x2 mult\n");
}

static void print_hand_levels(const PlayerBuild *build) {
    printf("Niveis das maos: Alta %d | Par %d | 2P %d | Trinca %d | Seq %d | Flush %d | FH %d | Quadra %d | SF %d\n",
           build->hand_levels[HAND_HIGH_CARD], build->hand_levels[HAND_PAIR], build->hand_levels[HAND_TWO_PAIR],
           build->hand_levels[HAND_THREE], build->hand_levels[HAND_STRAIGHT], build->hand_levels[HAND_FLUSH],
           build->hand_levels[HAND_FULL_HOUSE], build->hand_levels[HAND_FOUR], build->hand_levels[HAND_STRAIGHT_FLUSH]);
}

static void apply_hand_overrides(PhaseState *phase, HandEval *eval, int count) {
    if (count < 5) return;
    if (phase->force_flush_once) {
        if (eval->type < HAND_FLUSH) {
            eval->type = HAND_FLUSH;
            strcpy(eval->name, "Flush Arcano");
            eval->base_score = 80;
            eval->multiplier = 3;
        }
        phase->force_flush_once = 0;
    }
}

static void update_temporary_jokers_after_play(PlayerBuild *build) {
    for (int i = 0; i < build->joker_count; i++) {
        JokerInstance *joker = &build->jokers[i];
        if (!joker->active) continue;
        switch (joker->type) {
            case JOKER_POPCORN:
                joker->value -= 4;
                if (joker->value <= 0) joker->active = 0;
                break;
            case JOKER_ICE_CREAM:
                joker->value -= 5;
                if (joker->value <= 0) joker->active = 0;
                break;
            case JOKER_LUNCHBOX:
                joker->counter--;
                if (joker->counter <= 0) joker->active = 0;
                break;
            default:
                break;
        }
    }
    compact_jokers(build);
}

static void update_temporary_jokers_after_phase(PlayerBuild *build) {
    for (int i = 0; i < build->joker_count; i++) {
        JokerInstance *joker = &build->jokers[i];
        if (!joker->active) continue;
        switch (joker->type) {
            case JOKER_CAVENDISH:
                if ((rand() % 8) == 0) joker->active = 0;
                break;
            case JOKER_COFFEE:
                joker->counter--;
                if (joker->counter <= 0) joker->active = 0;
                break;
            default:
                break;
        }
    }
    compact_jokers(build);
}

static void grant_oracle_coupon_tarot(PlayerBuild *build, int phase_number) {
    if (build->coupon_levels[2] >= 2) {
        if (build_add_tarot(build, random_tarot())) {
            printf("Tarot Tycoon gerou 1 Tarot gratis nesta fase.\n");
        }
    } else if (build->coupon_levels[2] == 1 && phase_number % 2 == 0) {
        if (build_add_tarot(build, random_tarot())) {
            printf("Tarot Merchant gerou 1 Tarot gratis nesta fase.\n");
        }
    }
}

static void use_tarot(PlayerBuild *build, PhaseState *phase) {
    if (build->tarot_count <= 0) {
        printf("Voce nao possui Tarot.\n");
        return;
    }
    printf("\nEscolha um Tarot para usar:\n");
    for (int i = 0; i < build->tarot_count; i++) {
        printf("%d) %s\n", i + 1, tarot_name(build->tarot_inventory[i]));
    }
    printf("0) Cancelar\n");
    int choice = read_choice();
    if (choice <= 0 || choice > build->tarot_count) return;

    int idx = choice - 1;
    TarotType tarot = build->tarot_inventory[idx];
    switch (tarot) {
        case TAROT_FOOL:
            if (build->last_used_tarot != TAROT_NONE && build_add_tarot(build, build->last_used_tarot)) {
                printf("O Louco copiou %s.\n", tarot_name(build->last_used_tarot));
            } else {
                printf("O Louco nao encontrou Tarot valido para copiar.\n");
            }
            break;
        case TAROT_MAGICIAN:
            phase->all_lucky_once = 1;
            printf("O Mago: a proxima mao recebe Sorte em todas as cartas.\n");
            break;
        case TAROT_HIGH_PRIESTESS: {
            HandType first = (HandType)(rand() % MAX_HAND_LEVEL_TRACK);
            HandType second = (HandType)(rand() % MAX_HAND_LEVEL_TRACK);
            maybe_level_up_hand(build, first);
            maybe_level_up_hand(build, second);
            printf("A Sacerdotisa acelerou o estudo de duas maos de poker.\n");
            break;
        }
        case TAROT_EMPEROR:
            build_add_tarot(build, random_tarot());
            build_add_tarot(build, random_tarot());
            printf("O Imperador gerou ate 2 Tarots.\n");
            break;
        case TAROT_EMPRESS:
            phase->all_mult_once = 1;
            printf("A Imperatriz: a proxima mao recebe Multi em todas as cartas.\n");
            break;
        case TAROT_HIEROPHANT:
            phase->all_bonus_once = 1;
            printf("O Hierofante: a proxima mao recebe Bonus em todas as cartas.\n");
            break;
        case TAROT_LOVERS:
            phase->force_flush_once = 1;
            printf("Os Enamorados: a proxima mao de 5 cartas conta como Flush.\n");
            break;
        case TAROT_CHARIOT:
            phase->next_play_bonus_chips += 120;
            printf("O Carro: +120 chips na proxima jogada.\n");
            break;
        case TAROT_JUSTICE:
            phase->next_play_xmult *= 2.0;
            printf("A Justica: x2 mult na proxima jogada.\n");
            break;
        case TAROT_HERMIT: {
            int gain = min_int(build->coins, 20);
            build->coins += gain;
            printf("O Eremita: +%d moedas.\n", gain);
            break;
        }
        case TAROT_WHEEL:
            if ((rand() % 4) == 0) {
                apply_random_free_edition(build);
                printf("Roda da Fortuna ativou uma marca gratis.\n");
            } else {
                printf("Roda da Fortuna nao ativou nada desta vez.\n");
            }
            break;
        case TAROT_STRENGTH:
            phase->next_play_bonus_chips += 40;
            phase->next_play_bonus_mult += 4;
            printf("A Forca: +40 chips e +4 mult na proxima jogada.\n");
            break;
        case TAROT_TEMPERANCE: {
            int gain = 0;
            for (int i = 0; i < build->joker_count; i++) gain += build->jokers[i].sell_value;
            gain = min_int(gain, 50);
            build->coins += gain;
            printf("Temperanca: +%d moedas.\n", gain);
            break;
        }
        case TAROT_DEVIL:
            phase->all_glass_once = 1;
            printf("O Diabo: a proxima mao recebe Vidro em todas as cartas.\n");
            break;
        case TAROT_TOWER:
            phase->next_play_bonus_chips += 150;
            printf("A Torre: +150 chips na proxima jogada.\n");
            break;
        case TAROT_STAR:
            phase->blessed_suit = SUIT_ESPADAS;
            printf("A Estrela: cartas de Espadas recebem bonus na proxima mao.\n");
            break;
        case TAROT_MOON:
            phase->blessed_suit = SUIT_COPAS;
            printf("A Lua: cartas de Copas recebem bonus na proxima mao.\n");
            break;
        case TAROT_SUN:
            phase->blessed_suit = SUIT_OUROS;
            printf("O Sol: cartas de Ouros recebem bonus na proxima mao.\n");
            break;
        case TAROT_WORLD:
            phase->blessed_suit = SUIT_PAUS;
            printf("O Mundo: cartas de Paus recebem bonus na proxima mao.\n");
            break;
        case TAROT_JUDGEMENT:
            if (build_add_joker(build, random_joker())) {
                printf("Julgamento criou um novo Coringa.\n");
            } else {
                printf("Julgamento nao conseguiu criar Coringa por falta de espaco.\n");
            }
            break;
        case TAROT_AURA:
            apply_random_free_edition(build);
            printf("Aura concedeu uma edicao aleatoria a um Coringa.\n");
            break;
        case TAROT_WRAITH: {
            JokerType rare_pool[] = {JOKER_FOUR, JOKER_BOSS_SHIELD, JOKER_CRYPTID_RELAY, JOKER_COSMOS_PRISM, JOKER_LUCKY_JIMBO, JOKER_NOBLE_LINEAGE};
            JokerType rare = rare_pool[rand() % (int)(sizeof(rare_pool) / sizeof(rare_pool[0]))];
            int tax = min_int(build->coins, 6);
            build->coins -= tax;
            if (build_add_joker(build, rare)) printf("Wraith invocou %s e cobrou %d moedas.\n", joker_name(rare), tax);
            else printf("Wraith cobrou %d moedas, mas faltou espaco para o Coringa raro.\n", tax);
            break;
        }
        case TAROT_ANKH:
            if (build->joker_count <= 0) {
                printf("Ankh precisa de pelo menos um Coringa em jogo.\n");
            } else if (build_add_joker(build, build->jokers[0].type)) {
                build->jokers[build->joker_count - 1] = build->jokers[0];
                if (build->joker_count >= 3) build->jokers[1].active = 0;
                compact_jokers(build);
                printf("Ankh duplicou %s e cobrou um sacrificio.\n", joker_name(build->jokers[0].type));
            } else {
                printf("Ankh nao encontrou espaco para duplicar o Coringa.\n");
            }
            break;
        case TAROT_SOUL: {
            JokerType legends[] = {JOKER_NOBLE_LINEAGE, JOKER_BOSS_SHIELD, JOKER_CRYPTID_RELAY, JOKER_COSMOS_PRISM};
            JokerType legend = legends[rand() % (int)(sizeof(legends) / sizeof(legends[0]))];
            if (build_add_joker(build, legend)) printf("The Soul trouxe %s para a run.\n", joker_name(legend));
            else printf("The Soul encontrou um Coringa lendario, mas faltou espaco.\n");
            break;
        }
        default:
            break;
    }

    phase->tarot_used_this_phase = 1;
    build->last_used_tarot = tarot;
    for (int i = idx; i < build->tarot_count - 1; i++) {
        build->tarot_inventory[i] = build->tarot_inventory[i + 1];
    }
    build->tarot_count--;
}

static void apply_discard_effects(PlayerBuild *build, PhaseState *phase, Card discarded[], int count) {
    int low_cards = count_lows(discarded, count);
    int royal_cards = count_royals(discarded, count);
    int same_suit = 0;

    for (int suit = 0; suit < 4; suit++) {
        int match = count_matching_suit(discarded, count, suit);
        if (match > same_suit) same_suit = match;
    }

    if (has_joker(build, JOKER_COIN_LOW) && low_cards > 0) {
        build->coins += 1;
        printf("Troco Miudo: +1 moeda por descarte com carta baixa.\n");
    }
    if (has_joker(build, JOKER_COIN_ROYAL) && royal_cards > 0) {
        build->coins += royal_cards;
        printf("Imposto da Realeza: +%d moeda(s).\n", royal_cards);
    }
    if (same_suit >= 3) {
        build->coins += 2;
        printf("Descarte alinhado: +2 moedas.\n");
    }

    phase->discards_used += count;
    if (!phase->six_sense_paid && has_joker(build, JOKER_SIXTH_SENSE) && phase->discards_used >= 6) {
        if (build_add_tarot(build, random_tarot())) {
            phase->six_sense_paid = 1;
            printf("Sexto Senso criou 1 Tarot ao completar 6 descartes.\n");
        }
    }
}

static PlayOutcome calculate_play_outcome(PlayerBuild *build, PhaseState *phase, Card selected[], int count) {
    PlayOutcome outcome;
    outcome.eval = evaluate_cards(selected, count);
    outcome.points = 0;
    outcome.coins = 0;
    outcome.lucky_triggered = 0;
    outcome.enhanced_count = 0;

    apply_hand_overrides(phase, &outcome.eval, count);
    outcome.hand_level_before = build->hand_levels[outcome.eval.type];
    apply_hand_level_bonus(build, &outcome.eval);

    double chips = outcome.eval.base_score + outcome.eval.rank_sum + phase->next_play_bonus_chips + phase->discard_score_bonus;
    double mult = outcome.eval.multiplier + phase->next_play_bonus_mult;
    double xmult = phase->next_play_xmult > 0.0 ? phase->next_play_xmult : 1.0;

    int royal_count = count_royals(selected, count);
    int kings = count_specific_rank(selected, count, 13);
    int queens = count_specific_rank(selected, count, 12);
    int jacks = count_specific_rank(selected, count, 11);
    int low_count = count_lows(selected, count);

    for (int i = 0; i < count; i++) {
        CardEnhancement enhancement = selected[i].enhancement;
        if (phase->all_bonus_once) enhancement = ENHANCEMENT_BONUS;
        if (phase->all_mult_once) enhancement = ENHANCEMENT_MULT;
        if (phase->all_lucky_once) enhancement = ENHANCEMENT_LUCKY;
        if (phase->all_glass_once) enhancement = ENHANCEMENT_GLASS;

        if (enhancement != ENHANCEMENT_NONE) outcome.enhanced_count++;
        switch (enhancement) {
            case ENHANCEMENT_BONUS:
                chips += 20;
                break;
            case ENHANCEMENT_MULT:
                mult += 4;
                break;
            case ENHANCEMENT_LUCKY:
                if ((rand() % 4) == 0) {
                    mult += 20;
                    outcome.lucky_triggered = 1;
                }
                break;
            case ENHANCEMENT_GLASS:
                xmult *= 2.0;
                break;
            default:
                break;
        }

        if (phase->blessed_suit >= 0 && selected[i].suit == phase->blessed_suit) {
            chips += 15;
            mult += 2;
        }
    }

    for (int i = 0; i < build->joker_count; i++) {
        JokerInstance *joker = &build->jokers[i];
        if (!joker->active) continue;

        switch (joker->type) {
            case JOKER_FLAT:
                chips += 30;
                break;
            case JOKER_PAIR:
                if (outcome.eval.type == HAND_PAIR) chips += 20;
                break;
            case JOKER_TWO_PAIR:
                if (outcome.eval.type == HAND_TWO_PAIR) mult += 10;
                break;
            case JOKER_THREE:
                if (outcome.eval.type == HAND_THREE) chips += 35;
                break;
            case JOKER_FOUR:
                if (outcome.eval.type == HAND_FOUR) xmult *= 2.0;
                break;
            case JOKER_STRAIGHT:
                if (outcome.eval.type == HAND_STRAIGHT || outcome.eval.type == HAND_STRAIGHT_FLUSH) mult += 16;
                break;
            case JOKER_FLUSH:
                if (outcome.eval.type == HAND_FLUSH || outcome.eval.type == HAND_STRAIGHT_FLUSH) chips += 45;
                break;
            case JOKER_COIN_PAIR:
                if (outcome.eval.type == HAND_PAIR) outcome.coins += 2;
                break;
            case JOKER_COIN_FLUSH:
                if (outcome.eval.type == HAND_FLUSH || outcome.eval.type == HAND_STRAIGHT_FLUSH) outcome.coins += 3;
                break;
            case JOKER_COIN_THREE:
                if (outcome.eval.type == HAND_THREE) outcome.coins += 3;
                break;
            case JOKER_COIN_ROYAL:
                if (royal_count > 0) outcome.coins += 1;
                break;
            case JOKER_COIN_LOW:
                if (low_count > 0) outcome.coins += 1;
                break;
            case JOKER_POPCORN:
                mult += joker->value;
                break;
            case JOKER_ICE_CREAM:
                chips += joker->value;
                break;
            case JOKER_CAVENDISH:
                mult += joker->value;
                break;
            case JOKER_COFFEE:
                outcome.coins += 2;
                break;
            case JOKER_LUNCHBOX:
                if (phase->first_hand) chips += joker->value;
                break;
            case JOKER_MARKER:
                if (phase->question_correct) mult += 12;
                break;
            case JOKER_ROYAL_KING:
                chips += kings * 20;
                break;
            case JOKER_ROYAL_QUEEN:
                mult += queens * 6;
                break;
            case JOKER_ROYAL_JACK:
                if (jacks > 0 && (rand() % 3) == 0) outcome.coins += 2;
                break;
            case JOKER_ROYAL_COUNCIL:
                if (royal_count >= 2) xmult *= 1.5;
                break;
            case JOKER_THRONE:
                if (kings > 0 && queens > 0) chips += 40;
                break;
            case JOKER_NOBLE_LINEAGE:
                if (royal_count == count && count > 0) xmult *= 2.0;
                break;
            case JOKER_ARCANE_MINOR:
                mult += build->tarot_count * 3;
                break;
            case JOKER_OCCULT_LIBRARY:
                mult += build->tarot_count * 5;
                break;
            case JOKER_RITUAL_TABLE:
                if (phase->tarot_used_this_phase) chips += 50;
                break;
            case JOKER_ECHO_ARCANO:
                if (build->last_used_tarot != TAROT_NONE) xmult *= 1.2;
                break;
            case JOKER_STENCIL: {
                int empty_slots = build->joker_capacity - build->joker_count;
                xmult *= 1.0 + empty_slots * 0.35;
                break;
            }
            case JOKER_GREEDY:
                mult += count_matching_suit(selected, count, SUIT_OUROS) * 4;
                break;
            case JOKER_LOVELY:
                mult += count_matching_suit(selected, count, SUIT_COPAS) * 4;
                break;
            case JOKER_WRATHFUL:
                chips += count_matching_suit(selected, count, SUIT_ESPADAS) * 12;
                break;
            case JOKER_ASTUTE:
                chips += count_matching_suit(selected, count, SUIT_PAUS) * 12;
                break;
            case JOKER_MIRROR_QUIZ:
                if (phase->question_correct) xmult *= 2.0;
                break;
            case JOKER_BOSS_SHIELD:
                if (phase->boss) xmult *= 1.5;
                break;
            case JOKER_CRYPTID_RELAY:
                if (count_joker_editions(build) >= 2) xmult *= 1.4;
                break;
            case JOKER_COSMOS_PRISM:
                if (outcome.enhanced_count >= 2) xmult *= 1.6;
                break;
            case JOKER_LUCKY_JIMBO:
                if (outcome.lucky_triggered) xmult *= 1.5;
                break;
            case JOKER_FAMILIAR_WAGE:
                if (outcome.eval.type == HAND_PAIR || outcome.eval.type == HAND_TWO_PAIR || outcome.eval.type == HAND_THREE) {
                    outcome.coins += 2;
                }
                break;
            case JOKER_PI_CACHE:
                if (phase->tarot_used_this_phase) mult += 12;
                break;
            case JOKER_COIN_BOSS:
            case JOKER_SIXTH_SENSE:
            default:
                break;
        }

        switch (joker->edition) {
            case EDITION_FOIL:
                chips += 50;
                break;
            case EDITION_CHROME:
                mult += 10;
                break;
            case EDITION_PRISMATIC:
                xmult *= 1.5;
                break;
            default:
                break;
        }
    }

    if (phase->debuff_percent > 0) {
        xmult *= (100.0 - phase->debuff_percent) / 100.0;
    }

    phase->next_play_bonus_chips = 0;
    phase->next_play_bonus_mult = 0;
    phase->next_play_xmult = 1.0;
    phase->discard_score_bonus = 0;
    phase->all_bonus_once = 0;
    phase->all_mult_once = 0;
    phase->all_lucky_once = 0;
    phase->all_glass_once = 0;
    phase->blessed_suit = -1;

    double total = chips * mult * xmult;
    if (total < 0) total = 0;
    outcome.points = (int)(total + 0.5);
    return outcome;
}

static int play_phase(PlayerBuild *build, int phase_number) {
    Deck deck;
    Card hand[HAND_SIZE];
    int hand_count = 0;
    PhaseState phase;

    deck_init(&deck);
    deck_shuffle(&deck);
    fill_initial_hand(&deck, hand, &hand_count);

    phase.phase = phase_number;
    phase.boss = (phase_number % 3 == 0);
    {
        int ante_index = (phase_number - 1) / 3;
        int base_goal = 300 + ante_index * 250;
        int blind_kind = blind_kind_from_phase(phase_number);
        if (blind_kind == 0) phase.goal = base_goal;
        else if (blind_kind == 1) phase.goal = (base_goal * 3) / 2;
        else phase.goal = base_goal * 2;
    }
    phase.score = 0;
    phase.hands_left = 4;
    if (build->coupon_levels[1] == 1) phase.hands_left += 1;
    if (build->coupon_levels[1] == 2) phase.hands_left += 2;
    phase.discards_left = 2;
    phase.debuff_percent = 0;
    phase.next_play_bonus_chips = 0;
    phase.next_play_bonus_mult = 0;
    phase.next_play_xmult = 1.0;
    phase.discard_score_bonus = 0;
    phase.discards_used = 0;
    phase.six_sense_paid = 0;
    phase.question_correct = 0;
    phase.tarot_used_this_phase = 0;
    phase.first_hand = 1;
    phase.all_bonus_once = 0;
    phase.all_mult_once = 0;
    phase.all_lucky_once = 0;
    phase.all_glass_once = 0;
    phase.force_flush_once = 0;
    phase.blessed_suit = -1;

    grant_oracle_coupon_tarot(build, phase_number);

    printf("\n========== FASE %d %s ==========\n", phase.phase, phase.boss ? "BOSS" : "");
    printf("Meta da fase: %d pontos\n", phase.goal);

    if (phase.boss) {
        QuizResult boss_quiz = ask_quiz(1);
        build->coins += boss_quiz.coins;
        if (boss_quiz.correct) {
            phase.question_correct = 1;
            build->quiz_hits++;
            build->quiz_streak++;
            apply_random_free_edition(build);
            printf("Pergunta do boss acertada. O efeito do blind chefe foi neutralizado.\n");
        } else if (boss_quiz.apply_debuff) {
            build->quiz_streak = 0;
            int debuff = rand() % 3;
            if (debuff == 0) {
                phase.debuff_percent = has_joker(build, JOKER_BOSS_SHIELD) ? 10 : 20;
                printf("Boss: pontuacao reduzida em %d%% durante a fase.\n", phase.debuff_percent);
            } else if (debuff == 1 && phase.hands_left > 1) {
                phase.hands_left -= has_joker(build, JOKER_BOSS_SHIELD) ? 0 : 1;
                printf("Boss: pressao na fase aplicada.\n");
            } else if (phase.discards_left > 0) {
                phase.discards_left -= has_joker(build, JOKER_BOSS_SHIELD) ? 0 : 1;
                printf("Boss: descarte reduzido.\n");
            }
        }
    }

    while (phase.hands_left > 0 && phase.score < phase.goal && hand_count > 0) {
        printf("\n--- Fase %d | Pontos %d/%d | Maos %d | Descartes %d | Deck %d cartas ---\n",
               phase.phase, phase.score, phase.goal, phase.hands_left, phase.discards_left,
               DECK_SIZE - deck.top);
        print_build(build);
        print_score_table();
        print_hand_levels(build);
        printf("\nSua mao:\n");
        print_hand(hand, hand_count);

        printf("\n1) Jogar cartas\n");
        printf("2) Descartar cartas\n");
        printf("3) Usar Tarot\n");
        printf("0) Encerrar run\n");
        int choice = read_choice();

        if (choice == 0) return 0;
        if (choice == 3) {
            use_tarot(build, &phase);
            continue;
        }

        int indices[MAX_PLAY];
        int count = read_selection(indices, MAX_PLAY, hand_count);
        if (count <= 0) continue;

        if (choice == 1) {
            Card selected[MAX_PLAY];
            for (int i = 0; i < count; i++) selected[i] = hand[indices[i]];
            PlayOutcome outcome = calculate_play_outcome(build, &phase, selected, count);
            phase.score += outcome.points;
            phase.hands_left--;
            build->coins += outcome.coins;
            maybe_level_up_hand(build, outcome.eval.type);
            printf("Jogada: %s | base=%d | mult=%d | soma_rank=%d | pontos=%d",
                   outcome.eval.name, outcome.eval.base_score, outcome.eval.multiplier,
                   outcome.eval.rank_sum, outcome.points);
            if (outcome.coins > 0) printf(" | moedas +%d", outcome.coins);
            printf("\n");
            remove_and_draw(hand, &hand_count, &deck, indices, count);
            update_temporary_jokers_after_play(build);
            phase.first_hand = 0;
        } else if (choice == 2) {
            if (phase.discards_left <= 0) {
                printf("Sem descartes disponiveis.\n");
                continue;
            }
            Card discarded[MAX_PLAY];
            for (int i = 0; i < count; i++) discarded[i] = hand[indices[i]];
            phase.discards_left--;
            apply_discard_effects(build, &phase, discarded, count);
            remove_and_draw(hand, &hand_count, &deck, indices, count);
            printf("Cartas descartadas e novas cartas compradas.\n");
        } else {
            printf("Opcao invalida.\n");
        }
    }

    if (phase.score >= phase.goal) {
        int reward = blind_reward_from_phase(phase_number);
        if (has_joker(build, JOKER_COIN_BOSS) && phase.boss) reward += 5;
        build->coins += reward;
        printf("\nFase vencida! Pontos: %d/%d | Recompensa: +%d moedas.\n", phase.score, phase.goal, reward);
        update_temporary_jokers_after_phase(build);
        return 1;
    }

    printf("\nRun encerrada. Pontos: %d/%d.\n", phase.score, phase.goal);
    return 0;
}

static void post_phase_quiz_and_rewards(PlayerBuild *build, int phase_number) {
    if (phase_number % 3 == 0) {
        printf("\nBoss vencido. O fluxo volta ao normal.\n");
    } else {
        QuizResult quiz = ask_quiz(0);
        build->coins += quiz.coins;
        if (quiz.correct) {
            build->quiz_hits++;
            build->quiz_streak++;
            apply_random_free_edition(build);
            if ((build->quiz_hits % 2) == 0) {
                build->coupon_tokens++;
                printf("Cupom liberado pelas perguntas! Agora voce pode comprar um voucher por 10$ na loja.\n");
            }
            if (has_joker(build, JOKER_ARCANE_MINOR) && build_add_tarot(build, random_tarot())) {
                printf("Arcano Menor gerou 1 Tarot por acertar a pergunta.\n");
            }
            if (has_joker(build, JOKER_MARKER)) {
                build->coins += 2;
                printf("Marca-Texto concedeu +2 moedas pela resposta correta.\n");
            }
        } else {
            build->quiz_streak = 0;
            if (has_joker(build, JOKER_MARKER)) {
                build->coins += 5;
                printf("Marca-Texto suavizou o erro com +5 moedas.\n");
            }
        }
    }
}

void play_card_run(void) {
    PlayerBuild build;
    build_init(&build);

    printf("\n========== RUN DE CARTAS + QUIZ ==========\n");
    printf("Esta versao MVP inclui perguntas, Coringas com editions, Cupons liberados pelo quiz,\n");
    printf("Tarots inspirados no original e cartas especiais Bonus, Multi, Sorte e Vidro.\n");

    int survived = 1;
    for (int phase = 1; phase <= MAX_PHASES; phase++) {
        survived = play_phase(&build, phase);
        if (!survived) break;
        post_phase_quiz_and_rewards(&build, phase);
        run_shop(&build);
    }

    if (survived) {
        printf("\nParabens! Voce concluiu o MVP com %d moedas restantes.\n", build.coins);
    } else {
        printf("\nFim da run. Resultado final:\n");
        print_build(&build);
    }
}
