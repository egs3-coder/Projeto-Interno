#ifndef SHOP_H
#define SHOP_H

#define MAX_JOKERS 16
#define BASE_JOKER_SLOTS 5
#define MAX_TAROTS 16
#define MAX_COUPONS 12
#define COUPON_FAMILY_COUNT 4
#define MAX_HAND_LEVEL_TRACK 9

typedef enum {
    EDITION_NONE = 0,
    EDITION_FOIL,
    EDITION_CHROME,
    EDITION_PRISMATIC,
    EDITION_NEGATIVE
} JokerEdition;

typedef enum {
    JOKER_NONE = 0,
    JOKER_FLAT,
    JOKER_PAIR,
    JOKER_TWO_PAIR,
    JOKER_THREE,
    JOKER_FOUR,
    JOKER_STRAIGHT,
    JOKER_FLUSH,
    JOKER_COIN_PAIR,
    JOKER_COIN_FLUSH,
    JOKER_COIN_THREE,
    JOKER_COIN_ROYAL,
    JOKER_COIN_LOW,
    JOKER_COIN_BOSS,
    JOKER_POPCORN,
    JOKER_ICE_CREAM,
    JOKER_CAVENDISH,
    JOKER_COFFEE,
    JOKER_LUNCHBOX,
    JOKER_MARKER,
    JOKER_ROYAL_KING,
    JOKER_ROYAL_QUEEN,
    JOKER_ROYAL_JACK,
    JOKER_ROYAL_COUNCIL,
    JOKER_THRONE,
    JOKER_NOBLE_LINEAGE,
    JOKER_ARCANE_MINOR,
    JOKER_SIXTH_SENSE,
    JOKER_OCCULT_LIBRARY,
    JOKER_RITUAL_TABLE,
    JOKER_ECHO_ARCANO,
    JOKER_STENCIL,
    JOKER_GREEDY,
    JOKER_LOVELY,
    JOKER_WRATHFUL,
    JOKER_ASTUTE,
    JOKER_MIRROR_QUIZ,
    JOKER_BOSS_SHIELD,
    JOKER_CRYPTID_RELAY,
    JOKER_COSMOS_PRISM,
    JOKER_LUCKY_JIMBO,
    JOKER_FAMILIAR_WAGE,
    JOKER_PI_CACHE,
    JOKER_RED_CARD
} JokerType;

typedef enum {
    TAROT_NONE = 0,
    TAROT_FOOL,
    TAROT_MAGICIAN,
    TAROT_HIGH_PRIESTESS,
    TAROT_EMPRESS,
    TAROT_EMPEROR,
    TAROT_HIEROPHANT,
    TAROT_LOVERS,
    TAROT_CHARIOT,
    TAROT_JUSTICE,
    TAROT_HERMIT,
    TAROT_WHEEL,
    TAROT_STRENGTH,
    TAROT_HANGED_MAN,
    TAROT_DEATH,
    TAROT_TEMPERANCE,
    TAROT_DEVIL,
    TAROT_TOWER,
    TAROT_STAR,
    TAROT_MOON,
    TAROT_SUN,
    TAROT_WORLD,
    TAROT_JUDGEMENT,
    TAROT_AURA,
    TAROT_WRAITH,
    TAROT_ANKH,
    TAROT_SOUL
} TarotType;

typedef enum {
    COUPON_CLEARANCE = 0,
    COUPON_CLEARANCE_PLUS,
    COUPON_GRABBER,
    COUPON_GRABBER_PLUS,
    COUPON_ORACLE,
    COUPON_ORACLE_PLUS,
    COUPON_HONE,
    COUPON_HONE_PLUS
} CouponType;

typedef struct {
    JokerType type;
    JokerEdition edition;
    int value;
    int counter;
    int sell_value;
    int active;
} JokerInstance;

typedef struct {
    int coins;
    int quiz_hits;
    int quiz_streak;
    int coupon_tokens;
    int tarot_capacity;
    int joker_capacity;
    int pending_editions;
    int question_rewards_taken;
    int coupon_levels[COUPON_FAMILY_COUNT];
    JokerInstance jokers[MAX_JOKERS];
    int joker_count;
    TarotType tarot_inventory[MAX_TAROTS];
    int tarot_count;
    TarotType last_used_tarot;
    CouponType coupons[MAX_COUPONS];
    int coupon_count;
    int hand_levels[MAX_HAND_LEVEL_TRACK];
    int hand_progress[MAX_HAND_LEVEL_TRACK];
} PlayerBuild;

void build_init(PlayerBuild *build);
const char *joker_name(JokerType type);
const char *joker_rarity_name(JokerType type);
const char *joker_edition_name(JokerEdition edition);
const char *tarot_name(TarotType type);
const char *coupon_name(CouponType type);
void print_build(const PlayerBuild *build);
void run_shop(PlayerBuild *build);
JokerType random_joker(void);
JokerType random_shop_joker(void);
JokerType random_rare_joker(void);
JokerType random_legendary_joker(void);
TarotType random_tarot(void);
TarotType random_arcana_pack_tarot(void);
CouponType random_coupon_offer(const PlayerBuild *build);
JokerEdition random_shop_edition(const PlayerBuild *build, int phase_number);
int joker_base_price(JokerType type);
int joker_edition_extra_cost(JokerEdition edition);
int shop_discounted_price(int price, const PlayerBuild *build);
int build_add_joker(PlayerBuild *build, JokerType type);
int build_add_joker_offer(PlayerBuild *build, JokerType type, JokerEdition edition, int price);
int build_add_tarot(PlayerBuild *build, TarotType type);
int build_add_coupon(PlayerBuild *build, CouponType type);
int build_sell_joker(PlayerBuild *build, int index);
void compact_jokers(PlayerBuild *build);
void apply_random_free_edition(PlayerBuild *build);

#endif
