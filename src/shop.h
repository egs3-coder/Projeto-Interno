#ifndef SHOP_H
#define SHOP_H

#include "cards.h"

#define MAX_JOKERS 5
#define MAX_TAROTS 10
#define PACK_PREVIEW_SIZE 15

typedef enum {
    RARITY_COMMON = 0,
    RARITY_UNCOMMON,
    RARITY_RARE,
    RARITY_LEGENDARY
} CardRarity;

typedef enum {
    EDITION_NONE = 0,
    EDITION_FOIL,
    EDITION_HOLOGRAPHIC,
    EDITION_POLYCHROME,
    EDITION_NEGATIVE
} EditionType;

typedef enum {
    TAROT_CTX_ANYTIME = 1,
    TAROT_CTX_PHASE = 2,
    TAROT_CTX_PACK = 4
} TarotUseContext;

typedef enum {
    JOKER_NONE = 0,
    JOKER_FLAT,
    JOKER_PAIR,
    JOKER_FLUSH,
    JOKER_STRAIGHT,
    JOKER_ECONOMY,
    JOKER_PRISM,
    JOKER_ROYAL
} JokerType;

typedef enum {
    TAROT_NONE = 0,
    TAROT_PERSISTENCE,
    TAROT_FORCE,
    TAROT_TEMPERANCE,
    TAROT_HERMIT,
    TAROT_STAR,
    TAROT_MOON,
    TAROT_SUN,
    TAROT_WORLD,
    TAROT_STRENGTH,
    TAROT_DEATH,
    TAROT_WILD,
    TAROT_SEAL,
    TAROT_ECHO
} TarotType;

typedef struct {
    JokerType type;
    CardRarity rarity;
    EditionType edition;
} OwnedJoker;

typedef struct {
    int hands_played;
    int cards_discarded;
    int cards_bought;
    int tarots_used;
    int packs_opened;
    int shop_visits;
    int interest_earned;
    int discoveries;
    int phases_cleared;
    int best_hand_score;
    HandType best_hand_type;
    int hand_play_counts[HAND_STRAIGHT_FLUSH + 1];
} RunStats;

typedef struct {
    int *hands_left;
    int *next_play_bonus;
} TarotPhaseHook;

typedef struct {
    int coins;
    int quiz_hits;
    int coupons;
    OwnedJoker jokers[MAX_JOKERS];
    int joker_count;
    TarotType tarot_inventory[MAX_TAROTS];
    int tarot_count;
    Card deck_cards[MAX_DECK_CARDS];
    int deck_size;
    int seen_jokers[JOKER_ROYAL + 1];
    int seen_tarots[TAROT_ECHO + 1];
    RunStats stats;
} PlayerBuild;

void build_init(PlayerBuild *build);
const char *rarity_name(CardRarity rarity);
const char *edition_name(EditionType edition);
const char *joker_name(JokerType type);
const char *joker_description(JokerType type);
const char *tarot_name(TarotType type);
const char *tarot_description(TarotType type);
const char *hand_type_name(HandType type);
int joker_sell_value(const OwnedJoker *joker);
void print_build(const PlayerBuild *build);
void run_shop(PlayerBuild *build);
int try_use_tarot_inventory(PlayerBuild *build, int context_mask, Card *cards[], int card_count, TarotPhaseHook *hook);
OwnedJoker roll_random_joker(void);
TarotType random_tarot_general(void);
TarotType random_tarot_for_pack(void);
void register_discovery_joker(PlayerBuild *build, JokerType type);
void register_discovery_tarot(PlayerBuild *build, TarotType type);

#endif
