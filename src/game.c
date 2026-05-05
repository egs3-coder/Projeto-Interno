#include "game.h"
#include "cards.h"
#include "quiz.h"
#include "shop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PHASES 24

typedef enum {
    BLIND_SMALL = 0,
    BLIND_BIG,
    BLIND_BOSS
} BlindType;

typedef struct {
    int phase_number;
    int ante;
    BlindType blind_type;
    char blind_name[24];
    int goal;
    int score;
    int hands_left;
    int discards_left;
    int debuff_percent;
    int next_play_bonus;
    int blind_reward;
} PhaseState;

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

static void wait_ms(int milliseconds) {
    clock_t start = clock();
    while (((clock() - start) * 1000 / CLOCKS_PER_SEC) < milliseconds) {
    }
}

static const char *blind_name(BlindType type) {
    switch (type) {
        case BLIND_BIG: return "Big Blind";
        case BLIND_BOSS: return "Blind Chefe";
        default: return "Small Blind";
    }
}

static int blind_reward_value(BlindType type) {
    switch (type) {
        case BLIND_BIG: return 4;
        case BLIND_BOSS: return 5;
        default: return 3;
    }
}

static int blind_goal_value(int ante, BlindType type) {
    int base = 320 + (ante - 1) * 210;
    if (type == BLIND_BIG) return base + 190;
    if (type == BLIND_BOSS) return base + 420;
    return base;
}

static void setup_phase(PhaseState *phase, int phase_number) {
    phase->phase_number = phase_number;
    phase->ante = ((phase_number - 1) / 3) + 1;
    phase->blind_type = (BlindType)((phase_number - 1) % 3);
    snprintf(phase->blind_name, sizeof(phase->blind_name), "%s", blind_name(phase->blind_type));
    phase->goal = blind_goal_value(phase->ante, phase->blind_type);
    phase->score = 0;
    phase->hands_left = (phase->blind_type == BLIND_BOSS) ? 4 : 5;
    phase->discards_left = (phase->blind_type == BLIND_BOSS) ? 2 : 3;
    phase->debuff_percent = 0;
    phase->next_play_bonus = 0;
    phase->blind_reward = blind_reward_value(phase->blind_type);
}

static void remove_and_draw(Card *hand[], int *hand_count, Deck *deck, int indices[], int count) {
    int selected[HAND_SIZE] = {0};
    for (int i = 0; i < count; i++) {
        if (indices[i] >= 0 && indices[i] < *hand_count) selected[indices[i]] = 1;
    }

    Card *new_hand[HAND_SIZE];
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

static int count_jokers(const PlayerBuild *build, JokerType type) {
    int total = 0;
    for (int i = 0; i < build->joker_count; i++) {
        if (build->jokers[i].type == type) total++;
    }
    return total;
}

static void fill_initial_hand(Deck *deck, Card *hand[], int *hand_count) {
    *hand_count = 0;
    while (*hand_count < HAND_SIZE && deck_has_cards(deck)) {
        hand[(*hand_count)++] = deck_draw(deck);
    }
}

static void print_score_table(void) {
    printf("\nTabela base de pontuacao:\n");
    printf("Carta alta 10x1 | Par 20x2 | Dois pares 35x2 | Trinca 50x2\n");
    printf("Sequencia 70x3 | Flush 80x3 | Full House 100x4 | Quadra 120x4 | Straight Flush 150x5\n");
}

static int face_or_ace(const Card *card) {
    return card->rank == 1 || card->rank >= 11;
}

static int calculate_play_score(const PlayerBuild *build, const PhaseState *phase, Card *selected[], int count,
                                const HandEval *eval, int *gold_income) {
    int base = eval->base_score + eval->rank_sum;
    int mult = eval->multiplier;
    int flat_bonus = phase->next_play_bonus;
    double total_mult = 1.0;
    int wild_cards = 0;
    int royal_cards = 0;

    for (int i = 0; i < count; i++) {
        base += selected[i]->bonus_chips;
        mult += selected[i]->bonus_mult;
        if (selected[i]->wild_suit) wild_cards++;
        if (face_or_ace(selected[i])) royal_cards++;
        if (selected[i]->seal == SEAL_GOLD) *gold_income += 3;
        if (selected[i]->seal == SEAL_RED) {
            base += card_scoring_value(selected[i]) + selected[i]->bonus_chips;
        }
    }

    for (int i = 0; i < build->joker_count; i++) {
        const OwnedJoker *joker = &build->jokers[i];
        switch (joker->type) {
            case JOKER_FLAT:
                flat_bonus += 24;
                break;
            case JOKER_PAIR:
                if (eval->type == HAND_PAIR || eval->type == HAND_TWO_PAIR || eval->type == HAND_FULL_HOUSE) {
                    flat_bonus += 60;
                }
                break;
            case JOKER_FLUSH:
                if (eval->type == HAND_FLUSH || eval->type == HAND_STRAIGHT_FLUSH) total_mult *= 1.5;
                break;
            case JOKER_STRAIGHT:
                if (eval->type == HAND_STRAIGHT || eval->type == HAND_STRAIGHT_FLUSH) total_mult *= 1.4;
                break;
            case JOKER_PRISM:
                flat_bonus += wild_cards * 25;
                break;
            case JOKER_ROYAL:
                flat_bonus += royal_cards * 15;
                break;
            default:
                break;
        }

        switch (joker->edition) {
            case EDITION_FOIL:
                flat_bonus += 12;
                break;
            case EDITION_HOLOGRAPHIC:
                mult += 1;
                break;
            case EDITION_POLYCHROME:
                total_mult *= 1.2;
                break;
            case EDITION_NEGATIVE:
                flat_bonus += 18;
                break;
            default:
                break;
        }
    }

    if (build->coupons > 0) {
        total_mult *= 1.0 + (build->coupons * 0.03);
    }
    if (phase->debuff_percent > 0) {
        total_mult *= (100.0 - phase->debuff_percent) / 100.0;
    }

    if (mult < 1) mult = 1;
    double total = (double)(base + flat_bonus) * mult * total_mult;
    if (total < 0.0) total = 0.0;
    return (int)(total + 0.5);
}

static void add_tarot_to_inventory(PlayerBuild *build, TarotType tarot) {
    if (build->tarot_count >= MAX_TAROTS) return;
    build->tarot_inventory[build->tarot_count++] = tarot;
    register_discovery_tarot(build, tarot);
}

static void handle_discard_seals(PlayerBuild *build, Card *hand[], int indices[], int count) {
    int purple_seen = 0;
    for (int i = 0; i < count; i++) {
        if (hand[indices[i]]->seal == SEAL_PURPLE) purple_seen = 1;
    }
    if (purple_seen && build->tarot_count < MAX_TAROTS) {
        TarotType reward = random_tarot_general();
        add_tarot_to_inventory(build, reward);
        printf("Selo Roxo ativado: voce recebeu o Tarot %s.\n", tarot_name(reward));
    }
}

static void handle_blue_seal_end_phase(PlayerBuild *build, Card *hand[], int hand_count) {
    int blue_seen = 0;
    for (int i = 0; i < hand_count; i++) {
        if (hand[i]->seal == SEAL_BLUE) blue_seen = 1;
    }
    if (blue_seen && build->tarot_count < MAX_TAROTS) {
        TarotType reward = random_tarot_general();
        add_tarot_to_inventory(build, reward);
        printf("Selo Azul ativado: voce guardou um Tarot extra (%s).\n", tarot_name(reward));
    }
}

static void show_phase_receipt(const PhaseState *phase, int base_reward, int hand_reward, int interest_reward,
                               int economy_bonus, int total_reward) {
    printf("\n========================================\n");
    printf("RECIBO DA FASE - APOSTA %d/8 | %s\n", phase->ante, phase->blind_name);
    printf("========================================\n");
    wait_ms(80);
    printf("Recompensa da fase (%s)........ +$%d\n", phase->blind_name, base_reward);
    wait_ms(80);
    printf("Maos sobrando ($1 por mao)..... +$%d\n", hand_reward);
    wait_ms(80);
    printf("Juros (1 por $5, max. 5)....... +$%d\n", interest_reward);
    wait_ms(80);
    if (economy_bonus > 0) {
        printf("Coringa Economico.............. +$%d\n", economy_bonus);
        wait_ms(80);
    }
    printf("----------------------------------------\n");
    printf("TOTAL........................... +$%d\n", total_reward);
    printf("========================================\n");
}

static void update_run_stats(PlayerBuild *build, const HandEval *eval, int gained) {
    build->stats.hands_played++;
    build->stats.hand_play_counts[eval->type]++;
    if (gained > build->stats.best_hand_score) {
        build->stats.best_hand_score = gained;
        build->stats.best_hand_type = eval->type;
    }
}

static int most_played_hand(const PlayerBuild *build) {
    int best = HAND_HIGH_CARD;
    int best_count = -1;
    for (int i = 0; i <= HAND_STRAIGHT_FLUSH; i++) {
        if (build->stats.hand_play_counts[i] > best_count) {
            best_count = build->stats.hand_play_counts[i];
            best = i;
        }
    }
    return best;
}

static void print_final_summary(const PlayerBuild *build, int won_run, const PhaseState *last_phase) {
    int ante_progress = build->stats.phases_cleared / 3;
    if (won_run) ante_progress = 8;

    printf("\n========================================\n");
    printf("RESUMO DA TENTATIVA\n");
    printf("========================================\n");
    if (won_run) {
        printf("Resultado: run concluida com sucesso.\n");
    } else {
        printf("Resultado: derrota na Aposta %d/8 - %s.\n", last_phase->ante, last_phase->blind_name);
    }
    printf("Melhor mao: %s (%d pontos)\n",
           hand_type_name(build->stats.best_hand_type), build->stats.best_hand_score);
    printf("Mao mais jogada: %s (%d)\n",
           hand_type_name(most_played_hand(build)),
           build->stats.hand_play_counts[most_played_hand(build)]);
    printf("Maos jogadas: %d\n", build->stats.hands_played);
    printf("Cartas descartadas: %d\n", build->stats.cards_discarded);
    printf("Cartas compradas na loja/pacotes: %d\n", build->stats.cards_bought);
    printf("Tarots usados: %d\n", build->stats.tarots_used);
    printf("Pacotes abertos: %d\n", build->stats.packs_opened);
    printf("Novas descobertas: %d\n", build->stats.discoveries);
    printf("Juros acumulados: $%d\n", build->stats.interest_earned);
    printf("Apostas vencidas: %d/8\n", ante_progress);
    printf("Fases vencidas: %d/24\n", build->stats.phases_cleared);
    printf("Moedas finais: $%d\n", build->coins);
    printf("========================================\n");
}

static void post_phase_quiz_and_rewards(PlayerBuild *build, const PhaseState *phase) {
    if (phase->blind_type == BLIND_BOSS) {
        printf("\nBlind Chefe vencido. A aposta %d foi concluida.\n", phase->ante);
    } else {
        QuizResult quiz = ask_quiz(0);
        build->coins += quiz.coins;
        if (quiz.correct) {
            build->quiz_hits++;
            if (build->quiz_hits % 2 == 0) {
                build->coupons++;
                printf("Cupom liberado! Descontos leves na loja e +3%% de mult global por cupom.\n");
            }
        }
    }

    if ((phase->phase_number % 2) == 0 && build->tarot_count < MAX_TAROTS) {
        TarotType bonus = random_tarot_general();
        add_tarot_to_inventory(build, bonus);
        printf("Bonus de fase: voce recebeu o Tarot %s.\n", tarot_name(bonus));
    }
}

static int play_phase(PlayerBuild *build, int phase_number, PhaseState *out_phase) {
    Deck deck;
    Card *hand[HAND_SIZE];
    int hand_count = 0;
    PhaseState phase;

    setup_phase(&phase, phase_number);
    deck_from_cards(&deck, build->deck_cards, build->deck_size);
    deck_shuffle(&deck);
    fill_initial_hand(&deck, hand, &hand_count);

    printf("\n========== APOSTA %d/8 | %s ==========\n", phase.ante, phase.blind_name);
    printf("Meta da fase: %d pontos\n", phase.goal);

    if (phase.blind_type == BLIND_BOSS) {
        QuizResult boss_quiz = ask_quiz(1);
        build->coins += boss_quiz.coins;
        if (boss_quiz.correct) {
            build->quiz_hits++;
        } else if (boss_quiz.apply_debuff) {
            int debuff = rand() % 3;
            if (debuff == 0) {
                phase.debuff_percent = 20;
                printf("Debuff: pontuacao reduzida em 20%% durante a fase.\n");
            } else if (debuff == 1 && phase.hands_left > 1) {
                phase.hands_left--;
                printf("Debuff: -1 mao disponivel.\n");
            } else if (phase.discards_left > 0) {
                phase.discards_left--;
                printf("Debuff: -1 descarte disponivel.\n");
            }
        }
    }

    while (phase.hands_left > 0 && phase.score < phase.goal && hand_count > 0) {
        printf("\n--- Aposta %d/8 | %s | Pontos %d/%d | Maos %d | Descartes %d | Deck %d ---\n",
               phase.ante, phase.blind_name, phase.score, phase.goal, phase.hands_left,
               phase.discards_left, deck.count - deck.top);
        print_build(build);
        print_score_table();
        printf("\nSua mao:\n");
        print_hand(hand, hand_count);

        printf("\n1) Jogar cartas\n");
        printf("2) Descartar cartas\n");
        printf("3) Usar Tarot\n");
        printf("0) Encerrar run\n");
        int choice = read_choice();

        if (choice == 0) {
            *out_phase = phase;
            return 0;
        }
        if (choice == 3) {
            TarotPhaseHook hook = {&phase.hands_left, &phase.next_play_bonus};
            try_use_tarot_inventory(build, TAROT_CTX_PHASE, hand, hand_count, &hook);
            continue;
        }

        int indices[HAND_SIZE];
        int max_selection = (choice == 2) ? hand_count : MAX_PLAY;
        int count = read_selection(indices, max_selection, hand_count);
        if (count <= 0) continue;

        if (choice == 1) {
            Card *selected[MAX_PLAY];
            for (int i = 0; i < count; i++) selected[i] = hand[indices[i]];

            HandEval eval = evaluate_cards(selected, count);
            int gold_income = 0;
            int gained = calculate_play_score(build, &phase, selected, count, &eval, &gold_income);
            phase.score += gained;
            phase.hands_left--;
            phase.next_play_bonus = 0;
            build->coins += gold_income;
            update_run_stats(build, &eval, gained);

            printf("Jogada: %s | base=%d | mult=%d | soma_rank=%d | pontos=%d",
                   eval.name, eval.base_score, eval.multiplier, eval.rank_sum, gained);
            if (gold_income > 0) printf(" | Ouro dos selos: +$%d", gold_income);
            printf("\n");
            remove_and_draw(hand, &hand_count, &deck, indices, count);
        } else if (choice == 2) {
            if (phase.discards_left <= 0) {
                printf("Sem descartes disponiveis.\n");
                continue;
            }
            phase.discards_left--;
            build->stats.cards_discarded += count;
            handle_discard_seals(build, hand, indices, count);
            remove_and_draw(hand, &hand_count, &deck, indices, count);
            printf("Cartas descartadas e novas cartas compradas.\n");
        } else {
            printf("Opcao invalida.\n");
        }
    }

    *out_phase = phase;

    if (phase.score >= phase.goal) {
        int economy_count = count_jokers(build, JOKER_ECONOMY);
        int base_reward = phase.blind_reward;
        int hand_reward = phase.hands_left;
        int interest_cap = 5 + economy_count;
        int interest_reward = build->coins / 5;
        int economy_bonus = economy_count;

        if (interest_reward > interest_cap) interest_reward = interest_cap;
        if (interest_reward < 0) interest_reward = 0;

        handle_blue_seal_end_phase(build, hand, hand_count);

        int total_reward = base_reward + hand_reward + interest_reward + economy_bonus;
        build->coins += total_reward;
        build->stats.interest_earned += interest_reward;
        build->stats.phases_cleared++;

        show_phase_receipt(&phase, base_reward, hand_reward, interest_reward, economy_bonus, total_reward);
        return 1;
    }

    printf("\nRun encerrada. Pontos: %d/%d.\n", phase.score, phase.goal);
    return 0;
}

void play_card_run(void) {
    PlayerBuild build;
    PhaseState last_phase;
    memset(&last_phase, 0, sizeof(last_phase));
    build_init(&build);

    printf("\n========== RUN DE CARTAS + QUIZ ==========\n");
    printf("Objetivo: vencer 8 apostas (24 fases), crescer o baralho, administrar juros e montar sua engine.\n");
    printf("Tarots de economia podem ser usados a qualquer momento; tarots de alvo so funcionam na fase ou em Pacotes de Tarot.\n");

    int survived = 1;
    for (int phase = 1; phase <= MAX_PHASES; phase++) {
        survived = play_phase(&build, phase, &last_phase);
        if (!survived) break;
        post_phase_quiz_and_rewards(&build, &last_phase);
        if (phase < MAX_PHASES) run_shop(&build);
    }

    print_final_summary(&build, survived, &last_phase);
}
