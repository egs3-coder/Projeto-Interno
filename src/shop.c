#include "shop.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    RARITY_COMMON = 0,
    RARITY_UNCOMMON,
    RARITY_RARE
} JokerRarity;

typedef enum {
    SHOP_SLOT_EMPTY = 0,
    SHOP_SLOT_JOKER,
    SHOP_SLOT_TAROT
} ShopSlotKind;

typedef struct {
    ShopSlotKind kind;
    JokerType joker;
    JokerEdition edition;
    TarotType tarot;
    int price;
} ShopSlot;

typedef struct {
    ShopSlot slots[2];
    CouponType coupon;
    int coupon_available;
    int pack_available;
    int pack_cost;
    int reroll_cost;
} ShopState;

static int read_menu_choice(void) {
    char line[64];
    printf("Opcao: ");
    if (!fgets(line, sizeof(line), stdin)) return 0;
    return atoi(line);
}

static int coupon_family_from_type(CouponType type) {
    return (int)type / 2;
}

static int coupon_level_from_type(CouponType type) {
    return ((int)type % 2) == 0 ? 1 : 2;
}

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static void init_joker_instance(JokerInstance *joker, JokerType type) {
    joker->type = type;
    joker->edition = EDITION_NONE;
    joker->active = 1;
    joker->value = 0;
    joker->counter = 0;
    joker->sell_value = 1;

    switch (type) {
        case JOKER_POPCORN:
            joker->value = 24;
            break;
        case JOKER_ICE_CREAM:
            joker->value = 80;
            break;
        case JOKER_CAVENDISH:
            joker->value = 14;
            break;
        case JOKER_COFFEE:
            joker->counter = 3;
            break;
        case JOKER_LUNCHBOX:
            joker->counter = 3;
            joker->value = 45;
            break;
        case JOKER_RED_CARD:
            joker->value = 0;
            break;
        default:
            break;
    }
}

static JokerRarity joker_rarity(JokerType type) {
    switch (type) {
        case JOKER_FOUR:
        case JOKER_CRYPTID_RELAY:
        case JOKER_COSMOS_PRISM:
        case JOKER_LUCKY_JIMBO:
        case JOKER_BOSS_SHIELD:
        case JOKER_NOBLE_LINEAGE:
        case JOKER_MIRROR_QUIZ:
            return RARITY_RARE;
        case JOKER_THREE:
        case JOKER_STRAIGHT:
        case JOKER_FLUSH:
        case JOKER_ROYAL_COUNCIL:
        case JOKER_THRONE:
        case JOKER_OCCULT_LIBRARY:
        case JOKER_RITUAL_TABLE:
        case JOKER_STENCIL:
        case JOKER_PI_CACHE:
        case JOKER_RED_CARD:
        case JOKER_ARCANE_MINOR:
            return RARITY_UNCOMMON;
        default:
            return RARITY_COMMON;
    }
}

static int rarity_price(JokerRarity rarity) {
    switch (rarity) {
        case RARITY_COMMON:
            return 1 + rand() % 6;
        case RARITY_UNCOMMON:
            return 4 + rand() % 5;
        case RARITY_RARE:
            return 7 + rand() % 4;
        default:
            return 4;
    }
}

const char *joker_rarity_name(JokerType type) {
    switch (joker_rarity(type)) {
        case RARITY_RARE:
            return "Raro";
        case RARITY_UNCOMMON:
            return "Incomum";
        default:
            return "Comum";
    }
}

int joker_base_price(JokerType type) {
    switch (joker_rarity(type)) {
        case RARITY_RARE:
            return 8;
        case RARITY_UNCOMMON:
            return 6;
        default:
            return 4;
    }
}

int joker_edition_extra_cost(JokerEdition edition) {
    switch (edition) {
        case EDITION_FOIL:
            return 2;
        case EDITION_CHROME:
            return 3;
        case EDITION_PRISMATIC:
        case EDITION_NEGATIVE:
            return 5;
        default:
            return 0;
    }
}

int shop_discounted_price(int base_cost, const PlayerBuild *build) {
    int numerator = 100;
    if (build->coupon_levels[0] >= 2) numerator = 50;
    else if (build->coupon_levels[0] >= 1) numerator = 75;

    int result = (base_cost * numerator) / 100;
    if (result < 1) result = 1;
    return result;
}

JokerType random_rare_joker(void) {
    JokerType rare[] = {
        JOKER_FOUR,
        JOKER_BOSS_SHIELD,
        JOKER_CRYPTID_RELAY,
        JOKER_COSMOS_PRISM,
        JOKER_LUCKY_JIMBO,
        JOKER_NOBLE_LINEAGE
    };
    return rare[rand() % (int)(sizeof(rare) / sizeof(rare[0]))];
}

JokerType random_legendary_joker(void) {
    JokerType legendary[] = {
        JOKER_NOBLE_LINEAGE,
        JOKER_BOSS_SHIELD,
        JOKER_CRYPTID_RELAY,
        JOKER_COSMOS_PRISM
    };
    return legendary[rand() % (int)(sizeof(legendary) / sizeof(legendary[0]))];
}

TarotType random_arcana_pack_tarot(void) {
    int roll = rand() % 100;
    if (roll < 5) return TAROT_SOUL;
    if (roll < 10) return TAROT_ANKH;
    if (roll < 16) return TAROT_WRAITH;
    if (roll < 24) return TAROT_AURA;
    return random_tarot();
}

JokerEdition random_shop_edition(const PlayerBuild *build, int phase_number) {
    int roll = rand() % 1000;
    int ante = ((phase_number - 1) / 3) + 1;
    int hone = build ? build->coupon_levels[3] : 0;
    int foil;
    int chrome;
    int prismatic;
    int negative = 18;

    if (ante < 1) ante = 1;
    foil = ante < 4 ? 90 : 100;
    chrome = ante < 4 ? 40 : 45;
    prismatic = ante < 4 ? 10 : 15;

    if (hone >= 1) {
        foil += 30;
        chrome += 20;
        prismatic += 5;
        negative += 4;
    }
    if (hone >= 2) {
        foil += 30;
        chrome += 20;
        prismatic += 5;
        negative += 4;
    }

    if (roll < foil) return EDITION_FOIL;
    roll -= foil;
    if (roll < chrome) return EDITION_CHROME;
    roll -= chrome;
    if (roll < prismatic) return EDITION_PRISMATIC;
    roll -= prismatic;
    if (roll < negative) return EDITION_NEGATIVE;
    return EDITION_NONE;
}

static JokerEdition roll_shop_edition(const PlayerBuild *build) {
    int hone = build->coupon_levels[3];
    int roll = rand() % 100;

    if (hone >= 2) {
        if (roll < 24) return EDITION_FOIL;
        if (roll < 42) return EDITION_CHROME;
        if (roll < 54) return EDITION_PRISMATIC;
        if (roll < 62) return EDITION_NEGATIVE;
        return EDITION_NONE;
    }
    if (hone >= 1) {
        if (roll < 16) return EDITION_FOIL;
        if (roll < 27) return EDITION_CHROME;
        if (roll < 34) return EDITION_PRISMATIC;
        if (roll < 38) return EDITION_NEGATIVE;
        return EDITION_NONE;
    }

    if (roll < 4) return EDITION_FOIL;
    if (roll < 7) return EDITION_CHROME;
    if (roll < 9) return EDITION_PRISMATIC;
    if (roll < 10) return EDITION_NEGATIVE;
    return EDITION_NONE;
}

void build_init(PlayerBuild *build) {
    build->coins = 4;
    build->quiz_hits = 0;
    build->quiz_streak = 0;
    build->coupon_tokens = 0;
    build->tarot_capacity = 2;
    build->joker_capacity = BASE_JOKER_SLOTS;
    build->pending_editions = 0;
    build->question_rewards_taken = 0;
    build->joker_count = 0;
    build->tarot_count = 0;
    build->coupon_count = 0;
    build->last_used_tarot = TAROT_NONE;

    for (int i = 0; i < COUPON_FAMILY_COUNT; i++) build->coupon_levels[i] = 0;
    for (int i = 0; i < MAX_JOKERS; i++) init_joker_instance(&build->jokers[i], JOKER_NONE);
    for (int i = 0; i < MAX_TAROTS; i++) build->tarot_inventory[i] = TAROT_NONE;
    for (int i = 0; i < MAX_COUPONS; i++) build->coupons[i] = COUPON_CLEARANCE;
    for (int i = 0; i < MAX_HAND_LEVEL_TRACK; i++) {
        build->hand_levels[i] = 1;
        build->hand_progress[i] = 0;
    }
}

const char *joker_name(JokerType type) {
    switch (type) {
        /* #1  */ case JOKER_FLAT:          return "Coringa";
        /* #2  */ case JOKER_GREEDY:        return "Cor. Ganancioso";
        /* #3  */ case JOKER_LOVELY:        return "Cor. Vigoroso";
        /* #4  */ case JOKER_WRATHFUL:      return "Coringa Irado";
        /* #5  */ case JOKER_ASTUTE:        return "Cor. Guloso";
        /* #6  */ case JOKER_PAIR:          return "Coringa Alegre";
        /* #7  */ case JOKER_THREE:         return "Coringa Bobo";
        /* #8  */ case JOKER_STRAIGHT:      return "Coringa Maluco";
        /* #9  */ case JOKER_FLUSH:         return "Cor. Engracado";
        /* #10 */ case JOKER_FOUR:          return "Cor. Bizonho";
        /* #11 */ case JOKER_ROYAL_KING:    return "Cara Assustadora";
        /* #12 */ case JOKER_ROYAL_QUEEN:   return "Academico";
        /* #13 */ case JOKER_SIXTH_SENSE:   return "Quatro Dedos";
        /* #14 */ case JOKER_ARCANE_MINOR:  return "Atalho";
        /* #15 */ case JOKER_THRONE:        return "Respingo";
        /* #16 */ case JOKER_ROYAL_COUNCIL: return "Pareidolia";
        /* #17 */ case JOKER_OCCULT_LIBRARY:return "Cor. Borrado";
        /* #18 */ case JOKER_COIN_BOSS:     return "Comprovante";
        /* #19 */ case JOKER_NOBLE_LINEAGE: return "Mimico";
        /* #20 */ case JOKER_ROYAL_JACK:    return "Meias e Bucins";
        /* #21 */ case JOKER_LUNCHBOX:      return "Anoitecer";
        /* #22 */ case JOKER_COFFEE:        return "Agua com Gas";
        /* #23 */ case JOKER_COIN_FLUSH:    return "Cor. Dourado";
        /* #24 */ case JOKER_COIN_PAIR:     return "Cartao de Visitas";
        /* #25 */ case JOKER_MIRROR_QUIZ:   return "Ate a Lua";
        /* #26 */ case JOKER_STENCIL:       return "Cartao de Credito";
        /* #27 */ case JOKER_COIN_THREE:    return "Foguete";
        /* #28 */ case JOKER_COIN_ROYAL:    return "Estac. Reservado";
        /* #29 */ case JOKER_COIN_LOW:      return "Banana";
        /* #30 */ case JOKER_CAVENDISH:     return "Banana Cavendish";
        /* #31 */ case JOKER_POPCORN:       return "Pipoca";
        /* #32 */ case JOKER_ICE_CREAM:     return "Sorvete";
        /* #33 */ case JOKER_LUCKY_JIMBO:   return "Cor. Verde";
        /* #34 */ case JOKER_ECHO_ARCANO:   return "Corredor";
        /* #35 */ case JOKER_MARKER:        return "Vidente";
        /* #36 */ case JOKER_RITUAL_TABLE:  return "Constelacao";
        /* #37 */ case JOKER_TWO_PAIR:      return "Calcas Extras";
        /* #38 */ case JOKER_BOSS_SHIELD:   return "Pequeno Coringa";
        /* #39 */ case JOKER_CRYPTID_RELAY: return "Projeto";
        /* #40 */ case JOKER_COSMOS_PRISM:  return "Tempest. Ideias";
        /* #41 */ case JOKER_FAMILIAR_WAGE: return "Vampiro";
        /* #42 */ case JOKER_RED_CARD:      return "Cartao Vermelho";
        /* #43 */ case JOKER_PI_CACHE:      return "Holograma";
        default: return "Nenhum";
    }
}

const char *joker_edition_name(JokerEdition edition) {
    switch (edition) {
        case EDITION_FOIL: return "Foil";
        case EDITION_CHROME: return "Cromado";
        case EDITION_PRISMATIC: return "Prismatico";
        case EDITION_NEGATIVE: return "Negative";
        default: return "Normal";
    }
}

const char *tarot_name(TarotType type) {
    switch (type) {
        case TAROT_FOOL: return "O Louco";
        case TAROT_MAGICIAN: return "O Mago";
        case TAROT_HIGH_PRIESTESS: return "A Sacerdotisa";
        case TAROT_EMPRESS: return "A Imperatriz";
        case TAROT_EMPEROR: return "O Imperador";
        case TAROT_HIEROPHANT: return "O Hierofante";
        case TAROT_LOVERS: return "Os Enamorados";
        case TAROT_CHARIOT: return "O Carro";
        case TAROT_JUSTICE: return "A Justica";
        case TAROT_HERMIT: return "O Eremita";
        case TAROT_WHEEL: return "Roda da Fortuna";
        case TAROT_STRENGTH: return "A Forca";
        case TAROT_HANGED_MAN: return "O Enforcado";
        case TAROT_DEATH: return "A Morte";
        case TAROT_TEMPERANCE: return "Temperanca";
        case TAROT_DEVIL: return "O Diabo";
        case TAROT_TOWER: return "A Torre";
        case TAROT_STAR: return "A Estrela";
        case TAROT_MOON: return "A Lua";
        case TAROT_SUN: return "O Sol";
        case TAROT_WORLD: return "O Mundo";
        case TAROT_JUDGEMENT: return "Julgamento";
        case TAROT_AURA: return "Aura";
        case TAROT_WRAITH: return "Wraith";
        case TAROT_ANKH: return "Ankh";
        case TAROT_SOUL: return "The Soul";
        default: return "Nenhum";
    }
}

const char *coupon_name(CouponType type) {
    switch (type) {
        case COUPON_CLEARANCE:      return "Liquidacao";
        case COUPON_CLEARANCE_PLUS: return "Liquidacao Total";
        case COUPON_GRABBER:        return "Mao Longa";
        case COUPON_GRABBER_PLUS:   return "Lingua de Nacho";
        case COUPON_ORACLE:         return "Comerc. de Tarot";
        case COUPON_ORACLE_PLUS:    return "Magnata do Tarot";
        case COUPON_HONE:           return "Polimento";
        case COUPON_HONE_PLUS:      return "Brilho Intenso";
        default: return "Cupom";
    }
}

JokerType random_joker(void) {
    return (JokerType)(1 + rand() % JOKER_RED_CARD);
}

JokerType random_shop_joker(void) {
    JokerType common[] = {
        JOKER_FLAT, JOKER_PAIR, JOKER_TWO_PAIR, JOKER_COIN_PAIR, JOKER_COIN_FLUSH,
        JOKER_POPCORN, JOKER_COFFEE, JOKER_ROYAL_KING, JOKER_GREEDY, JOKER_LOVELY
    };
    JokerType uncommon[] = {
        JOKER_THREE, JOKER_STRAIGHT, JOKER_FLUSH, JOKER_COIN_THREE, JOKER_COIN_ROYAL,
        JOKER_LUNCHBOX, JOKER_OCCULT_LIBRARY, JOKER_RITUAL_TABLE, JOKER_STENCIL,
        JOKER_MIRROR_QUIZ, JOKER_RED_CARD
    };
    JokerType rare[] = {
        JOKER_FOUR, JOKER_BOSS_SHIELD, JOKER_CRYPTID_RELAY, JOKER_COSMOS_PRISM,
        JOKER_LUCKY_JIMBO, JOKER_NOBLE_LINEAGE
    };
    int roll = rand() % 100;

    if (roll < 65) return common[rand() % (int)(sizeof(common) / sizeof(common[0]))];
    if (roll < 90) return uncommon[rand() % (int)(sizeof(uncommon) / sizeof(uncommon[0]))];
    return rare[rand() % (int)(sizeof(rare) / sizeof(rare[0]))];
}

TarotType random_tarot(void) {
    return (TarotType)(1 + rand() % TAROT_JUDGEMENT);
}

CouponType random_coupon_offer(const PlayerBuild *build) {
    int family = rand() % COUPON_FAMILY_COUNT;
    int level = build->coupon_levels[family];
    if (level <= 0) return (CouponType)(family * 2);
    if (level == 1) return (CouponType)(family * 2 + 1);
    return (CouponType)(family * 2 + (rand() % 2));
}

int build_add_joker(PlayerBuild *build, JokerType type) {
    if (build->joker_count >= build->joker_capacity || build->joker_count >= MAX_JOKERS) return 0;
    init_joker_instance(&build->jokers[build->joker_count], type);
    build->jokers[build->joker_count].sell_value = 2;
    build->joker_count++;
    while (build->pending_editions > 0) {
        apply_random_free_edition(build);
        build->pending_editions--;
    }
    return 1;
}

int build_add_joker_offer(PlayerBuild *build, JokerType type, JokerEdition edition, int price) {
    JokerInstance *joker;
    JokerEdition previous_edition;

    if (build->joker_count >= MAX_JOKERS) return 0;
    if (build->joker_count >= build->joker_capacity && edition != EDITION_NEGATIVE) return 0;

    init_joker_instance(&build->jokers[build->joker_count], type);
    build->jokers[build->joker_count].sell_value = 2;
    build->joker_count++;
    while (build->pending_editions > 0) {
        apply_random_free_edition(build);
        build->pending_editions--;
    }
    joker = &build->jokers[build->joker_count - 1];
    previous_edition = joker->edition;
    if (previous_edition == EDITION_NEGATIVE && edition != EDITION_NEGATIVE) build->joker_capacity--;
    if (previous_edition != EDITION_NEGATIVE && edition == EDITION_NEGATIVE) build->joker_capacity++;
    joker->edition = edition;
    joker->sell_value = max_int(1, price / 2);
    return 1;
}

int build_add_tarot(PlayerBuild *build, TarotType type) {
    if (build->tarot_count >= build->tarot_capacity || build->tarot_count >= MAX_TAROTS) return 0;
    build->tarot_inventory[build->tarot_count++] = type;
    return 1;
}

int build_add_coupon(PlayerBuild *build, CouponType type) {
    int family = coupon_family_from_type(type);
    int level = coupon_level_from_type(type);
    if (build->coupon_levels[family] >= level) return 0;
    if (build->coupon_count >= MAX_COUPONS) return 0;
    build->coupons[build->coupon_count++] = type;
    build->coupon_levels[family] = level;
    if (type == COUPON_ORACLE_PLUS) build->tarot_capacity += 2;
    else if (type == COUPON_ORACLE) build->tarot_capacity += 1;
    return 1;
}

int build_sell_joker(PlayerBuild *build, int index) {
    int sell_price;

    if (index < 0 || index >= build->joker_count) return 0;

    sell_price = build->jokers[index].sell_value;
    if (build->jokers[index].edition == EDITION_NEGATIVE) build->joker_capacity--;
    build->jokers[index].active = 0;
    compact_jokers(build);
    build->coins += sell_price;

    return sell_price;
}

void compact_jokers(PlayerBuild *build) {
    int write = 0;
    int negatives = 0;
    for (int i = 0; i < build->joker_count; i++) {
        if (!build->jokers[i].active || build->jokers[i].type == JOKER_NONE) continue;
        if (write != i) build->jokers[write] = build->jokers[i];
        if (build->jokers[write].edition == EDITION_NEGATIVE) negatives++;
        write++;
    }
    for (int i = write; i < MAX_JOKERS; i++) init_joker_instance(&build->jokers[i], JOKER_NONE);
    build->joker_count = write;
    build->joker_capacity = BASE_JOKER_SLOTS + negatives;
}

void apply_random_free_edition(PlayerBuild *build) {
    if (build->joker_count <= 0) {
        build->pending_editions++;
        return;
    }

    int target = rand() % build->joker_count;
    JokerEdition edition = roll_shop_edition(build);
    if (edition == EDITION_NONE) edition = EDITION_FOIL;

    if (build->jokers[target].edition == EDITION_NEGATIVE && edition != EDITION_NEGATIVE) build->joker_capacity--;
    if (build->jokers[target].edition != EDITION_NEGATIVE && edition == EDITION_NEGATIVE) build->joker_capacity++;

    build->jokers[target].edition = edition;
    build->jokers[target].sell_value += joker_edition_extra_cost(edition) / 2;
    printf("Pergunta correta: %s recebeu a marca %s gratis.\n",
           joker_name(build->jokers[target].type), joker_edition_name(edition));
}

void print_build(const PlayerBuild *build) {
    printf("Moedas: %d | Slots de Coringa: %d/%d | Tarots: %d/%d | Cupons liberados: %d\n",
           build->coins, build->joker_count, build->joker_capacity, build->tarot_count,
           build->tarot_capacity, build->coupon_tokens);

    if (build->coupon_count > 0) {
        printf("Cupons ativos:\n");
        for (int i = 0; i < build->coupon_count; i++) {
            printf("  %d) %s\n", i + 1, coupon_name(build->coupons[i]));
        }
    }
    if (build->joker_count > 0) {
        printf("Coringas ativos:\n");
        for (int i = 0; i < build->joker_count; i++) {
            printf("  %d) [%s] %s\n", i + 1, joker_edition_name(build->jokers[i].edition),
                   joker_name(build->jokers[i].type));
        }
    }
    if (build->tarot_count > 0) {
        printf("Tarots no inventario:\n");
        for (int i = 0; i < build->tarot_count; i++) {
            printf("  %d) %s\n", i + 1, tarot_name(build->tarot_inventory[i]));
        }
    }
}

static void make_random_shop_slot(ShopSlot *slot, const PlayerBuild *build) {
    int joker_weight = 20;
    int tarot_weight = 4;
    if (build->coupon_levels[2] == 1) tarot_weight = 8;
    else if (build->coupon_levels[2] >= 2) tarot_weight = 16;

    int roll = rand() % (joker_weight + tarot_weight);
    if (roll < joker_weight) {
        slot->kind = SHOP_SLOT_JOKER;
        slot->joker = random_joker();
        slot->edition = roll_shop_edition(build);
        slot->price = rarity_price(joker_rarity(slot->joker)) + joker_edition_extra_cost(slot->edition);
        slot->price = shop_discounted_price(slot->price, build);
        slot->tarot = TAROT_NONE;
    } else {
        slot->kind = SHOP_SLOT_TAROT;
        slot->tarot = random_tarot();
        slot->price = shop_discounted_price(3, build);
        slot->joker = JOKER_NONE;
        slot->edition = EDITION_NONE;
    }
}

static void refresh_shop_state(ShopState *state, const PlayerBuild *build, int full_refresh) {
    for (int i = 0; i < 2; i++) {
        if (full_refresh || state->slots[i].kind == SHOP_SLOT_EMPTY) make_random_shop_slot(&state->slots[i], build);
    }
}

static int add_joker_with_price(PlayerBuild *build, JokerType type, JokerEdition edition, int price) {
    return build_add_joker_offer(build, type, edition, price);
}

static void buy_from_random_slot(PlayerBuild *build, ShopState *state, int index) {
    if (index < 0 || index > 1) return;
    ShopSlot *slot = &state->slots[index];
    if (slot->kind == SHOP_SLOT_EMPTY) {
        printf("Esse espaco da loja esta vazio.\n");
        return;
    }
    if (build->coins < slot->price) {
        printf("Moedas insuficientes.\n");
        return;
    }

    if (slot->kind == SHOP_SLOT_JOKER) {
        if (!add_joker_with_price(build, slot->joker, slot->edition, slot->price)) {
            printf("Limite de coringas atingido.\n");
            return;
        }
        printf("Comprado: [%s] %s\n", joker_edition_name(slot->edition), joker_name(slot->joker));
    } else {
        if (!build_add_tarot(build, slot->tarot)) {
            printf("Inventario de Tarot cheio.\n");
            return;
        }
        printf("Comprado: %s\n", tarot_name(slot->tarot));
    }

    build->coins -= slot->price;
    slot->kind = SHOP_SLOT_EMPTY;
}

static void buy_coupon(PlayerBuild *build, ShopState *state) {
    if (!state->coupon_available) {
        printf("O Cupom desta loja ja foi comprado.\n");
        return;
    }
    if (build->coupon_tokens <= 0) {
        printf("Nenhum Cupom foi liberado pelas perguntas ainda.\n");
        return;
    }
    if (build->coins < 10) {
        printf("Moedas insuficientes.\n");
        return;
    }
    if (!build_add_coupon(build, state->coupon)) {
        printf("Voce ja possui esta versao de Cupom.\n");
        return;
    }

    build->coins -= 10;
    build->coupon_tokens--;
    state->coupon_available = 0;
    printf("Comprado: %s\n", coupon_name(state->coupon));
}

static void open_pack(PlayerBuild *build, ShopState *state) {
    if (!state->pack_available) {
        printf("O pacote desta loja ja foi aberto.\n");
        return;
    }
    if (build->coins < state->pack_cost) {
        printf("Moedas insuficientes para pacote.\n");
        return;
    }

    JokerType option_jokers[3];
    TarotType option_tarots[3];
    int is_joker[3];
    printf("\nPacote aberto. Escolha 1 opcao:\n");
    for (int i = 0; i < 3; i++) {
        is_joker[i] = rand() % 2;
        if (is_joker[i]) {
            option_jokers[i] = random_joker();
            printf("%d) %s\n", i + 1, joker_name(option_jokers[i]));
        } else {
            option_tarots[i] = random_arcana_pack_tarot();
            printf("%d) %s\n", i + 1, tarot_name(option_tarots[i]));
        }
    }

    int choice = read_menu_choice();
    if (choice < 1 || choice > 3) {
        printf("Opcao invalida. Pacote cancelado.\n");
        return;
    }

    if (is_joker[choice - 1]) {
        if (!build_add_joker(build, option_jokers[choice - 1])) {
            printf("Sem slot de coringa. Pacote cancelado sem custo.\n");
            return;
        }
    } else if (!build_add_tarot(build, option_tarots[choice - 1])) {
        printf("Inventario de Tarot cheio. Pacote cancelado sem custo.\n");
        return;
    }

    build->coins -= state->pack_cost;
    state->pack_available = 0;
    printf("Pacote comprado.\n");
}

static void sell_joker(PlayerBuild *build) {
    if (build->joker_count <= 0) {
        printf("Voce nao possui coringas para vender.\n");
        return;
    }
    print_build(build);
    printf("Escolha o coringa para vender.\n");
    int choice = read_menu_choice();
    if (choice < 1 || choice > build->joker_count) {
        printf("Opcao invalida.\n");
        return;
    }

    int idx = choice - 1;
    int sell_price = build->jokers[idx].sell_value;
    printf("Vendido: %s por %d moedas.\n", joker_name(build->jokers[idx].type), sell_price);
    build_sell_joker(build, idx);
}

static void reroll_shop(PlayerBuild *build, ShopState *state) {
    int cost = state->reroll_cost;
    if (build->coins < cost) {
        printf("Moedas insuficientes para atualizar a loja.\n");
        return;
    }
    build->coins -= cost;
    refresh_shop_state(state, build, 1);
    state->reroll_cost++;
    printf("Loja atualizada.\n");
}

void run_shop(PlayerBuild *build) {
    ShopState state;
    state.coupon = random_coupon_offer(build);
    state.coupon_available = 1;
    state.pack_available = 1;
    state.pack_cost = shop_discounted_price(4, build);
    state.reroll_cost = 5;
    for (int i = 0; i < 2; i++) state.slots[i].kind = SHOP_SLOT_EMPTY;
    refresh_shop_state(&state, build, 1);

    int running = 1;
    while (running) {
        printf("\n========== LOJA ==========\n");
        print_build(build);

        for (int i = 0; i < 2; i++) {
            if (state.slots[i].kind == SHOP_SLOT_JOKER) {
                printf("%d) Slot %d: [%s] %s (%d moedas)\n", i + 1, i + 1,
                       joker_edition_name(state.slots[i].edition), joker_name(state.slots[i].joker),
                       state.slots[i].price);
            } else if (state.slots[i].kind == SHOP_SLOT_TAROT) {
                printf("%d) Slot %d: %s (%d moedas)\n", i + 1, i + 1,
                       tarot_name(state.slots[i].tarot), state.slots[i].price);
            } else {
                printf("%d) Slot %d: vazio\n", i + 1, i + 1);
            }
        }

        if (state.pack_available) printf("3) Abrir pacote (%d moedas)\n", state.pack_cost);
        else printf("3) Pacote esgotado\n");

        if (state.coupon_available) printf("4) Comprar Cupom (%d moedas): %s\n", 10, coupon_name(state.coupon));
        else printf("4) Cupom esgotado\n");

        printf("5) Atualizar loja (%d moedas)\n", state.reroll_cost);
        printf("6) Vender coringa\n");
        printf("0) Continuar run\n");

        switch (read_menu_choice()) {
            case 1:
                buy_from_random_slot(build, &state, 0);
                break;
            case 2:
                buy_from_random_slot(build, &state, 1);
                break;
            case 3:
                open_pack(build, &state);
                break;
            case 4:
                buy_coupon(build, &state);
                break;
            case 5:
                reroll_shop(build, &state);
                break;
            case 6:
                sell_joker(build);
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    }
}
