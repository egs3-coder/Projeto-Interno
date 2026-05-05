#include "shop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    JokerType type;
    CardRarity rarity;
    int base_price;
    int base_sell;
    const char *name;
    const char *description;
} JokerDef;

typedef struct {
    TarotType type;
    CardRarity rarity;
    int usage_mask;
    int min_targets;
    int max_targets;
    const char *name;
    const char *description;
} TarotDef;

static const JokerDef JOKER_DEFS[] = {
    {JOKER_NONE, RARITY_COMMON, 0, 0, "Nenhum", ""},
    {JOKER_FLAT, RARITY_COMMON, 20, 12, "Coringa Base", "+24 pontos em toda mao pontuada."},
    {JOKER_PAIR, RARITY_COMMON, 22, 13, "Coringa dos Pares", "+60 pontos em Par, Dois Pares e Full House."},
    {JOKER_FLUSH, RARITY_UNCOMMON, 28, 16, "Coringa do Naipe", "x1.5 em Flush e Straight Flush."},
    {JOKER_STRAIGHT, RARITY_UNCOMMON, 27, 15, "Coringa da Sequencia", "x1.4 em Sequencia e Straight Flush."},
    {JOKER_ECONOMY, RARITY_UNCOMMON, 26, 15, "Coringa Economico", "+$1 na recompensa da fase e +1 no teto de juros."},
    {JOKER_PRISM, RARITY_RARE, 34, 20, "Coringa Prismatico", "+25 pontos por carta de naipe coringa jogada."},
    {JOKER_ROYAL, RARITY_LEGENDARY, 44, 25, "Coringa Real", "+15 pontos para cada A, J, Q ou K jogado."}
};

static const TarotDef TAROT_DEFS[] = {
    {TAROT_NONE, RARITY_COMMON, 0, 0, 0, "Nenhum", ""},
    {TAROT_PERSISTENCE, RARITY_COMMON, TAROT_CTX_PHASE, 0, 0, "Persistencia", "+1 mao nesta fase."},
    {TAROT_FORCE, RARITY_COMMON, TAROT_CTX_PHASE, 0, 0, "Forca", "+90 pontos na proxima jogada."},
    {TAROT_TEMPERANCE, RARITY_UNCOMMON, TAROT_CTX_ANYTIME | TAROT_CTX_PHASE | TAROT_CTX_PACK, 0, 0, "Temperanca", "Receba o valor total de venda dos seus Coringas."},
    {TAROT_HERMIT, RARITY_UNCOMMON, TAROT_CTX_ANYTIME | TAROT_CTX_PHASE | TAROT_CTX_PACK, 0, 0, "Eremita", "Duplique suas moedas atuais, ate +20."},
    {TAROT_STAR, RARITY_COMMON, TAROT_CTX_PHASE | TAROT_CTX_PACK, 1, 3, "Estrela", "Converta ate 3 cartas selecionadas para Ouros."},
    {TAROT_MOON, RARITY_COMMON, TAROT_CTX_PHASE | TAROT_CTX_PACK, 1, 3, "Lua", "Converta ate 3 cartas selecionadas para Paus."},
    {TAROT_SUN, RARITY_COMMON, TAROT_CTX_PHASE | TAROT_CTX_PACK, 1, 3, "Sol", "Converta ate 3 cartas selecionadas para Copas."},
    {TAROT_WORLD, RARITY_COMMON, TAROT_CTX_PHASE | TAROT_CTX_PACK, 1, 3, "Mundo", "Converta ate 3 cartas selecionadas para Espadas."},
    {TAROT_STRENGTH, RARITY_UNCOMMON, TAROT_CTX_PHASE | TAROT_CTX_PACK, 1, 2, "Forca Maior", "Aumente em +1 o valor de ate 2 cartas."},
    {TAROT_DEATH, RARITY_RARE, TAROT_CTX_PHASE | TAROT_CTX_PACK, 2, 2, "Morte", "Converta a carta da esquerda na carta da direita."},
    {TAROT_WILD, RARITY_UNCOMMON, TAROT_CTX_PHASE | TAROT_CTX_PACK, 1, 1, "Amantes", "A carta selecionada passa a contar como qualquer naipe."},
    {TAROT_SEAL, RARITY_RARE, TAROT_CTX_PHASE | TAROT_CTX_PACK, 1, 1, "Oraculo", "Aplique um selo aleatorio a 1 carta selecionada."},
    {TAROT_ECHO, RARITY_RARE, TAROT_CTX_PHASE | TAROT_CTX_PACK, 1, 1, "Eco Astral", "Crie uma copia da carta selecionada no seu baralho."}
};

static const JokerDef *joker_def(JokerType type) {
    if (type < 0 || type > JOKER_ROYAL) return &JOKER_DEFS[0];
    return &JOKER_DEFS[type];
}

static const TarotDef *tarot_def(TarotType type) {
    if (type < 0 || type > TAROT_ECHO) return &TAROT_DEFS[0];
    return &TAROT_DEFS[type];
}

static int read_menu_choice(void) {
    char line[64];
    printf("Opcao: ");
    if (!fgets(line, sizeof(line), stdin)) return 0;
    return atoi(line);
}

static int min_int(int a, int b) {
    return a < b ? a : b;
}

static CardRarity roll_rarity(void) {
    int roll = rand() % 100;
    if (roll < 57) return RARITY_COMMON;
    if (roll < 84) return RARITY_UNCOMMON;
    if (roll < 97) return RARITY_RARE;
    return RARITY_LEGENDARY;
}

static EditionType roll_edition(void) {
    int roll = rand() % 100;
    if (roll < 70) return EDITION_NONE;
    if (roll < 83) return EDITION_FOIL;
    if (roll < 94) return EDITION_HOLOGRAPHIC;
    if (roll < 99) return EDITION_POLYCHROME;
    return EDITION_NEGATIVE;
}

static JokerType choose_joker_from_rarity(CardRarity rarity) {
    JokerType pool[8];
    int count = 0;
    for (int i = 1; i <= JOKER_ROYAL; i++) {
        if (JOKER_DEFS[i].rarity == rarity) {
            pool[count++] = (JokerType)i;
        }
    }
    if (count == 0) return JOKER_FLAT;
    return pool[rand() % count];
}

static TarotType choose_tarot_from_filter(int pack_only) {
    TarotType pool[32];
    int count = 0;
    for (int i = 1; i <= TAROT_ECHO; i++) {
        const TarotDef *def = &TAROT_DEFS[i];
        if (pack_only && !(def->usage_mask & TAROT_CTX_PACK) && !(def->usage_mask & TAROT_CTX_ANYTIME)) continue;
        if (!pack_only && (def->type == TAROT_NONE)) continue;
        pool[count++] = (TarotType)i;
    }
    if (count == 0) return TAROT_TEMPERANCE;
    return pool[rand() % count];
}

static int edition_price_bonus(EditionType edition) {
    switch (edition) {
        case EDITION_FOIL: return 4;
        case EDITION_HOLOGRAPHIC: return 5;
        case EDITION_POLYCHROME: return 8;
        case EDITION_NEGATIVE: return 10;
        default: return 0;
    }
}

static int edition_sell_bonus(EditionType edition) {
    switch (edition) {
        case EDITION_FOIL: return 3;
        case EDITION_HOLOGRAPHIC: return 4;
        case EDITION_POLYCHROME: return 6;
        case EDITION_NEGATIVE: return 7;
        default: return 0;
    }
}

static int tarot_is_allowed(TarotType type, int context_mask) {
    const TarotDef *def = tarot_def(type);
    if (context_mask == TAROT_CTX_ANYTIME) {
        return (def->usage_mask & TAROT_CTX_ANYTIME) != 0;
    }
    if (context_mask == TAROT_CTX_PHASE) {
        return (def->usage_mask & TAROT_CTX_PHASE) != 0 || (def->usage_mask & TAROT_CTX_ANYTIME) != 0;
    }
    if (context_mask == TAROT_CTX_PACK) {
        return (def->usage_mask & TAROT_CTX_PACK) != 0 || (def->usage_mask & TAROT_CTX_ANYTIME) != 0;
    }
    return 0;
}

static void print_card_pool(Card *cards[], int count) {
    for (int i = 0; i < count; i++) {
        printf("[%d] ", i + 1);
        print_card(cards[i]);
        printf("\n");
    }
}

static int read_target_indices(int out[], int min_targets, int max_targets, int available) {
    char line[256];
    while (1) {
        printf("Selecione entre %d e %d carta(s) pelo indice, ou 0 para cancelar: ", min_targets, max_targets);
        if (!fgets(line, sizeof(line), stdin)) return 0;
        if (line[0] == '0') return 0;
        int count = parse_indices(line, out, max_targets, available);
        if (count >= min_targets && count <= max_targets) return count;
        printf("Selecao invalida.\n");
    }
}

static void sample_cards_from_deck(PlayerBuild *build, Card *out[], int *count) {
    int indices[MAX_DECK_CARDS];
    int sample_count = min_int(build->deck_size, PACK_PREVIEW_SIZE);
    for (int i = 0; i < build->deck_size; i++) indices[i] = i;
    for (int i = build->deck_size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }
    for (int i = 0; i < sample_count; i++) {
        out[i] = &build->deck_cards[indices[i]];
    }
    *count = sample_count;
}

static SealType random_seal(void) {
    int roll = rand() % 4;
    switch (roll) {
        case 0: return SEAL_GOLD;
        case 1: return SEAL_RED;
        case 2: return SEAL_BLUE;
        default: return SEAL_PURPLE;
    }
}

static int add_tarot_to_inventory(PlayerBuild *build, TarotType tarot) {
    if (build->tarot_count >= MAX_TAROTS) {
        printf("Inventario de Tarot cheio.\n");
        return 0;
    }
    build->tarot_inventory[build->tarot_count++] = tarot;
    register_discovery_tarot(build, tarot);
    return 1;
}

static int add_joker_to_inventory(PlayerBuild *build, OwnedJoker joker) {
    if (build->joker_count >= MAX_JOKERS) {
        printf("Limite de Coringas atingido.\n");
        return 0;
    }
    build->jokers[build->joker_count++] = joker;
    register_discovery_joker(build, joker.type);
    return 1;
}

static int apply_tarot_effect(PlayerBuild *build, TarotType type, Card *targets[], int target_count, TarotPhaseHook *hook) {
    switch (type) {
        case TAROT_PERSISTENCE:
            if (!hook || !hook->hands_left) return 0;
            *hook->hands_left += 1;
            printf("Persistencia usada: +1 mao nesta fase.\n");
            return 1;
        case TAROT_FORCE:
            if (!hook || !hook->next_play_bonus) return 0;
            *hook->next_play_bonus += 90;
            printf("Forca usada: +90 pontos na proxima jogada.\n");
            return 1;
        case TAROT_TEMPERANCE: {
            int reward = 0;
            for (int i = 0; i < build->joker_count; i++) reward += joker_sell_value(&build->jokers[i]);
            build->coins += reward;
            printf("Temperanca usada: +%d moedas com base no valor de venda dos seus Coringas.\n", reward);
            return 1;
        }
        case TAROT_HERMIT: {
            int reward = build->coins;
            if (reward > 20) reward = 20;
            build->coins += reward;
            printf("Eremita usado: +%d moedas.\n", reward);
            return 1;
        }
        case TAROT_STAR:
            for (int i = 0; i < target_count; i++) targets[i]->suit = SUIT_OUROS;
            printf("Estrela usada: cartas convertidas para Ouros.\n");
            return 1;
        case TAROT_MOON:
            for (int i = 0; i < target_count; i++) targets[i]->suit = SUIT_PAUS;
            printf("Lua usada: cartas convertidas para Paus.\n");
            return 1;
        case TAROT_SUN:
            for (int i = 0; i < target_count; i++) targets[i]->suit = SUIT_COPAS;
            printf("Sol usado: cartas convertidas para Copas.\n");
            return 1;
        case TAROT_WORLD:
            for (int i = 0; i < target_count; i++) targets[i]->suit = SUIT_ESPADAS;
            printf("Mundo usado: cartas convertidas para Espadas.\n");
            return 1;
        case TAROT_STRENGTH:
            for (int i = 0; i < target_count; i++) {
                if (targets[i]->rank < 13) targets[i]->rank++;
            }
            printf("Forca Maior usada: cartas fortalecidas.\n");
            return 1;
        case TAROT_DEATH:
            if (target_count != 2) return 0;
            *targets[0] = *targets[1];
            printf("Morte usada: a carta da esquerda virou a carta da direita.\n");
            return 1;
        case TAROT_WILD:
            if (target_count != 1) return 0;
            targets[0]->wild_suit = 1;
            printf("Amantes usado: a carta agora vale como qualquer naipe.\n");
            return 1;
        case TAROT_SEAL:
            if (target_count != 1) return 0;
            targets[0]->seal = random_seal();
            printf("Oraculo usado: %s aplicado.\n", seal_name(targets[0]->seal));
            return 1;
        case TAROT_ECHO:
            if (target_count != 1) return 0;
            if (build->deck_size >= MAX_DECK_CARDS) {
                printf("Seu baralho ja esta no limite.\n");
                return 0;
            }
            build->deck_cards[build->deck_size++] = *targets[0];
            printf("Eco Astral usado: uma copia da carta foi adicionada ao baralho.\n");
            return 1;
        default:
            return 0;
    }
}

static int resolve_tarot(PlayerBuild *build, TarotType tarot, int context_mask, Card *cards[], int card_count, TarotPhaseHook *hook) {
    const TarotDef *def = tarot_def(tarot);
    if (!tarot_is_allowed(tarot, context_mask)) {
        printf("Esse Tarot nao pode ser usado neste momento.\n");
        return 0;
    }

    if (def->max_targets <= 0) {
        if (apply_tarot_effect(build, tarot, NULL, 0, hook)) {
            build->stats.tarots_used++;
            return 1;
        }
        printf("Esse Tarot exige uma fase ativa para ser usado.\n");
        return 0;
    }

    if (!cards || card_count <= 0) {
        printf("Nao ha cartas disponiveis para esse Tarot agora.\n");
        return 0;
    }

    printf("\nCartas disponiveis para %s:\n", tarot_name(tarot));
    print_card_pool(cards, card_count);

    int indices[3];
    int target_count = read_target_indices(indices, def->min_targets, def->max_targets, card_count);
    if (target_count <= 0) {
        printf("Uso do Tarot cancelado.\n");
        return 0;
    }

    Card *targets[3];
    for (int i = 0; i < target_count; i++) {
        targets[i] = cards[indices[i]];
    }

    if (apply_tarot_effect(build, tarot, targets, target_count, hook)) {
        build->stats.tarots_used++;
        return 1;
    }

    printf("Nao foi possivel resolver esse Tarot.\n");
    return 0;
}

static void print_joker_offer(const OwnedJoker *joker, int index) {
    printf("%d) %s | %s | %s\n", index, joker_name(joker->type),
           rarity_name(joker->rarity), edition_name(joker->edition));
    printf("   %s\n", joker_description(joker->type));
}

static void buy_joker(PlayerBuild *build, OwnedJoker joker, int cost) {
    if (build->coins < cost) {
        printf("Moedas insuficientes.\n");
        return;
    }
    if (!add_joker_to_inventory(build, joker)) return;
    build->coins -= cost;
    build->stats.cards_bought++;
    printf("Comprado: %s (%s, %s)\n", joker_name(joker.type),
           rarity_name(joker.rarity), edition_name(joker.edition));
}

static void buy_tarot(PlayerBuild *build, TarotType tarot, int cost) {
    if (build->coins < cost) {
        printf("Moedas insuficientes.\n");
        return;
    }
    if (!add_tarot_to_inventory(build, tarot)) return;
    build->coins -= cost;
    build->stats.cards_bought++;
    printf("Comprado: %s\n", tarot_name(tarot));
}

static void sell_joker(PlayerBuild *build) {
    if (build->joker_count <= 0) {
        printf("Voce nao possui Coringas para vender.\n");
        return;
    }

    printf("\nEscolha o Coringa para inspecionar e vender:\n");
    for (int i = 0; i < build->joker_count; i++) {
        printf("%d) %s | %s | %s\n", i + 1, joker_name(build->jokers[i].type),
               rarity_name(build->jokers[i].rarity), edition_name(build->jokers[i].edition));
    }
    printf("0) Cancelar\n");
    int choice = read_menu_choice();
    if (choice <= 0 || choice > build->joker_count) return;

    int idx = choice - 1;
    int value = joker_sell_value(&build->jokers[idx]);
    printf("%s vale %d moedas. Vender?\n", joker_name(build->jokers[idx].type), value);
    printf("1) Sim\n0) Nao\n");
    int confirm = read_menu_choice();
    if (confirm != 1) return;

    build->coins += value;
    printf("Vendido: %s por %d moedas.\n", joker_name(build->jokers[idx].type), value);
    for (int i = idx; i < build->joker_count - 1; i++) {
        build->jokers[i] = build->jokers[i + 1];
    }
    build->joker_count--;
}

static void open_joker_pack(PlayerBuild *build, int option_count, int picks, int cost) {
    if (build->coins < cost) {
        printf("Moedas insuficientes para este pacote.\n");
        return;
    }
    if (build->joker_count >= MAX_JOKERS) {
        printf("Sem espaco para novos Coringas.\n");
        return;
    }

    OwnedJoker options[5];
    int taken[5] = {0};
    for (int i = 0; i < option_count; i++) options[i] = roll_random_joker();

    build->coins -= cost;
    build->stats.packs_opened++;

    int picks_left = min_int(picks, MAX_JOKERS - build->joker_count);
    while (picks_left > 0) {
        printf("\nPacote de Coringas aberto: escolha %d carta(s).\n", picks_left);
        for (int i = 0; i < option_count; i++) {
            if (!taken[i]) print_joker_offer(&options[i], i + 1);
        }
        printf("0) Encerrar selecao\n");
        int choice = read_menu_choice();
        if (choice == 0) break;
        if (choice < 1 || choice > option_count || taken[choice - 1]) {
            printf("Opcao invalida.\n");
            continue;
        }
        if (add_joker_to_inventory(build, options[choice - 1])) {
            taken[choice - 1] = 1;
            build->stats.cards_bought++;
            picks_left--;
            printf("Selecionado: %s\n", joker_name(options[choice - 1].type));
        }
    }
}

static void open_tarot_pack(PlayerBuild *build, int option_count, int picks, int cost) {
    if (build->coins < cost) {
        printf("Moedas insuficientes para este pacote.\n");
        return;
    }

    TarotType options[5];
    int taken[5] = {0};
    for (int i = 0; i < option_count; i++) options[i] = random_tarot_for_pack();

    Card *preview[PACK_PREVIEW_SIZE];
    int preview_count = 0;
    sample_cards_from_deck(build, preview, &preview_count);

    build->coins -= cost;
    build->stats.packs_opened++;

    printf("\nPacote de Tarot aberto. Estas cartas foram puxadas do seu baralho para uso imediato:\n");
    print_card_pool(preview, preview_count);

    int picks_left = picks;
    while (picks_left > 0) {
        printf("\nEscolha %d Tarot(s) para resolver agora.\n", picks_left);
        for (int i = 0; i < option_count; i++) {
            if (taken[i]) continue;
            printf("%d) %s | %s\n", i + 1, tarot_name(options[i]), rarity_name(tarot_def(options[i])->rarity));
            printf("   %s\n", tarot_description(options[i]));
        }
        printf("0) Encerrar selecao\n");

        int choice = read_menu_choice();
        if (choice == 0) break;
        if (choice < 1 || choice > option_count || taken[choice - 1]) {
            printf("Opcao invalida.\n");
            continue;
        }

        if (resolve_tarot(build, options[choice - 1], TAROT_CTX_PACK, preview, preview_count, NULL)) {
            taken[choice - 1] = 1;
            build->stats.cards_bought++;
            picks_left--;
        }
    }
}

void build_init(PlayerBuild *build) {
    memset(build, 0, sizeof(*build));
    build->coins = 25;
    build_standard_deck(build->deck_cards, &build->deck_size);
}

const char *rarity_name(CardRarity rarity) {
    switch (rarity) {
        case RARITY_COMMON: return "Comum";
        case RARITY_UNCOMMON: return "Incomum";
        case RARITY_RARE: return "Raro";
        case RARITY_LEGENDARY: return "Lendario";
        default: return "Desconhecido";
    }
}

const char *edition_name(EditionType edition) {
    switch (edition) {
        case EDITION_FOIL: return "Foil";
        case EDITION_HOLOGRAPHIC: return "Holografico";
        case EDITION_POLYCHROME: return "Policromado";
        case EDITION_NEGATIVE: return "Negativo";
        default: return "Base";
    }
}

const char *joker_name(JokerType type) {
    return joker_def(type)->name;
}

const char *joker_description(JokerType type) {
    return joker_def(type)->description;
}

const char *tarot_name(TarotType type) {
    return tarot_def(type)->name;
}

const char *tarot_description(TarotType type) {
    return tarot_def(type)->description;
}

const char *hand_type_name(HandType type) {
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

int joker_sell_value(const OwnedJoker *joker) {
    const JokerDef *def = joker_def(joker->type);
    return def->base_sell + edition_sell_bonus(joker->edition);
}

void register_discovery_joker(PlayerBuild *build, JokerType type) {
    if (type <= JOKER_NONE || type > JOKER_ROYAL) return;
    if (!build->seen_jokers[type]) {
        build->seen_jokers[type] = 1;
        build->stats.discoveries++;
    }
}

void register_discovery_tarot(PlayerBuild *build, TarotType type) {
    if (type <= TAROT_NONE || type > TAROT_ECHO) return;
    if (!build->seen_tarots[type]) {
        build->seen_tarots[type] = 1;
        build->stats.discoveries++;
    }
}

OwnedJoker roll_random_joker(void) {
    CardRarity rarity = roll_rarity();
    JokerType type = choose_joker_from_rarity(rarity);
    OwnedJoker joker;
    joker.type = type;
    joker.rarity = rarity;
    joker.edition = roll_edition();
    return joker;
}

TarotType random_tarot_general(void) {
    return choose_tarot_from_filter(0);
}

TarotType random_tarot_for_pack(void) {
    return choose_tarot_from_filter(1);
}

void print_build(const PlayerBuild *build) {
    printf("Moedas: %d | Cupons: %d | Coringas: %d/%d | Tarots: %d/%d | Baralho: %d cartas\n",
           build->coins, build->coupons, build->joker_count, MAX_JOKERS,
           build->tarot_count, MAX_TAROTS, build->deck_size);
    if (build->joker_count > 0) {
        printf("Coringas ativos:\n");
        for (int i = 0; i < build->joker_count; i++) {
            printf("  %d) %s | %s | %s\n", i + 1, joker_name(build->jokers[i].type),
                   edition_name(build->jokers[i].edition), rarity_name(build->jokers[i].rarity));
            printf("     %s\n", joker_description(build->jokers[i].type));
        }
    }
    if (build->tarot_count > 0) {
        printf("Tarots no inventario:\n");
        for (int i = 0; i < build->tarot_count; i++) {
            printf("  %d) %s | %s\n", i + 1, tarot_name(build->tarot_inventory[i]),
                   rarity_name(tarot_def(build->tarot_inventory[i])->rarity));
            printf("     %s\n", tarot_description(build->tarot_inventory[i]));
        }
    }
}

int try_use_tarot_inventory(PlayerBuild *build, int context_mask, Card *cards[], int card_count, TarotPhaseHook *hook) {
    if (build->tarot_count <= 0) {
        printf("Voce nao possui Tarot.\n");
        return 0;
    }

    printf("\nEscolha um Tarot para usar:\n");
    for (int i = 0; i < build->tarot_count; i++) {
        int allowed = tarot_is_allowed(build->tarot_inventory[i], context_mask);
        printf("%d) %s%s\n", i + 1, tarot_name(build->tarot_inventory[i]),
               allowed ? "" : " [indisponivel agora]");
        printf("   %s\n", tarot_description(build->tarot_inventory[i]));
    }
    printf("0) Cancelar\n");
    int choice = read_menu_choice();
    if (choice <= 0 || choice > build->tarot_count) return 0;

    int idx = choice - 1;
    TarotType tarot = build->tarot_inventory[idx];
    if (!resolve_tarot(build, tarot, context_mask, cards, card_count, hook)) return 0;

    for (int i = idx; i < build->tarot_count - 1; i++) {
        build->tarot_inventory[i] = build->tarot_inventory[i + 1];
    }
    build->tarot_count--;
    return 1;
}

void run_shop(PlayerBuild *build) {
    build->stats.shop_visits++;

    OwnedJoker today_joker = roll_random_joker();
    TarotType today_tarot = random_tarot_general();
    int coupon_discount = build->coupons > 0 ? min_int(6, 2 + build->coupons) : 0;
    int joker_cost = joker_def(today_joker.type)->base_price + edition_price_bonus(today_joker.edition) - coupon_discount;
    int tarot_cost = 14 - (coupon_discount / 2);
    int pack_joker_3_cost = 18 - coupon_discount;
    int pack_joker_5_cost = 28 - coupon_discount;
    int pack_joker_2of5_cost = 40 - coupon_discount;
    int pack_tarot_3_cost = 12 - (coupon_discount / 2);
    int pack_tarot_5_cost = 18 - (coupon_discount / 2);
    int pack_tarot_2of5_cost = 26 - (coupon_discount / 2);

    if (joker_cost < 10) joker_cost = 10;
    if (tarot_cost < 8) tarot_cost = 8;
    if (pack_joker_3_cost < 12) pack_joker_3_cost = 12;
    if (pack_joker_5_cost < 18) pack_joker_5_cost = 18;
    if (pack_joker_2of5_cost < 24) pack_joker_2of5_cost = 24;
    if (pack_tarot_3_cost < 9) pack_tarot_3_cost = 9;
    if (pack_tarot_5_cost < 12) pack_tarot_5_cost = 12;
    if (pack_tarot_2of5_cost < 18) pack_tarot_2of5_cost = 18;

    int running = 1;
    while (running) {
        printf("\n========== LOJA ==========\n");
        print_build(build);
        printf("1) Comprar Coringa do dia (%d moedas)\n", joker_cost);
        print_joker_offer(&today_joker, 1);
        printf("2) Comprar Tarot do dia (%d moedas): %s\n", tarot_cost, tarot_name(today_tarot));
        printf("   %s\n", tarot_description(today_tarot));
        printf("3) Pacote de Coringas 1/3 (%d moedas)\n", pack_joker_3_cost);
        printf("4) Pacote de Coringas 1/5 (%d moedas)\n", pack_joker_5_cost);
        printf("5) Pacote de Coringas 2/5 (%d moedas)\n", pack_joker_2of5_cost);
        printf("6) Pacote de Tarot 1/3 (%d moedas)\n", pack_tarot_3_cost);
        printf("7) Pacote de Tarot 1/5 (%d moedas)\n", pack_tarot_5_cost);
        printf("8) Pacote de Tarot 2/5 (%d moedas)\n", pack_tarot_2of5_cost);
        printf("9) Usar Tarot livre do inventario\n");
        printf("10) Vender Coringa\n");
        printf("0) Continuar run\n");

        int choice = read_menu_choice();
        switch (choice) {
            case 1: buy_joker(build, today_joker, joker_cost); break;
            case 2: buy_tarot(build, today_tarot, tarot_cost); break;
            case 3: open_joker_pack(build, 3, 1, pack_joker_3_cost); break;
            case 4: open_joker_pack(build, 5, 1, pack_joker_5_cost); break;
            case 5: open_joker_pack(build, 5, 2, pack_joker_2of5_cost); break;
            case 6: open_tarot_pack(build, 3, 1, pack_tarot_3_cost); break;
            case 7: open_tarot_pack(build, 5, 1, pack_tarot_5_cost); break;
            case 8: open_tarot_pack(build, 5, 2, pack_tarot_2of5_cost); break;
            case 9: try_use_tarot_inventory(build, TAROT_CTX_ANYTIME, NULL, 0, NULL); break;
            case 10: sell_joker(build); break;
            case 0: running = 0; break;
            default: printf("Opcao invalida.\n"); break;
        }
    }
}
