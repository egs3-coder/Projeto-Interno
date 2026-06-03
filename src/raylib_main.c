#include "cards.h"
#include "player_data.h"
#include "raylib_question_bank.h"
#include "shop.h"
#include "raylib.h"
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SCREEN_W 1600
#define SCREEN_H 900
#define MAX_PHASES 24
#define MAX_PACK_CHOICES 5
#define MAX_TAROT_SAMPLE 15
#define MAX_TOOLTIP 1024
#define MAX_PENDING_MARKS 12
#define MAX_QUIZ_LOG 64
#define GUI_ASSET_PATH_CAP 256
#define GUI_JOKER_TEXTURE_COUNT (JOKER_RED_CARD + 1)
#define GUI_TAROT_TEXTURE_COUNT (TAROT_JUDGEMENT + 1)
#define GUI_COUPON_TEXTURE_COUNT (COUPON_HONE_PLUS + 1)
#define GUI_EDITION_TEXTURE_COUNT (EDITION_NEGATIVE + 1)
#define GUI_CARD_ENH_TEXTURE_COUNT (ENHANCEMENT_WILD + 1)

typedef enum {
    SCREEN_TITLE = 0,
    SCREEN_PROFILE,
    SCREEN_LEADERBOARD,
    SCREEN_HISTORY,
    SCREEN_COLLECTION,
    SCREEN_RUN_INFO,
    SCREEN_PLAY,
    SCREEN_REWARD,
    SCREEN_QUIZ,
    SCREEN_ROUND_PREVIEW,
    SCREEN_SHOP,
    SCREEN_PACK,
    SCREEN_TAROT_APPLY,
    SCREEN_END
} ScreenId;

typedef enum {
    QUIZ_POST_PHASE = 0,
    QUIZ_BOSS,
    QUIZ_SKIP
} QuizMode;

typedef enum {
    OFFER_NONE = 0,
    OFFER_JOKER,
    OFFER_TAROT,
    OFFER_COUPON,
    OFFER_PACK
} OfferType;

typedef enum {
    COLLECTION_JOKERS = 0,
    COLLECTION_COUPONS,
    COLLECTION_MARKS,
    COLLECTION_BOSSES,
    COLLECTION_TAROTS,
    COLLECTION_TAB_COUNT
} CollectionTab;

typedef enum {
    PACK_BUFFOON_3_1 = 0,
    PACK_BUFFOON_5_1,
    PACK_BUFFOON_5_2,
    PACK_ARCANA_3_1,
    PACK_ARCANA_5_1,
    PACK_ARCANA_5_2
} PackKind;

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
    int hands_played_this_phase;
} PhaseState;

typedef struct {
    HandEval eval;
    int points;
    int coins;
    int lucky_triggered;
    int enhanced_count;
} PlayOutcome;

typedef RayQuizQuestion Question;

typedef struct {
    int queued;
    float phase;
} PulseAnim;

typedef struct {
    int valid;
    int selected_count;
    HandEval eval;
    double chips;
    double mult;
    double xmult;
    int points;
    int coins;
} SelectionPreview;

typedef struct {
    int card_hits[MAX_PLAY];
    int joker_hits[MAX_JOKERS];
    int counter_hits[4];
} ActivationTrace;

typedef struct {
    OfferType type;
    JokerType joker;
    JokerEdition edition;
    TarotType tarot;
    CouponType coupon;
    PackKind pack_kind;
    int price;
    int sold;
} Offer;

typedef struct {
    Offer slots[2];
    Offer voucher;
    Offer packs[2];
    int reroll_cost;
} ShopState;

typedef struct {
    int active;
    PackKind kind;
    Offer choices[MAX_PACK_CHOICES];
    int choice_count;
    int picks_remaining;
} PackState;

typedef struct {
    int active;
    TarotType tarot;
    int from_inventory;
    int inventory_index;
    int target_count_required;
    int target_count_selected;
    int use_sample_deck;
    int sample_count;
    Card sample_cards[MAX_TAROT_SAMPLE];
    int sample_master_indices[MAX_TAROT_SAMPLE];
    int selected[MAX_TAROT_SAMPLE];
    ScreenId return_screen;
} TarotApplyState;

typedef struct {
    int active;
    int base_reward;
    int hand_bonus;
    int interest;
    int joker_bonus;
    int total;
    float anim;
    ScreenId next_screen;
    QuizMode next_quiz_mode;
    char blind_name[32];
} RewardState;

typedef struct {
    QuizMode mode;
    int phase_number;
    int bet;
    int blind;
    int attempts;
    int correct;
    char title[64];
    char category[48];
    char difficulty[32];
    char question[180];
    char hint[160];
} QuizLogEntry;

typedef enum {
    SKIP_REWARD_MARK = 0,
    SKIP_REWARD_HAND_SIZE,
    SKIP_REWARD_FREE_PACKS,
    SKIP_REWARD_MONEY,
    SKIP_REWARD_FREE_REROLLS
} SkipRewardType;

typedef struct {
    SkipRewardType type;
    int amount;
    JokerEdition mark;
    char label[96];
} SkipReward;

typedef struct {
    Texture2D cards[4][14];
    Texture2D enhanced[GUI_CARD_ENH_TEXTURE_COUNT];
    Texture2D jokers[GUI_JOKER_TEXTURE_COUNT];
    Texture2D editions[GUI_EDITION_TEXTURE_COUNT];
    Texture2D tarots[GUI_TAROT_TEXTURE_COUNT];
    Texture2D coupons[GUI_COUPON_TEXTURE_COUNT];
    Texture2D packs[PACK_ARCANA_5_2 + 1];
    Texture2D card_sheet;
    Texture2D joker_sheet;
    Texture2D tarot_sheet;
    Texture2D coupon_sheet;
    Texture2D pack_sheet;
    Texture2D villain;
    int loaded;
} AssetPack;

typedef struct AppState AppState;

static void update_temp_jokers_after_phase(PlayerBuild *build);
static void apply_hand_overrides(PhaseState *phase, HandEval *eval, int count);
static Color rgb(int r, int g, int b);
static Rectangle Rf(float x, float y, float w, float h);

typedef struct {
    int best_hand_score;
    HandType best_hand_type;
    int total_points;
    int hand_play_counts[MAX_HAND_LEVEL_TRACK];
    int hands_played;
    int cards_discarded;
    int cards_bought;
    int rerolls;
    int packs_opened;
    int tarots_used;
    int phases_cleared;
    int antes_cleared;
    int quiz_answer_counts[4];
    int quiz_correct_answers;
    int joker_use_counts[GUI_JOKER_TEXTURE_COUNT];
    int tarot_use_counts[GUI_TAROT_TEXTURE_COUNT];
    char defeated_by[32];
    char run_code[16];
} RunStats;

typedef struct {
    int chips;
    int mult;
    int threshold_base;
} HandGrowth;

struct AppState {
    ScreenId screen;
    PlayerBuild build;
    int phase_number;
    int run_won;
    JokerEdition pending_marks[MAX_PENDING_MARKS];
    int pending_mark_count;
    JokerEdition phase_mark_reward;
    int phase_mark_available;
    Card master_deck[DECK_SIZE];
    Deck phase_deck;
    int phase_sources[DECK_SIZE];
    Card hand[HAND_SIZE];
    int hand_sources[HAND_SIZE];
    int hand_count;
    int hovered_card;
    int selected_joker_index;
    int selected_tarot_index;
    int dragging_card_index;
    int dragging_joker_index;
    int drag_started;
    Vector2 drag_start;
    Vector2 drag_offset;
    bool selected[HAND_SIZE];
    float select_anim[HAND_SIZE];
    float hover_anim[HAND_SIZE];
    PulseAnim hand_pulses[HAND_SIZE];
    PulseAnim joker_pulses[MAX_JOKERS];
    PulseAnim counter_pulses[4];
    PhaseState phase;
    ShopState shop;
    PackState pack;
    TarotApplyState tarot_apply;
    RewardState reward;
    QuizMode quiz_mode;
    Question current_question;
    SelectionPreview preview;
    RunStats stats;
    char active_player[PLAYER_NAME_CAP];
    char draft_player[PLAYER_NAME_CAP];
    char history_query[PLAYER_NAME_CAP];
    PlayerCollection collection;
    AssetPack assets;
    int question_mask;
    int draft_question_mask;
    int profile_focus;
    int history_focus;
    int leaderboard_mode;
    int collection_tab;
    int collection_page;
    int run_info_tab;
    ScreenId run_info_return_screen;
    QuizLogEntry quiz_log[MAX_QUIZ_LOG];
    int quiz_log_count;
    int pending_quiz_log_index;
    int pending_failed_question;
    Question failed_question;
    int current_quiz_log_index;
    int current_ante;
    int ante_played_blinds;
    int ante_correct_questions;
    int next_phase_hand_bonus;
    int free_pack_credits;
    int free_rerolls;
    SkipReward phase_skip_reward;
    int auto_start_after_profile;
    int last_run_saved;
    int last_rank_position;
    int last_rank_total;
    RunRecord last_record;
    char status[256];
    char tooltip[MAX_TOOLTIP];
};

static void set_status(AppState *app, const char *fmt, ...);
static int player_is_linked(const AppState *app);
static void prime_profile_from_active(AppState *app);
static void unlink_active_player(AppState *app);
static void handle_text_box_input(char *buffer, size_t size, int active);
static Question draw_random_question(const AppState *app, QuizMode mode);
static void save_current_run_record(AppState *app);
static int has_joker(const PlayerBuild *build, JokerType type);
static void open_run_info(AppState *app, ScreenId return_screen);
static void record_quiz_result(AppState *app, int answer, int correct);
static void draw_run_info_screen(AppState *app);
static void load_asset_pack(AssetPack *assets);
static void unload_asset_pack(AssetPack *assets);
static void draw_villain_card(AppState *app, Rectangle box, const char *speech, int angry);
static int rec_sum_int(const int *values, int count);
static int rec_min_int(const int *values, int count);
static int rec_max_int(const int *values, int count);
static double rec_sum_sq_diff_int(const int *values, int count, double mean);
static void load_active_collection(AppState *app);
static void save_active_collection(const AppState *app);
static void unlock_collection_joker(AppState *app, JokerType joker);
static void unlock_collection_coupon(AppState *app, CouponType coupon);
static void unlock_collection_mark(AppState *app, JokerEdition edition);
static void unlock_collection_boss(AppState *app, int phase_number);
static void unlock_collection_tarot(AppState *app, TarotType tarot);

static AssetPack *g_assets = NULL;
static Rectangle g_render_view = {0};
static float g_render_scale = 1.0f;

static const HandGrowth HAND_GROWTH[MAX_HAND_LEVEL_TRACK] = {
    {6, 1, 4},
    {8, 1, 4},
    {10, 1, 5},
    {12, 1, 5},
    {14, 1, 6},
    {12, 1, 6},
    {15, 1, 7},
    {18, 1, 8},
    {20, 1, 10}
};

static Color rgb(int r, int g, int b) {
    return (Color){(unsigned char)r, (unsigned char)g, (unsigned char)b, 255};
}

static Rectangle Rf(float x, float y, float w, float h) {
    return (Rectangle){x, y, w, h};
}

static void update_render_layout(void) {
    float sw = (float)GetScreenWidth();
    float sh = (float)GetScreenHeight();
    float scale_x = sw / (float)SCREEN_W;
    float scale_y = sh / (float)SCREEN_H;
    g_render_scale = scale_x < scale_y ? scale_x : scale_y;
    if (g_render_scale <= 0.0f) g_render_scale = 1.0f;
    g_render_view.width = (float)SCREEN_W * g_render_scale;
    g_render_view.height = (float)SCREEN_H * g_render_scale;
    g_render_view.x = (sw - g_render_view.width) * 0.5f;
    g_render_view.y = (sh - g_render_view.height) * 0.5f;
}

static Vector2 virtual_mouse(void) {
    Vector2 mouse = GetMousePosition();
    return (Vector2){
        (mouse.x - g_render_view.x) / g_render_scale,
        (mouse.y - g_render_view.y) / g_render_scale
    };
}

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static int min_int(int a, int b) {
    return a < b ? a : b;
}

static int texture_ready(Texture2D texture) {
    return texture.id != 0;
}

static void draw_texture_fit(Texture2D texture, Rectangle box, Color tint) {
    Rectangle src;
    if (!texture_ready(texture)) return;
    src = Rf(0, 0, (float)texture.width, (float)texture.height);
    DrawTexturePro(texture, src, box, (Vector2){0, 0}, 0.0f, tint);
}

static void draw_texture_region_fit(Texture2D texture, Rectangle src, Rectangle box, Color tint) {
    if (!texture_ready(texture)) return;
    DrawTexturePro(texture, src, box, (Vector2){0, 0}, 0.0f, tint);
}

static Texture2D load_texture_if_exists(const char *path) {
    Texture2D texture = {0};
    if (path && FileExists(path)) texture = LoadTexture(path);
    return texture;
}

static Rectangle sheet_rect(int column, int row, float cell_width, float cell_height) {
    return Rf(column * cell_width, row * cell_height, cell_width, cell_height);
}

static Rectangle card_sheet_src(int suit, int rank) {
    static const int suit_rows[4] = {0, 2, 3, 1};
    return sheet_rect(rank - 1, suit_rows[suit], 142.0f, 190.0f);
}

static int joker_sheet_index(JokerType joker) {
    switch (joker) {
        case JOKER_FLAT:          return 1;   /* Coringa */
        case JOKER_GREEDY:        return 17;  /* Cor. Ganancioso */
        case JOKER_LOVELY:        return 18;  /* Cor. Vigoroso */
        case JOKER_WRATHFUL:      return 19;  /* Coringa Irado */
        case JOKER_ASTUTE:        return 20;  /* Cor. Guloso */
        case JOKER_PAIR:          return 3;   /* Coringa Alegre */
        case JOKER_THREE:         return 3;   /* Coringa Bobo (mesma sprite) */
        case JOKER_STRAIGHT:      return 6;   /* Coringa Maluco */
        case JOKER_FLUSH:         return 7;   /* Cor. Engracado */
        case JOKER_FOUR:          return 9;   /* Cor. Bizonho */
        case JOKER_ROYAL_KING:    return 33;  /* Cara Assustadora */
        case JOKER_ROYAL_QUEEN:   return 64;  /* Academico */
        case JOKER_SIXTH_SENSE:   return 67;  /* Quatro Dedos */
        case JOKER_ARCANE_MINOR:  return 124; /* Atalho */
        case JOKER_THRONE:        return 107; /* Respingo */
        case JOKER_ROYAL_COUNCIL: return 37;  /* Pareidolia */
        case JOKER_OCCULT_LIBRARY:return 65;  /* Cor. Borrado */
        case JOKER_COIN_BOSS:     return 70;  /* Comprovante */
        case JOKER_NOBLE_LINEAGE: return 15;  /* Mimico */
        case JOKER_ROYAL_JACK:    return 14;  /* Meias e Bucins */
        case JOKER_LUNCHBOX:      return 75;  /* Anoitecer */
        case JOKER_COFFEE:        return 154; /* Agua com Gas */
        case JOKER_COIN_FLUSH:    return 30;  /* Cor. Dourado */
        case JOKER_COIN_PAIR:     return 42;  /* Cartao de Visitas */
        case JOKER_MIRROR_QUIZ:   return 139; /* Ate a Lua */
        case JOKER_STENCIL:       return 16;  /* Cartao de Credito */
        case JOKER_COIN_THREE:    return 129; /* Foguete */
        case JOKER_COIN_ROYAL:    return 137; /* Estac. Reservado */
        case JOKER_COIN_LOW:      return 68;  /* Banana */
        case JOKER_CAVENDISH:     return 116; /* Banana Cavendish */
        case JOKER_POPCORN:       return 152; /* Pipoca */
        case JOKER_ICE_CREAM:     return 105; /* Sorvete */
        case JOKER_LUCKY_JIMBO:   return 113; /* Cor. Verde */
        case JOKER_ECHO_ARCANO:   return 104; /* Corredor */
        case JOKER_MARKER:        return 58;  /* Vidente */
        case JOKER_RITUAL_TABLE:  return 110; /* Constelacao */
        case JOKER_TWO_PAIR:      return 155; /* Calcas Extras */
        case JOKER_BOSS_SHIELD:   return 122; /* Pequeno Coringa */
        case JOKER_CRYPTID_RELAY: return 31;  /* Projeto */
        case JOKER_COSMOS_PRISM:  return 78;  /* Tempest. Ideias */
        case JOKER_FAMILIAR_WAGE: return 25;  /* Gabarito */
        case JOKER_RED_CARD:      return 118; /* Cartao Vermelho */
        case JOKER_PI_CACHE:      return 103; /* Questao de Prova */
        default: return 0;
    }
}

static Rectangle joker_sheet_src(JokerType joker) {
    int index = joker_sheet_index(joker) - 1;
    return sheet_rect(index % 10, index / 10, 142.0f, 190.0f);
}

static int tarot_sheet_index(TarotType tarot) {
    switch (tarot) {
        case TAROT_FOOL: return 1;
        case TAROT_MAGICIAN: return 2;
        case TAROT_HIGH_PRIESTESS: return 3;
        case TAROT_EMPRESS: return 4;
        case TAROT_EMPEROR: return 5;
        case TAROT_HIEROPHANT: return 6;
        case TAROT_LOVERS: return 7;
        case TAROT_CHARIOT: return 8;
        case TAROT_JUSTICE: return 9;
        case TAROT_HERMIT: return 10;
        case TAROT_WHEEL: return 11;
        case TAROT_STRENGTH: return 12;
        case TAROT_HANGED_MAN: return 13;
        case TAROT_DEATH: return 14;
        case TAROT_TEMPERANCE: return 15;
        case TAROT_DEVIL: return 16;
        case TAROT_TOWER: return 17;
        case TAROT_STAR: return 18;
        case TAROT_MOON: return 19;
        case TAROT_SUN: return 20;
        case TAROT_JUDGEMENT: return 21;
        case TAROT_WORLD: return 22;
        case TAROT_SOUL: return 23;
        case TAROT_AURA: return 40;
        case TAROT_WRAITH: return 41;
        case TAROT_ANKH: return 46;
        default: return 0;
    }
}

static Rectangle tarot_sheet_src(TarotType tarot) {
    int index = tarot_sheet_index(tarot) - 1;
    return sheet_rect(index % 10, index / 10, 142.0f, 190.0f);
}

static int coupon_sheet_index(CouponType coupon) {
    switch (coupon) {
        case COUPON_CLEARANCE:      return 4;   /* Liquidacao */
        case COUPON_CLEARANCE_PLUS: return 13;  /* Liquidacao Total */
        case COUPON_GRABBER:        return 6;   /* Mao Longa */
        case COUPON_GRABBER_PLUS:   return 15;  /* Lingua de Nacho */
        case COUPON_ORACLE:         return 2;   /* Comerc. de Tarot */
        case COUPON_ORACLE_PLUS:    return 11;  /* Magnata do Tarot */
        case COUPON_HONE:           return 5;   /* Polimento */
        case COUPON_HONE_PLUS:      return 14;  /* Brilho Intenso */
        default: return 0;
    }
}

static Rectangle coupon_sheet_src(CouponType coupon) {
    int index = coupon_sheet_index(coupon) - 1;
    return sheet_rect(index % 9, index / 9, 142.0f, 190.0f);
}

static Rectangle pack_sheet_src(PackKind kind) {
    int index;
    switch (kind) {
        case PACK_BUFFOON_3_1: index = 33; break;
        case PACK_BUFFOON_5_1: index = 35; break;
        case PACK_BUFFOON_5_2: index = 36; break;
        case PACK_ARCANA_3_1: index = 1; break;
        case PACK_ARCANA_5_1: index = 9; break;
        case PACK_ARCANA_5_2: index = 11; break;
        default: index = 0; break;
    }
    if (index <= 0) return Rf(0, 0, 0, 0);
    index--;
    return sheet_rect(index % 4, index / 4, 142.0f, 190.0f);
}

static int texture_can_cover(Texture2D texture, Rectangle src) {
    return texture_ready(texture) &&
           src.x >= 0.0f && src.y >= 0.0f &&
           src.x + src.width <= (float)texture.width &&
           src.y + src.height <= (float)texture.height;
}

static const char *asset_suit_slug(int suit) {
    switch (suit) {
        case SUIT_COPAS: return "copas";
        case SUIT_OUROS: return "ouros";
        case SUIT_ESPADAS: return "espadas";
        case SUIT_PAUS: return "paus";
        default: return "naipe";
    }
}

static const char *asset_rank_slug(int rank) {
    switch (rank) {
        case 1: return "A";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: return TextFormat("%d", rank);
    }
}

static const char *gui_edition_name(JokerEdition edition) {
    switch (edition) {
        case EDITION_FOIL: return "Foil";
        case EDITION_CHROME: return "Holografico";
        case EDITION_PRISMATIC: return "Policromado";
        case EDITION_NEGATIVE: return "Negative";
        default: return "Normal";
    }
}

static const char *gui_mark_label(JokerEdition edition) {
    switch (edition) {
        case EDITION_FOIL: return "Foil";
        case EDITION_CHROME: return "Holo";
        case EDITION_PRISMATIC: return "Poli";
        case EDITION_NEGATIVE: return "Neg";
        default: return "Base";
    }
}

static Color mark_color(JokerEdition edition) {
    switch (edition) {
        case EDITION_FOIL: return rgb(239, 245, 64);
        case EDITION_CHROME: return rgb(83, 188, 255);
        case EDITION_PRISMATIC: return rgb(255, 75, 222);
        case EDITION_NEGATIVE: return rgb(119, 67, 255);
        default: return WHITE;
    }
}

static const char *mark_desc(JokerEdition edition) {
    switch (edition) {
        case EDITION_FOIL: return "+50 chips fixos para este Coringa.";
        case EDITION_CHROME: return "+4 mult fixos para este Coringa.";
        case EDITION_PRISMATIC: return "x1.5 mult final para este Coringa.";
        case EDITION_NEGATIVE: return "+1 slot de Coringa enquanto estiver ativo.";
        default: return "Sem marca especial.";
    }
}

static int color_is_bright(Color color) {
    int luminance = color.r * 299 + color.g * 587 + color.b * 114;
    return luminance >= 150000;
}

static Color button_text_color(Color fill) {
    return color_is_bright(fill) ? rgb(33, 36, 42) : WHITE;
}

static void draw_edition_badge(Rectangle box, JokerEdition edition) {
    Rectangle badge;
    Color fill;
    Color text;
    const char *label;

    if (edition <= EDITION_NONE) return;
    fill = mark_color(edition);
    text = button_text_color(fill);
    label = gui_mark_label(edition);
    badge = Rf(box.x + box.width - 58, box.y + 8, 48, 22);
    DrawRectangleRounded(badge, 0.35f, 8, Fade(fill, 0.95f));
    DrawRectangleRoundedLinesEx(badge, 0.35f, 8, 2.0f, Fade(WHITE, 0.75f));
    DrawText(label, (int)badge.x + 6, (int)badge.y + 4, 12, text);
}

static int collection_total_jokers(void) {
    return JOKER_RED_CARD;
}

static int collection_total_coupons(void) {
    return COUPON_HONE_PLUS + 1;
}

static int collection_total_marks(void) {
    return 4;
}

static int collection_total_bosses(void) {
    return 8;
}

static int collection_total_tarots(void) {
    return TAROT_SOUL;
}

static int collection_coupon_index(CouponType coupon) {
    return (int)coupon + 1;
}

static int collection_mark_index(JokerEdition edition) {
    switch (edition) {
        case EDITION_FOIL: return 1;
        case EDITION_CHROME: return 2;
        case EDITION_PRISMATIC: return 3;
        case EDITION_NEGATIVE: return 4;
        default: return 0;
    }
}

static int collection_boss_index(int phase_number) {
    int ante = (phase_number - 1) / 3;
    if (ante < 0) ante = 0;
    if (ante > 7) ante = 7;
    return ante + 1;
}

static const char *collection_tab_name(CollectionTab tab) {
    switch (tab) {
        case COLLECTION_JOKERS: return "Coringas";
        case COLLECTION_COUPONS: return "Cupons";
        case COLLECTION_MARKS: return "Marcas";
        case COLLECTION_BOSSES: return "Chefes";
        case COLLECTION_TAROTS: return "Tarots";
        default: return "Colecao";
    }
}

static int collection_tab_total(CollectionTab tab) {
    switch (tab) {
        case COLLECTION_JOKERS: return collection_total_jokers();
        case COLLECTION_COUPONS: return collection_total_coupons();
        case COLLECTION_MARKS: return collection_total_marks();
        case COLLECTION_BOSSES: return collection_total_bosses();
        case COLLECTION_TAROTS: return collection_total_tarots();
        default: return 0;
    }
}

static Rectangle scale_rect_center(Rectangle box, float scale) {
    float new_w = box.width * scale;
    float new_h = box.height * scale;
    return Rf(box.x + (box.width - new_w) * 0.5f,
              box.y + (box.height - new_h) * 0.5f,
              new_w, new_h);
}

static void update_pulse(PulseAnim *pulse, float speed) {
    if (pulse->queued <= 0) {
        pulse->phase = 0.0f;
        return;
    }
    pulse->phase += GetFrameTime() * speed;
    if (pulse->phase >= 1.0f) {
        pulse->phase = 0.0f;
        pulse->queued--;
    }
}

static void queue_pulse(PulseAnim *pulse, int amount) {
    if (amount <= 0) return;
    pulse->queued += amount;
    if (pulse->queued > 10) pulse->queued = 10;
}

static float pulse_scale(const PulseAnim *pulse, float amount) {
    if (pulse->queued <= 0) return 1.0f;
    return 1.0f + sinf(pulse->phase * PI) * amount;
}

static void update_all_pulses(AppState *app) {
    for (int i = 0; i < HAND_SIZE; i++) update_pulse(&app->hand_pulses[i], 5.8f);
    for (int i = 0; i < MAX_JOKERS; i++) update_pulse(&app->joker_pulses[i], 5.2f);
    for (int i = 0; i < 4; i++) update_pulse(&app->counter_pulses[i], 5.0f);
}

static JokerEdition random_reward_mark(const AppState *app) {
    int ante = app ? ((app->phase_number - 1) / 3) + 1 : 1;
    int roll = rand() % 100;
    int foil_cut = ante < 4 ? 55 : 50;
    int chrome_cut = foil_cut + 25;
    int prism_cut = chrome_cut + (ante < 4 ? 10 : 15);

    if (roll < foil_cut) return EDITION_FOIL;
    if (roll < chrome_cut) return EDITION_CHROME;
    if (roll < prism_cut) return EDITION_PRISMATIC;
    return EDITION_NEGATIVE;
}

static void queue_mark_reward(AppState *app, JokerEdition edition, const char *status_fmt) {
    if (app->pending_mark_count < MAX_PENDING_MARKS) {
        app->pending_marks[app->pending_mark_count++] = edition;
    }
    unlock_collection_mark(app, edition);
    if (status_fmt) set_status(app, status_fmt, gui_edition_name(edition));
}

static JokerEdition pop_pending_mark(AppState *app) {
    JokerEdition edition;
    if (app->pending_mark_count <= 0) return EDITION_NONE;
    edition = app->pending_marks[0];
    for (int i = 1; i < app->pending_mark_count; i++) app->pending_marks[i - 1] = app->pending_marks[i];
    app->pending_mark_count--;
    return edition;
}

static void schedule_phase_mark(AppState *app) {
    int roll;
    if (app->phase.boss) {
        app->phase_mark_available = 0;
        app->phase_mark_reward = EDITION_NONE;
        return;
    }
    app->phase_mark_available = 1;
    app->phase_mark_reward = random_reward_mark(app);
    memset(&app->phase_skip_reward, 0, sizeof(app->phase_skip_reward));
    roll = rand() % 100;
    if (roll < 35) {
        app->phase_skip_reward.type = SKIP_REWARD_MARK;
        app->phase_skip_reward.mark = app->phase_mark_reward;
        snprintf(app->phase_skip_reward.label, sizeof(app->phase_skip_reward.label), "Marca %s na loja", gui_edition_name(app->phase_mark_reward));
    } else if (roll < 52) {
        app->phase_skip_reward.type = SKIP_REWARD_HAND_SIZE;
        app->phase_skip_reward.amount = 3;
        snprintf(app->phase_skip_reward.label, sizeof(app->phase_skip_reward.label), "+3 tamanho de mao na proxima rodada");
    } else if (roll < 69) {
        app->phase_skip_reward.type = SKIP_REWARD_FREE_PACKS;
        app->phase_skip_reward.amount = 1 + (rand() % 2);
        snprintf(app->phase_skip_reward.label, sizeof(app->phase_skip_reward.label), "%d pacote(s) gratuito(s)", app->phase_skip_reward.amount);
    } else if (roll < 84) {
        app->phase_skip_reward.type = SKIP_REWARD_MONEY;
        app->phase_skip_reward.amount = 6 + ((app->phase_number - 1) / 3) * 2;
        snprintf(app->phase_skip_reward.label, sizeof(app->phase_skip_reward.label), "+$%d imediatos", app->phase_skip_reward.amount);
    } else {
        app->phase_skip_reward.type = SKIP_REWARD_FREE_REROLLS;
        app->phase_skip_reward.amount = 2;
        snprintf(app->phase_skip_reward.label, sizeof(app->phase_skip_reward.label), "%d atualizacoes gratis", app->phase_skip_reward.amount);
    }
    unlock_collection_mark(app, app->phase_mark_reward);
}

static void apply_skip_reward(AppState *app) {
    if (!app) return;
    switch (app->phase_skip_reward.type) {
        case SKIP_REWARD_MARK:
            if (app->phase_mark_available) {
                queue_mark_reward(app, app->phase_mark_reward, "Pergunta dificil vencida. Oferta futura marcada com %s.");
                app->phase_mark_available = 0;
            }
            break;
        case SKIP_REWARD_HAND_SIZE:
            app->next_phase_hand_bonus += app->phase_skip_reward.amount;
            set_status(app, "Pulo perfeito: +%d cartas de mao na proxima rodada.", app->phase_skip_reward.amount);
            break;
        case SKIP_REWARD_FREE_PACKS:
            app->free_pack_credits += app->phase_skip_reward.amount;
            set_status(app, "Pulo perfeito: %d pacote(s) gratuito(s) na loja.", app->phase_skip_reward.amount);
            break;
        case SKIP_REWARD_MONEY:
            app->build.coins += app->phase_skip_reward.amount;
            set_status(app, "Pulo perfeito: +$%d.", app->phase_skip_reward.amount);
            break;
        case SKIP_REWARD_FREE_REROLLS:
            app->free_rerolls += app->phase_skip_reward.amount;
            set_status(app, "Pulo perfeito: %d atualizacoes gratis na loja.", app->phase_skip_reward.amount);
            break;
    }
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

static int ante_base_goal(int ante) {
    static const int base_goals[] = {300, 800, 2000, 5000, 11000, 20000, 35000, 50000};
    if (ante < 1) ante = 1;
    if (ante <= 8) return base_goals[ante - 1];
    return 50000 + (ante - 8) * 65000;
}

static const char *blind_name_from_phase(int phase_number) {
    int kind = blind_kind_from_phase(phase_number);
    if (kind == 0) return "Small Blind";
    if (kind == 1) return "Big Blind";
    return "Blind Chefe";
}

static const char *blind_boss_name_from_phase(int phase_number) {
    static const char *names[] = {"Pausa", "Ruido", "Silencio", "Mar Revolto", "Nevoa", "Apagao", "Paulada", "Vazio"};
    int ante = (phase_number - 1) / 3;
    if (ante < 0) ante = 0;
    if (ante > 7) ante = 7;
    return names[ante];
}

static const char *blind_boss_desc_from_index(int index) {
    switch (index) {
        case 1: return "Falhar reduz 1 mao da fase.";
        case 2: return "Falhar reduz 1 descarte da fase.";
        case 3: return "Falhar aplica -16% na pontuacao final.";
        case 4: return "Falhar reduz 2 maos da fase.";
        case 5: return "Falhar aplica -24% na pontuacao final.";
        case 6: return "Falhar reduz 2 descartes da fase.";
        case 7: return "Falhar aplica -32% na pontuacao final.";
        case 8: return "Falhar reduz 1 mao, 1 descarte e aplica -12%.";
        default: return "Neutralize a pergunta para evitar o debuff.";
    }
}

static const char *blind_boss_desc_from_phase(int phase_number) {
    return blind_boss_desc_from_index(collection_boss_index(phase_number));
}

static void apply_boss_failure_effect(AppState *app) {
    int idx;
    int shielded;

    if (!app) return;
    idx = collection_boss_index(app->phase_number);
    shielded = has_joker(&app->build, JOKER_BOSS_SHIELD);

    switch (idx) {
        case 1:
            if (!shielded && app->phase.hands_left > 1) app->phase.hands_left--;
            break;
        case 2:
            if (!shielded && app->phase.discards_left > 0) app->phase.discards_left--;
            break;
        case 3:
            app->phase.debuff_percent = shielded ? 8 : 16;
            break;
        case 4:
            if (!shielded && app->phase.hands_left > 2) app->phase.hands_left -= 2;
            else if (!shielded && app->phase.hands_left > 1) app->phase.hands_left--;
            break;
        case 5:
            app->phase.debuff_percent = shielded ? 12 : 24;
            break;
        case 6:
            if (!shielded && app->phase.discards_left > 1) app->phase.discards_left -= 2;
            else if (!shielded && app->phase.discards_left > 0) app->phase.discards_left--;
            break;
        case 7:
            app->phase.debuff_percent = shielded ? 16 : 32;
            break;
        case 8:
            if (!shielded && app->phase.hands_left > 1) app->phase.hands_left--;
            if (!shielded && app->phase.discards_left > 0) app->phase.discards_left--;
            app->phase.debuff_percent = shielded ? 6 : 12;
            break;
        default:
            break;
    }
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

static const char *enhancement_desc(CardEnhancement enhancement) {
    switch (enhancement) {
        case ENHANCEMENT_BONUS: return "Bonus: +20 chips";
        case ENHANCEMENT_MULT: return "Multi: +4 mult";
        case ENHANCEMENT_LUCKY: return "Sorte: 25% de +20 mult";
        case ENHANCEMENT_GLASS: return "Vidro: x2 mult";
        case ENHANCEMENT_WILD: return "Coringa: conta como qualquer naipe";
        default: return "Carta padrao";
    }
}

static const char *joker_desc(JokerType type) {
    switch (type) {
        case JOKER_FLAT:          return "+4 Mult.";
        case JOKER_GREEDY:        return "Cada carta de Ouros pontuada concede +3 Mult.";
        case JOKER_LOVELY:        return "Cada carta de Copas pontuada concede +3 Mult.";
        case JOKER_WRATHFUL:      return "Cada carta de Espadas pontuada concede +3 Mult.";
        case JOKER_ASTUTE:        return "Cada carta de Paus pontuada concede +3 Mult.";
        case JOKER_PAIR:          return "Se a mao jogada for Par: +8 Mult.";
        case JOKER_THREE:         return "Se a mao jogada for Three of a Kind: +12 Mult.";
        case JOKER_STRAIGHT:      return "Se a mao jogada for Straight: +10 Mult.";
        case JOKER_FLUSH:         return "Se a mao jogada for Flush: +12 Mult.";
        case JOKER_FOUR:          return "Se a mao jogada for Full House: +10 Mult.";
        case JOKER_ROYAL_KING:    return "Cada carta de figura pontuada recebe +30 Chips.";
        case JOKER_ROYAL_QUEEN:   return "Cada As pontuado recebe +20 Chips e +4 Mult.";
        case JOKER_SIXTH_SENSE:   return "Straights e Flushes precisam de apenas 4 cartas.";
        case JOKER_ARCANE_MINOR:  return "Sequencias podem ter lacunas de ate 1 valor\n(ex: 4-5-7-8-9 conta como Straight).";
        case JOKER_THRONE:        return "Todas as cartas jogadas pontuam, mesmo fora\nda combinacao principal.";
        case JOKER_ROYAL_COUNCIL: return "Todas as cartas do deck passam a ser\nconsideradas cartas de figura.";
        case JOKER_OCCULT_LIBRARY:return "Espadas e Paus sao tratados como o mesmo naipe.\nCopas e Ouros tambem.";
        case JOKER_COIN_BOSS:     return "A primeira carta pontuada e ativada mais 2 vezes.";
        case JOKER_NOBLE_LINEAGE: return "Reativa todos os efeitos das cartas mantidas na mao.";
        case JOKER_ROYAL_JACK:    return "Todas as cartas de figura pontuadas sao\nativadas uma segunda vez.";
        case JOKER_LUNCHBOX:      return "Se for a ultima mao da rodada, todas as cartas\npontuadas sao ativadas novamente.";
        case JOKER_COFFEE:        return "Todas as cartas pontuadas sao ativadas novamente\ndurante as proximas 10 rodadas.";
        case JOKER_COIN_FLUSH:    return "Ganha $4 ao final da rodada.";
        case JOKER_COIN_PAIR:     return "Cartas de figura tem 50% de chance de dar $2\nao pontuar.";
        case JOKER_MIRROR_QUIZ:   return "Juros maximos aumentam em +$1.";
        case JOKER_STENCIL:       return "O dinheiro pode chegar ate -$20.";
        case JOKER_COIN_THREE:    return "Ganha $1 ao final de cada rodada.\nSobe +$1 por cada Boss Blind derrotado.";
        case JOKER_COIN_ROYAL:    return "Cartas de figura mantidas na mao tem 50%\nde chance de gerar $1.";
        case JOKER_COIN_LOW:      return "+15 Mult (1 em 6 de chance de ser destruida\nao final da rodada).";
        case JOKER_CAVENDISH:     return "x3 Mult (1 em 1000 de chance de ser destruida\nao final da rodada).";
        case JOKER_POPCORN:       return "x2 Mult (perde 0,04x Mult ao final de cada rodada).";
        case JOKER_ICE_CREAM:     return "+100 Chips (perde 5 Chips ao final de cada rodada).";
        case JOKER_LUCKY_JIMBO:   return "Ganha +1 Mult ao final de cada rodada\nsem usar descartes.";
        case JOKER_ECHO_ARCANO:   return "Se a mao jogada for Straight: ganha\n+15 Chips permanentemente.";
        case JOKER_MARKER:        return "Ganha +1 Mult permanente sempre que um\nTarot e usado na tentativa.";
        case JOKER_RITUAL_TABLE:  return "Ganha +0,1x Mult permanente ao jogar\na mao mais jogada.";
        case JOKER_TWO_PAIR:      return "Se a mao jogada for Two Pair: ganha\n+2 Mult permanentemente.";
        case JOKER_BOSS_SHIELD:   return "Ganha +8 Chips permanentes sempre que\num 2 pontua.";
        case JOKER_CRYPTID_RELAY: return "Copia exatamente o efeito do Coringa\nimediatamente a direita.";
        case JOKER_COSMOS_PRISM:  return "Copia exatamente o efeito do Coringa\nmais a esquerda.";
        case JOKER_FAMILIAR_WAGE: return "Ganha 0,25x Mult ao acertar uma pergunta\ndo quiz.";
        case JOKER_RED_CARD:      return "A cada pacote ignorado ganha +3 Mult\n(comeca com 0).";
        case JOKER_PI_CACHE:      return "Acertar a pergunta do boss garante\n2X Mult na rodada.";
        default: return "Coringa de suporte.";
    }
}

static const char *tarot_display_name(TarotType type) {
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
        default: return "Tarot";
    }
}

static const char *tarot_desc(TarotType type) {
    switch (type) {
        case TAROT_FOOL: return "Copia o ultimo Tarot usado.";
        case TAROT_MAGICIAN: return "Selecione ate 2 cartas da mao para tornar Sorte.";
        case TAROT_HIGH_PRIESTESS: return "Acelera a evolucao de duas maos de poker, como uma leitura favoravel do destino.";
        case TAROT_EMPRESS: return "Selecione ate 2 cartas para tornar Multi.";
        case TAROT_EMPEROR: return "Cria ate 2 Tarots aleatorios.";
        case TAROT_HIEROPHANT: return "Selecione ate 2 cartas para tornar Bonus.";
        case TAROT_LOVERS: return "Selecione 1 carta para tornar Coringa de naipe.";
        case TAROT_CHARIOT: return "Selecione 1 carta para subir 2 ranks.";
        case TAROT_JUSTICE: return "Selecione 1 carta para tornar Vidro.";
        case TAROT_HERMIT: return "Duplica parte do seu caixa atual, ate um teto seguro para o MVP.";
        case TAROT_WHEEL: return "Chance de receber uma marca gratis.";
        case TAROT_STRENGTH: return "Selecione ate 2 cartas para subir 1 rank.";
        case TAROT_HANGED_MAN: return "Selecione ate 2 cartas para reduzir 2 ranks e limpar a melhoria.";
        case TAROT_DEATH: return "Selecione 2 cartas; a primeira copia rank, naipe e melhoria da segunda.";
        case TAROT_TEMPERANCE: return "Recebe o valor de venda total dos Coringas, ate 50.";
        case TAROT_DEVIL: return "Selecione 1 carta para receber uma melhoria aleatoria forte.";
        case TAROT_TOWER: return "Selecione 1 carta para virar Vidro e ganhar 1 rank.";
        case TAROT_STAR: return "Selecione ate 3 cartas para virar Espadas, seguindo a identidade classica do tarot.";
        case TAROT_MOON: return "Selecione ate 3 cartas para virar Copas e favorecer maos vermelhas.";
        case TAROT_SUN: return "Selecione ate 3 cartas para virar Ouros e puxar valor para a jogada.";
        case TAROT_WORLD: return "Selecione ate 3 cartas para virar Paus e completar rotas de naipe escuro.";
        case TAROT_JUDGEMENT: return "Cria um novo Coringa se houver espaco.";
        case TAROT_AURA: return "Spectral adaptada: concede uma edicao aleatoria garantida para um Coringa.";
        case TAROT_WRAITH: return "Spectral adaptada: invoca um Coringa raro, mas cobra moedas pelo pacto.";
        case TAROT_ANKH: return "Spectral adaptada: duplica um Coringa e sacrifica outro para equilibrar o poder.";
        case TAROT_SOUL: return "A mais rara do pack arcano: tenta trazer um Coringa quase lendario para a run.";
        default: return "Tarot utilitario.";
    }
}

static const char *coupon_desc(CouponType type) {
    switch (type) {
        case COUPON_CLEARANCE:      return "Todos os itens da loja custam 25% menos.";
        case COUPON_CLEARANCE_PLUS: return "Todos os itens da loja custam 50% menos.";
        case COUPON_GRABBER:        return "+1 mao por rodada.";
        case COUPON_GRABBER_PLUS:   return "+1 mao adicional por rodada.";
        case COUPON_ORACLE:         return "Dobro de chance de aparecerem Tarots na loja.";
        case COUPON_ORACLE_PLUS:    return "Quadruplo de chance de aparecerem Tarots na loja.";
        case COUPON_HONE:           return "Dobro de chance de encontrar cartas com edicoes.";
        case COUPON_HONE_PLUS:      return "Quadruplo de chance de encontrar cartas com edicoes.";
        default: return "Cupom permanente.";
    }
}

static const char *pack_name(PackKind kind) {
    switch (kind) {
        case PACK_BUFFOON_3_1: return "Buffoon Pack";
        case PACK_BUFFOON_5_1: return "Jumbo Buffoon Pack";
        case PACK_BUFFOON_5_2: return "Mega Buffoon Pack";
        case PACK_ARCANA_3_1: return "Arcana Pack";
        case PACK_ARCANA_5_1: return "Jumbo Arcana Pack";
        case PACK_ARCANA_5_2: return "Mega Arcana Pack";
        default: return "Pack";
    }
}

static const char *pack_desc(PackKind kind) {
    switch (kind) {
        case PACK_BUFFOON_3_1: return "Escolha 1 entre 3 Coringas.";
        case PACK_BUFFOON_5_1: return "Escolha 1 entre 5 Coringas.";
        case PACK_BUFFOON_5_2: return "Escolha 2 entre 5 Coringas.";
        case PACK_ARCANA_3_1: return "Escolha 1 entre 3 Tarots.";
        case PACK_ARCANA_5_1: return "Escolha 1 entre 5 Tarots.";
        case PACK_ARCANA_5_2: return "Escolha 2 entre 5 Tarots.";
        default: return "Pack.";
    }
}

static void load_asset_pack(AssetPack *assets) {
    char path[GUI_ASSET_PATH_CAP];
    if (!assets) return;
    memset(assets, 0, sizeof(*assets));

    assets->card_sheet = load_texture_if_exists("assets/8BitDeck.png");
    assets->joker_sheet = load_texture_if_exists("assets/Jokers.png");
    assets->tarot_sheet = load_texture_if_exists("assets/Tarots.png");
    assets->coupon_sheet = load_texture_if_exists("assets/Vouchers.png");
    assets->pack_sheet = load_texture_if_exists("assets/boosters.png");

    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 1; rank <= 13; rank++) {
            snprintf(path, sizeof(path), "assets/cards/%s_%s.png", asset_rank_slug(rank), asset_suit_slug(suit));
            assets->cards[suit][rank] = load_texture_if_exists(path);
        }
    }
    for (int i = 1; i < GUI_CARD_ENH_TEXTURE_COUNT; i++) {
        snprintf(path, sizeof(path), "assets/enhancements/%d.png", i);
        assets->enhanced[i] = load_texture_if_exists(path);
    }
    for (int i = 1; i < GUI_JOKER_TEXTURE_COUNT; i++) {
        snprintf(path, sizeof(path), "assets/jokers/%d.png", i);
        assets->jokers[i] = load_texture_if_exists(path);
    }
    for (int i = 1; i < GUI_EDITION_TEXTURE_COUNT; i++) {
        snprintf(path, sizeof(path), "assets/editions/%d.png", i);
        assets->editions[i] = load_texture_if_exists(path);
    }
    for (int i = 1; i < GUI_TAROT_TEXTURE_COUNT; i++) {
        snprintf(path, sizeof(path), "assets/tarots/%d.png", i);
        assets->tarots[i] = load_texture_if_exists(path);
    }
    for (int i = 0; i < GUI_COUPON_TEXTURE_COUNT; i++) {
        snprintf(path, sizeof(path), "assets/coupons/%d.png", i);
        assets->coupons[i] = load_texture_if_exists(path);
    }
    for (int i = 0; i <= PACK_ARCANA_5_2; i++) {
        snprintf(path, sizeof(path), "assets/packs/%d.png", i);
        assets->packs[i] = load_texture_if_exists(path);
    }
    assets->villain = load_texture_if_exists("assets/villain/jimbo.png");
    assets->loaded = 1;
}

static void unload_asset_pack(AssetPack *assets) {
    if (!assets || !assets->loaded) return;
    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 1; rank <= 13; rank++) {
            if (texture_ready(assets->cards[suit][rank])) UnloadTexture(assets->cards[suit][rank]);
        }
    }
    for (int i = 1; i < GUI_CARD_ENH_TEXTURE_COUNT; i++) if (texture_ready(assets->enhanced[i])) UnloadTexture(assets->enhanced[i]);
    for (int i = 1; i < GUI_JOKER_TEXTURE_COUNT; i++) if (texture_ready(assets->jokers[i])) UnloadTexture(assets->jokers[i]);
    for (int i = 1; i < GUI_EDITION_TEXTURE_COUNT; i++) if (texture_ready(assets->editions[i])) UnloadTexture(assets->editions[i]);
    for (int i = 1; i < GUI_TAROT_TEXTURE_COUNT; i++) if (texture_ready(assets->tarots[i])) UnloadTexture(assets->tarots[i]);
    for (int i = 0; i < GUI_COUPON_TEXTURE_COUNT; i++) if (texture_ready(assets->coupons[i])) UnloadTexture(assets->coupons[i]);
    for (int i = 0; i <= PACK_ARCANA_5_2; i++) if (texture_ready(assets->packs[i])) UnloadTexture(assets->packs[i]);
    if (texture_ready(assets->card_sheet)) UnloadTexture(assets->card_sheet);
    if (texture_ready(assets->joker_sheet)) UnloadTexture(assets->joker_sheet);
    if (texture_ready(assets->tarot_sheet)) UnloadTexture(assets->tarot_sheet);
    if (texture_ready(assets->coupon_sheet)) UnloadTexture(assets->coupon_sheet);
    if (texture_ready(assets->pack_sheet)) UnloadTexture(assets->pack_sheet);
    if (texture_ready(assets->villain)) UnloadTexture(assets->villain);
    memset(assets, 0, sizeof(*assets));
}

static int pack_price(PackKind kind, const PlayerBuild *build) {
    int price;
    switch (kind) {
        case PACK_BUFFOON_3_1: price = 5; break;
        case PACK_BUFFOON_5_1: price = 7; break;
        case PACK_BUFFOON_5_2: price = 9; break;
        case PACK_ARCANA_3_1: price = 4; break;
        case PACK_ARCANA_5_1: price = 6; break;
        case PACK_ARCANA_5_2: price = 8; break;
        default: price = 5; break;
    }
    if (build->coupon_levels[0] >= 2) price = max_int(1, price / 2);
    else if (build->coupon_levels[0] >= 1) price = max_int(1, (price * 3) / 4);
    return price;
}

static void draw_panel(Rectangle box, Color fill, Color outline) {
    DrawRectangleRounded(box, 0.08f, 10, fill);
    DrawRectangleRoundedLinesEx(box, 0.08f, 10, 3.0f, outline);
}

static bool hover(Rectangle box) {
    return CheckCollisionPointRec(virtual_mouse(), box);
}

static bool clicked(Rectangle box) {
    return hover(box) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void draw_multiline(const char *text, int x, int y, int size, Color color) {
    char line[512];
    int cursor = 0;
    int line_index = 0;
    for (const char *p = text;; p++) {
        if (*p == '\n' || *p == '\0') {
            line[cursor] = '\0';
            DrawText(line, x, y + line_index * (size + 4), size, color);
            line_index++;
            cursor = 0;
            if (*p == '\0') break;
        } else if (cursor < (int)sizeof(line) - 1) {
            line[cursor++] = *p;
        }
    }
}

static int draw_wrapped_text(const char *text, Rectangle box, int size, int line_gap, Color color, int draw) {
    char word[160];
    char line[512];
    int word_len = 0;
    int line_len = 0;
    int lines = 0;
    int max_lines = (int)(box.height / (float)(size + line_gap));

    if (!text || size <= 0) return 0;
    if (max_lines < 1) max_lines = 1;
    line[0] = '\0';

    for (const char *p = text;; p++) {
        int flush_word = (*p == ' ' || *p == '\n' || *p == '\0');
        if (!flush_word) {
            if (word_len < (int)sizeof(word) - 1) word[word_len++] = *p;
        }
        if (flush_word) {
            int candidate_width;
            word[word_len] = '\0';
            if (word_len > 0) {
                char candidate[640];
                if (line_len > 0) snprintf(candidate, sizeof(candidate), "%s %s", line, word);
                else snprintf(candidate, sizeof(candidate), "%s", word);
                candidate_width = MeasureText(candidate, size);
                if (line_len > 0 && candidate_width > box.width) {
                    if (draw && lines < max_lines) DrawText(line, (int)box.x, (int)(box.y + lines * (size + line_gap)), size, color);
                    lines++;
                    snprintf(line, sizeof(line), "%s", word);
                    line_len = (int)strlen(line);
                } else {
                    snprintf(line, sizeof(line), "%s", candidate);
                    line_len = (int)strlen(line);
                }
                word_len = 0;
            }
            if (*p == '\n' || *p == '\0') {
                if (line_len > 0 || *p == '\n') {
                    if (draw && lines < max_lines) DrawText(line, (int)box.x, (int)(box.y + lines * (size + line_gap)), size, color);
                    lines++;
                    line[0] = '\0';
                    line_len = 0;
                }
            }
            if (*p == '\0') break;
        }
    }

    return lines;
}

static void set_status(AppState *app, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(app->status, sizeof(app->status), fmt, args);
    va_end(args);
}

static void set_tooltip(AppState *app, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(app->tooltip, sizeof(app->tooltip), fmt, args);
    va_end(args);
}

static void save_active_collection(const AppState *app) {
    if (!app || !app->active_player[0]) return;
    player_collection_save(PLAYER_COLLECTION_FILE, &app->collection);
}

static void load_active_collection(AppState *app) {
    if (!app) return;
    if (!app->active_player[0]) {
        player_collection_clear(&app->collection);
        return;
    }
    player_collection_load(PLAYER_COLLECTION_FILE, app->active_player, &app->collection);
}

static void unlock_collection_joker(AppState *app, JokerType joker) {
    if (!app || !app->active_player[0] || joker <= JOKER_NONE) return;
    if (player_collection_has(app->collection.joker_bits, (int)joker)) return;
    player_collection_unlock(&app->collection.joker_bits, (int)joker);
    save_active_collection(app);
}

static void unlock_collection_coupon(AppState *app, CouponType coupon) {
    int index;
    if (!app || !app->active_player[0]) return;
    index = collection_coupon_index(coupon);
    if (player_collection_has(app->collection.coupon_bits, index)) return;
    player_collection_unlock(&app->collection.coupon_bits, index);
    save_active_collection(app);
}

static void unlock_collection_mark(AppState *app, JokerEdition edition) {
    int index;
    if (!app || !app->active_player[0]) return;
    index = collection_mark_index(edition);
    if (index <= 0 || player_collection_has(app->collection.mark_bits, index)) return;
    player_collection_unlock(&app->collection.mark_bits, index);
    save_active_collection(app);
}

static void unlock_collection_boss(AppState *app, int phase_number) {
    int index;
    if (!app || !app->active_player[0]) return;
    index = collection_boss_index(phase_number);
    if (player_collection_has(app->collection.boss_bits, index)) return;
    player_collection_unlock(&app->collection.boss_bits, index);
    save_active_collection(app);
}

static void unlock_collection_tarot(AppState *app, TarotType tarot) {
    if (!app || !app->active_player[0] || tarot <= TAROT_NONE) return;
    if (player_collection_has(app->collection.tarot_bits, (int)tarot)) return;
    player_collection_unlock(&app->collection.tarot_bits, (int)tarot);
    save_active_collection(app);
}

static int player_is_linked(const AppState *app) {
    return app && app->active_player[0] && player_mask_is_valid(app->question_mask);
}

static void prime_profile_from_active(AppState *app) {
    if (!app) return;
    if (player_is_linked(app)) {
        snprintf(app->draft_player, sizeof(app->draft_player), "%s", app->active_player);
        app->draft_question_mask = app->question_mask;
    } else {
        if (!app->draft_player[0]) app->draft_player[0] = '\0';
        if (!player_mask_is_valid(app->draft_question_mask)) app->draft_question_mask = PLAYER_MASK_LOGIC | PLAYER_MASK_MAT;
    }
}

static void unlink_active_player(AppState *app) {
    if (!app) return;
    memset(app->active_player, 0, sizeof(app->active_player));
    memset(app->draft_player, 0, sizeof(app->draft_player));
    player_collection_clear(&app->collection);
    app->question_mask = 0;
    app->draft_question_mask = PLAYER_MASK_LOGIC | PLAYER_MASK_MAT;
    app->auto_start_after_profile = 0;
    set_status(app, "Jogador desvinculado. Outra pessoa pode assumir a proxima run.");
}

static void handle_text_box_input(char *buffer, size_t size, int active) {
    int codepoint;
    size_t len;
    if (!active || !buffer || size == 0) return;

    codepoint = GetCharPressed();
    while (codepoint > 0) {
        len = strlen(buffer);
        if (codepoint >= 32 && codepoint <= 126 && len + 1 < size) {
            buffer[len] = (char)codepoint;
            buffer[len + 1] = '\0';
        }
        codepoint = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        len = strlen(buffer);
        if (len > 0) buffer[len - 1] = '\0';
    }
}

static void save_profile_from_draft(AppState *app, int start_run_after_save) {
    char clean_name[PLAYER_NAME_CAP];
    int mask;
    if (!app) return;

    player_copy_clean(clean_name, sizeof(clean_name), app->draft_player);
    if (!clean_name[0]) {
        set_status(app, "Digite um nome para salvar a run.");
        return;
    }
    if (!player_mask_is_valid(app->draft_question_mask)) {
        set_status(app, "Selecione pelo menos uma categoria de perguntas.");
        return;
    }

    mask = player_name_is_admin(clean_name) ? PLAYER_MASK_ALL : app->draft_question_mask;
    snprintf(app->active_player, sizeof(app->active_player), "%s", clean_name);
    snprintf(app->draft_player, sizeof(app->draft_player), "%s", clean_name);
    app->question_mask = mask;
    app->draft_question_mask = mask;
    load_active_collection(app);
    app->auto_start_after_profile = 0;
    if (player_name_is_admin(clean_name)) set_status(app, "Jogador Admin vinculado com colecao completa liberada.");
    else set_status(app, "Jogador %s vinculado como %s.", app->active_player, player_modality_name_from_mask(app->question_mask));

    if (start_run_after_save) {
        app->screen = SCREEN_TITLE;
        app->auto_start_after_profile = 1;
    } else {
        app->screen = SCREEN_TITLE;
    }
}

static void activate_admin_profile(AppState *app) {
    if (!app) return;
    snprintf(app->active_player, sizeof(app->active_player), "Admin");
    snprintf(app->draft_player, sizeof(app->draft_player), "Admin");
    app->question_mask = PLAYER_MASK_ALL;
    app->draft_question_mask = PLAYER_MASK_ALL;
    load_active_collection(app);
    set_status(app, "Jogador Admin pronto com a colecao completa liberada.");
}

static int count_selected_cards(const AppState *app) {
    int count = 0;
    for (int i = 0; i < app->hand_count; i++) if (app->selected[i]) count++;
    return count;
}

static int count_matching_suit(Card selected[], int count, int suit) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (selected[i].enhancement == ENHANCEMENT_WILD || selected[i].suit == suit) total++;
    }
    return total;
}

static int count_royals(Card selected[], int count) {
    int total = 0;
    for (int i = 0; i < count; i++) if (selected[i].rank >= 11 || selected[i].rank == 1) total++;
    return total;
}

static int count_lows(Card selected[], int count) {
    int total = 0;
    for (int i = 0; i < count; i++) if (selected[i].rank >= 2 && selected[i].rank <= 4) total++;
    return total;
}

static int count_rank(Card selected[], int count, int rank) {
    int total = 0;
    for (int i = 0; i < count; i++) if (selected[i].rank == rank) total++;
    return total;
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

static int hand_level_threshold(const PlayerBuild *build, HandType type) {
    int level = build->hand_levels[type];
    return HAND_GROWTH[type].threshold_base + (level - 1) * 2;
}

static void apply_hand_level_bonus(const PlayerBuild *build, HandEval *eval) {
    int extra_levels = build->hand_levels[eval->type] - 1;
    if (extra_levels <= 0) return;
    eval->base_score += HAND_GROWTH[eval->type].chips * extra_levels;
    eval->multiplier += HAND_GROWTH[eval->type].mult * extra_levels;
}

static void maybe_level_up_hand(PlayerBuild *build, HandType type, char *status, size_t status_size) {
    build->hand_progress[type]++;
    while (build->hand_progress[type] >= hand_level_threshold(build, type)) {
        build->hand_progress[type] -= hand_level_threshold(build, type);
        build->hand_levels[type]++;
        snprintf(status, status_size, "%s evoluiu para nivel %d.", hand_type_name(type), build->hand_levels[type]);
    }
}

static PlayOutcome calculate_outcome_internal(AppState *app, Card selected[], int count, PhaseState *phase,
                                              ActivationTrace *trace, SelectionPreview *preview, int consume_phase) {
    PlayOutcome outcome;
    double chips;
    double mult;
    double xmult;
    int royal_count;
    int kings;
    int queens;
    int jacks;
    int low_count;

    memset(&outcome, 0, sizeof(outcome));
    if (trace) memset(trace, 0, sizeof(*trace));
    if (preview) memset(preview, 0, sizeof(*preview));

    outcome.eval = evaluate_cards(selected, count);
    apply_hand_overrides(phase, &outcome.eval, count);
    apply_hand_level_bonus(&app->build, &outcome.eval);

    chips = outcome.eval.base_score + outcome.eval.rank_sum + phase->next_play_bonus_chips + phase->discard_score_bonus;
    mult = outcome.eval.multiplier + phase->next_play_bonus_mult;
    xmult = phase->next_play_xmult > 0.0 ? phase->next_play_xmult : 1.0;
    royal_count = count_royals(selected, count);
    kings = count_rank(selected, count, 13);
    queens = count_rank(selected, count, 12);
    jacks = count_rank(selected, count, 11);
    low_count = count_lows(selected, count);

    if (phase->next_play_bonus_chips > 0 || phase->discard_score_bonus > 0) {
        if (trace) trace->counter_hits[0]++;
    }
    if (phase->next_play_bonus_mult > 0) {
        if (trace) trace->counter_hits[1]++;
    }
    if (phase->next_play_xmult > 1.0) {
        if (trace) trace->counter_hits[2]++;
    }

    for (int i = 0; i < count; i++) {
        CardEnhancement enhancement = selected[i].enhancement;
        if (phase->all_bonus_once) enhancement = ENHANCEMENT_BONUS;
        if (phase->all_mult_once) enhancement = ENHANCEMENT_MULT;
        if (phase->all_lucky_once) enhancement = ENHANCEMENT_LUCKY;
        if (phase->all_glass_once) enhancement = ENHANCEMENT_GLASS;

        if (selected[i].rank > 0) {
            if (trace) {
                trace->card_hits[i]++;
                trace->counter_hits[0]++;
            }
        }
        if (enhancement != ENHANCEMENT_NONE) outcome.enhanced_count++;
        if (enhancement == ENHANCEMENT_BONUS) {
            chips += 20;
            if (trace) {
                trace->card_hits[i]++;
                trace->counter_hits[0]++;
            }
        } else if (enhancement == ENHANCEMENT_MULT) {
            mult += 4;
            if (trace) {
                trace->card_hits[i]++;
                trace->counter_hits[1]++;
            }
        } else if (enhancement == ENHANCEMENT_LUCKY) {
            if ((rand() % 4) == 0) {
                mult += 20;
                outcome.lucky_triggered = 1;
                if (trace) {
                    trace->card_hits[i]++;
                    trace->counter_hits[1]++;
                }
            }
        } else if (enhancement == ENHANCEMENT_GLASS) {
            xmult *= 2.0;
            if (trace) {
                trace->card_hits[i]++;
                trace->counter_hits[2]++;
            }
        }

        if (phase->blessed_suit >= 0 && (selected[i].suit == phase->blessed_suit || selected[i].enhancement == ENHANCEMENT_WILD)) {
            chips += 15;
            mult += 2;
            if (trace) {
                trace->card_hits[i]++;
                trace->counter_hits[0]++;
                trace->counter_hits[1]++;
            }
        }
    }

    for (int i = 0; i < app->build.joker_count; i++) {
        JokerInstance *joker = &app->build.jokers[i];
        if (!joker->active) continue;

        switch (joker->type) {
            case JOKER_FLAT: chips += 24; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } break;
            case JOKER_PAIR: if (outcome.eval.type == HAND_PAIR) { chips += 16; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } } break;
            case JOKER_TWO_PAIR: if (outcome.eval.type == HAND_TWO_PAIR) { mult += 7; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } } break;
            case JOKER_THREE: if (outcome.eval.type == HAND_THREE) { chips += 30; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } } break;
            case JOKER_FOUR: if (outcome.eval.type == HAND_FOUR) { xmult *= 1.7; if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; } } break;
            case JOKER_STRAIGHT: if (outcome.eval.type == HAND_STRAIGHT || outcome.eval.type == HAND_STRAIGHT_FLUSH) { mult += 12; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } } break;
            case JOKER_FLUSH: if (outcome.eval.type == HAND_FLUSH || outcome.eval.type == HAND_STRAIGHT_FLUSH) { chips += 36; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } } break;
            case JOKER_COIN_PAIR: if (outcome.eval.type == HAND_PAIR) { outcome.coins += 2; if (trace) { trace->joker_hits[i]++; trace->counter_hits[3]++; } } break;
            case JOKER_COIN_FLUSH: if (outcome.eval.type == HAND_FLUSH || outcome.eval.type == HAND_STRAIGHT_FLUSH) { outcome.coins += 2; if (trace) { trace->joker_hits[i]++; trace->counter_hits[3]++; } } break;
            case JOKER_COIN_THREE: if (outcome.eval.type == HAND_THREE) { outcome.coins += 2; if (trace) { trace->joker_hits[i]++; trace->counter_hits[3]++; } } break;
            case JOKER_COIN_ROYAL: if (royal_count > 0) { outcome.coins += 1; if (trace) { trace->joker_hits[i] += royal_count; trace->counter_hits[3] += royal_count; } } break;
            case JOKER_COIN_LOW: if (low_count > 0) { outcome.coins += 1; if (trace) { trace->joker_hits[i]++; trace->counter_hits[3]++; } } break;
            case JOKER_COIN_BOSS: break;
            case JOKER_POPCORN: mult += joker->value; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } break;
            case JOKER_ICE_CREAM: chips += joker->value; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } break;
            case JOKER_CAVENDISH: mult += joker->value; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } break;
            case JOKER_COFFEE: outcome.coins += 2; if (trace) { trace->joker_hits[i]++; trace->counter_hits[3]++; } break;
            case JOKER_LUNCHBOX: if (phase->first_hand) { chips += joker->value; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } } break;
            case JOKER_MARKER: if (phase->question_correct) { mult += 8; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } } break;
            case JOKER_ROYAL_KING: if (kings > 0) { chips += kings * 16; if (trace) { trace->joker_hits[i] += kings; trace->counter_hits[0] += kings; } } break;
            case JOKER_ROYAL_QUEEN: if (queens > 0) { mult += queens * 4; if (trace) { trace->joker_hits[i] += queens; trace->counter_hits[1] += queens; } } break;
            case JOKER_ROYAL_JACK: if (jacks > 0 && (rand() % 3) == 0) { outcome.coins += 2; if (trace) { trace->joker_hits[i]++; trace->counter_hits[3]++; } } break;
            case JOKER_ROYAL_COUNCIL: if (royal_count >= 2) { xmult *= 1.35; if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; } } break;
            case JOKER_THRONE: if (kings > 0 && queens > 0) { chips += 40; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } } break;
            case JOKER_NOBLE_LINEAGE: if (royal_count == count && count > 0) { xmult *= 1.75; if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; } } break;
            case JOKER_ARCANE_MINOR: if (app->build.tarot_count > 0) { mult += app->build.tarot_count * 2; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } } break;
            case JOKER_SIXTH_SENSE: break;
            case JOKER_OCCULT_LIBRARY: if (app->build.tarot_count > 0) { mult += app->build.tarot_count * 3; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } } break;
            case JOKER_RITUAL_TABLE: if (phase->tarot_used_this_phase) { chips += 35; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } } break;
            case JOKER_ECHO_ARCANO: if (app->build.last_used_tarot != TAROT_NONE) { xmult *= 1.2; if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; } } break;
            case JOKER_STENCIL: {
                double extra = 1.0 + (app->build.joker_capacity - app->build.joker_count) * 0.25;
                if (extra > 1.0) {
                    xmult *= extra;
                    if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; }
                }
                break;
            }
            case JOKER_GREEDY: {
                int bonus = count_matching_suit(selected, count, SUIT_OUROS) * 3;
                if (bonus > 0) { mult += bonus; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } }
                break;
            }
            case JOKER_LOVELY: {
                int bonus = count_matching_suit(selected, count, SUIT_COPAS) * 3;
                if (bonus > 0) { mult += bonus; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } }
                break;
            }
            case JOKER_WRATHFUL: {
                int bonus = count_matching_suit(selected, count, SUIT_ESPADAS) * 12;
                if (bonus > 0) { chips += bonus; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } }
                break;
            }
            case JOKER_ASTUTE: {
                int bonus = count_matching_suit(selected, count, SUIT_PAUS) * 12;
                if (bonus > 0) { chips += bonus; if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; } }
                break;
            }
            case JOKER_MIRROR_QUIZ: if (phase->question_correct) { xmult *= 1.6; if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; } } break;
            case JOKER_BOSS_SHIELD: if (phase->boss) { xmult *= 1.35; if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; } } break;
            case JOKER_CRYPTID_RELAY: if (count_joker_editions(&app->build) >= 2) { xmult *= 1.25; if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; } } break;
            case JOKER_COSMOS_PRISM: if (outcome.enhanced_count >= 2) { xmult *= 1.35; if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; } } break;
            case JOKER_LUCKY_JIMBO: if (outcome.lucky_triggered) { xmult *= 1.3; if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; } } break;
            case JOKER_FAMILIAR_WAGE:
                if (outcome.eval.type == HAND_PAIR || outcome.eval.type == HAND_TWO_PAIR || outcome.eval.type == HAND_THREE) {
                    outcome.coins += 2;
                    if (trace) { trace->joker_hits[i]++; trace->counter_hits[3]++; }
                }
                break;
            case JOKER_PI_CACHE: if (phase->tarot_used_this_phase) { mult += 8; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } } break;
            case JOKER_RED_CARD: if (joker->value > 0) { mult += joker->value; if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; } } break;
            default: break;
        }

        if (joker->edition == EDITION_FOIL) {
            chips += 50;
            if (trace) { trace->joker_hits[i]++; trace->counter_hits[0]++; }
        } else if (joker->edition == EDITION_CHROME) {
            mult += 4;
            if (trace) { trace->joker_hits[i]++; trace->counter_hits[1]++; }
        } else if (joker->edition == EDITION_PRISMATIC) {
            xmult *= 1.5;
            if (trace) { trace->joker_hits[i]++; trace->counter_hits[2]++; }
        }
    }

    if (phase->debuff_percent > 0) xmult *= (100.0 - phase->debuff_percent) / 100.0;

    {
        double total = chips * mult * xmult;
        if (total < 0) total = 0;
        outcome.points = (int)(total + 0.5);
    }

    if (preview) {
        preview->valid = count > 0;
        preview->selected_count = count;
        preview->eval = outcome.eval;
        preview->chips = chips;
        preview->mult = mult;
        preview->xmult = xmult;
        preview->points = outcome.points;
        preview->coins = outcome.coins;
    }

    if (consume_phase) {
        app->phase.force_flush_once = phase->force_flush_once;
        app->phase.next_play_bonus_chips = 0;
        app->phase.next_play_bonus_mult = 0;
        app->phase.next_play_xmult = 1.0;
        app->phase.discard_score_bonus = 0;
        app->phase.all_bonus_once = 0;
        app->phase.all_mult_once = 0;
        app->phase.all_lucky_once = 0;
        app->phase.all_glass_once = 0;
        app->phase.blessed_suit = -1;
    }

    return outcome;
}

static Question draw_random_question(const AppState *app, QuizMode mode) {
    int bet = app ? ((app->phase_number - 1) / 3) + 1 : 1;
    RayQuizDifficulty difficulty = (mode == QUIZ_BOSS || mode == QUIZ_SKIP)
        ? RAY_QUIZ_DIFFICULTY_MUITO_DIFICIL
        : ray_quiz_difficulty_from_bet(bet);
    int mask = app ? app->question_mask : PLAYER_MASK_ALL;
    return ray_quiz_draw(mask, difficulty);
}

static void seed_run_code(AppState *app) {
    static const char chars[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    for (int i = 0; i < 7; i++) app->stats.run_code[i] = chars[rand() % (int)(sizeof(chars) - 1)];
    app->stats.run_code[7] = '\0';
}

static void init_master_deck(AppState *app) {
    int idx = 0;
    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 1; rank <= 13; rank++) {
            app->master_deck[idx].rank = rank;
            app->master_deck[idx].suit = suit;
            app->master_deck[idx].enhancement = ENHANCEMENT_NONE;
            idx++;
        }
    }
}

static void build_phase_deck_from_master(AppState *app) {
    for (int i = 0; i < DECK_SIZE; i++) {
        app->phase_deck.cards[i] = app->master_deck[i];
        app->phase_sources[i] = i;
    }
    app->phase_deck.top = 0;
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card tmp = app->phase_deck.cards[i];
        int src = app->phase_sources[i];
        app->phase_deck.cards[i] = app->phase_deck.cards[j];
        app->phase_sources[i] = app->phase_sources[j];
        app->phase_deck.cards[j] = tmp;
        app->phase_sources[j] = src;
    }
}

static int draw_phase_card(AppState *app, Card *out_card, int *out_source) {
    if (app->phase_deck.top >= DECK_SIZE) return 0;
    *out_card = app->phase_deck.cards[app->phase_deck.top];
    *out_source = app->phase_sources[app->phase_deck.top];
    app->phase_deck.top++;
    return 1;
}

static void fill_hand(AppState *app) {
    app->hand_count = 0;
    while (app->hand_count < HAND_SIZE) {
        Card card;
        int source;
        if (!draw_phase_card(app, &card, &source)) break;
        app->hand[app->hand_count] = card;
        app->hand_sources[app->hand_count] = source;
        app->selected[app->hand_count] = false;
        app->select_anim[app->hand_count] = 0.0f;
        app->hover_anim[app->hand_count] = 0.0f;
        app->hand_count++;
    }
}

static void remove_and_draw(AppState *app, int indices[], int count) {
    int selected_mask[HAND_SIZE] = {0};
    Card new_hand[HAND_SIZE];
    int new_sources[HAND_SIZE];
    int new_count = 0;

    for (int i = 0; i < count; i++) if (indices[i] >= 0 && indices[i] < app->hand_count) selected_mask[indices[i]] = 1;
    for (int i = 0; i < app->hand_count; i++) {
        if (!selected_mask[i]) {
            new_hand[new_count] = app->hand[i];
            new_sources[new_count] = app->hand_sources[i];
            new_count++;
        }
    }
    while (new_count < HAND_SIZE) {
        Card card;
        int source;
        if (!draw_phase_card(app, &card, &source)) break;
        new_hand[new_count] = card;
        new_sources[new_count] = source;
        new_count++;
    }
    for (int i = 0; i < new_count; i++) {
        app->hand[i] = new_hand[i];
        app->hand_sources[i] = new_sources[i];
        app->selected[i] = false;
    }
    app->hand_count = new_count;
}

static void update_master_card(AppState *app, int master_index, Card card) {
    if (master_index < 0 || master_index >= DECK_SIZE) return;
    app->master_deck[master_index] = card;
    for (int i = 0; i < app->hand_count; i++) {
        if (app->hand_sources[i] == master_index) app->hand[i] = card;
    }
}

static TarotType random_tarot_gui(void) {
    TarotType common[] = {TAROT_MAGICIAN, TAROT_EMPRESS, TAROT_HIEROPHANT, TAROT_LOVERS, TAROT_STAR, TAROT_MOON, TAROT_SUN, TAROT_WORLD, TAROT_STRENGTH};
    TarotType uncommon[] = {TAROT_FOOL, TAROT_HIGH_PRIESTESS, TAROT_EMPEROR, TAROT_WHEEL, TAROT_HERMIT, TAROT_TEMPERANCE, TAROT_CHARIOT, TAROT_HANGED_MAN};
    TarotType rare[] = {TAROT_JUSTICE, TAROT_DEATH, TAROT_DEVIL, TAROT_TOWER, TAROT_JUDGEMENT};
    int roll = rand() % 100;
    if (roll < 62) return common[rand() % (int)(sizeof(common) / sizeof(common[0]))];
    if (roll < 92) return uncommon[rand() % (int)(sizeof(uncommon) / sizeof(uncommon[0]))];
    return rare[rand() % (int)(sizeof(rare) / sizeof(rare[0]))];
}

static TarotType random_arcana_pack_tarot_gui(void) {
    int roll = rand() % 100;
    if (roll < 5) return TAROT_SOUL;
    if (roll < 10) return TAROT_ANKH;
    if (roll < 16) return TAROT_WRAITH;
    if (roll < 24) return TAROT_AURA;
    return random_tarot_gui();
}

static void init_offer(Offer *offer) {
    memset(offer, 0, sizeof(*offer));
}

static void fill_joker_offer(AppState *app, Offer *offer, JokerEdition forced_edition) {
    init_offer(offer);
    offer->type = OFFER_JOKER;
    offer->joker = random_shop_joker();
    offer->edition = forced_edition != EDITION_NONE ? forced_edition : random_shop_edition(&app->build, app->phase_number);
    offer->price = shop_discounted_price(joker_base_price(offer->joker) + joker_edition_extra_cost(offer->edition), &app->build);
    unlock_collection_joker(app, offer->joker);
    if (offer->edition != EDITION_NONE) unlock_collection_mark(app, offer->edition);
}

static void fill_tarot_offer(AppState *app, Offer *offer) {
    init_offer(offer);
    offer->type = OFFER_TAROT;
    offer->tarot = random_tarot_gui();
    offer->price = shop_discounted_price(3, &app->build);
    unlock_collection_tarot(app, offer->tarot);
}

static void build_shop_offer(AppState *app, Offer *offer, JokerEdition forced_edition) {
    init_offer(offer);
    if (forced_edition != EDITION_NONE) {
        fill_joker_offer(app, offer, forced_edition);
        return;
    }
    if ((rand() % 100) < (app->build.coupon_levels[2] > 0 ? 28 : 16)) {
        fill_tarot_offer(app, offer);
    } else {
        fill_joker_offer(app, offer, EDITION_NONE);
    }
}

static PackKind random_pack_kind(int arcana) {
    int roll = rand() % 100;
    if (!arcana) {
        if (roll < 52) return PACK_BUFFOON_3_1;
        if (roll < 84) return PACK_BUFFOON_5_1;
        return PACK_BUFFOON_5_2;
    }
    if (roll < 52) return PACK_ARCANA_3_1;
    if (roll < 84) return PACK_ARCANA_5_1;
    return PACK_ARCANA_5_2;
}

static void build_shop(AppState *app) {
    app->shop.reroll_cost = 5;
    build_shop_offer(app, &app->shop.slots[0], app->pending_mark_count > 0 ? pop_pending_mark(app) : EDITION_NONE);
    build_shop_offer(app, &app->shop.slots[1], app->pending_mark_count > 0 ? pop_pending_mark(app) : EDITION_NONE);
    init_offer(&app->shop.voucher);
    app->shop.voucher.type = OFFER_COUPON;
    app->shop.voucher.coupon = random_coupon_offer(&app->build);
    app->shop.voucher.price = 10;
    unlock_collection_coupon(app, app->shop.voucher.coupon);
    init_offer(&app->shop.packs[0]);
    app->shop.packs[0].type = OFFER_PACK;
    app->shop.packs[0].pack_kind = random_pack_kind(0);
    app->shop.packs[0].price = pack_price(app->shop.packs[0].pack_kind, &app->build);
    init_offer(&app->shop.packs[1]);
    app->shop.packs[1].type = OFFER_PACK;
    app->shop.packs[1].pack_kind = random_pack_kind(1);
    app->shop.packs[1].price = pack_price(app->shop.packs[1].pack_kind, &app->build);
}

static int pack_has_joker_offer(const PackState *pack, JokerType joker) {
    for (int i = 0; i < pack->choice_count; i++) {
        if (pack->choices[i].type == OFFER_JOKER && pack->choices[i].joker == joker) return 1;
    }
    return 0;
}

static int pack_has_tarot_offer(const PackState *pack, TarotType tarot) {
    for (int i = 0; i < pack->choice_count; i++) {
        if (pack->choices[i].type == OFFER_TAROT && pack->choices[i].tarot == tarot) return 1;
    }
    return 0;
}

static void set_pack_choices(AppState *app, PackKind kind) {
    app->pack.active = 1;
    app->pack.kind = kind;
    app->pack.choice_count = (kind == PACK_BUFFOON_3_1 || kind == PACK_ARCANA_3_1) ? 3 : 5;
    app->pack.picks_remaining = (kind == PACK_BUFFOON_5_2 || kind == PACK_ARCANA_5_2) ? 2 : 1;
    for (int i = 0; i < MAX_PACK_CHOICES; i++) init_offer(&app->pack.choices[i]);
    for (int i = 0; i < app->pack.choice_count; i++) {
        if (kind == PACK_BUFFOON_3_1 || kind == PACK_BUFFOON_5_1 || kind == PACK_BUFFOON_5_2) {
            JokerType joker;
            app->pack.choices[i].type = OFFER_JOKER;
            do {
                joker = random_shop_joker();
            } while (pack_has_joker_offer(&app->pack, joker));
            app->pack.choices[i].joker = joker;
            app->pack.choices[i].edition = app->pending_mark_count > 0 ? pop_pending_mark(app) : random_shop_edition(&app->build, app->phase_number);
            unlock_collection_joker(app, joker);
            if (app->pack.choices[i].edition != EDITION_NONE) unlock_collection_mark(app, app->pack.choices[i].edition);
        } else {
            TarotType tarot;
            app->pack.choices[i].type = OFFER_TAROT;
            do {
                tarot = random_arcana_pack_tarot_gui();
            } while (pack_has_tarot_offer(&app->pack, tarot));
            app->pack.choices[i].tarot = tarot;
            unlock_collection_tarot(app, tarot);
        }
    }
}

static int is_tarot_targeted(TarotType tarot) {
    switch (tarot) {
        case TAROT_MAGICIAN:
        case TAROT_EMPRESS:
        case TAROT_HIEROPHANT:
        case TAROT_LOVERS:
        case TAROT_CHARIOT:
        case TAROT_JUSTICE:
        case TAROT_STRENGTH:
        case TAROT_HANGED_MAN:
        case TAROT_DEATH:
        case TAROT_DEVIL:
        case TAROT_TOWER:
        case TAROT_STAR:
        case TAROT_MOON:
        case TAROT_SUN:
        case TAROT_WORLD:
            return 1;
        default:
            return 0;
    }
}

static int tarot_target_count(TarotType tarot) {
    switch (tarot) {
        case TAROT_MAGICIAN:
        case TAROT_EMPRESS:
        case TAROT_HIEROPHANT:
        case TAROT_STRENGTH:
        case TAROT_HANGED_MAN:
        case TAROT_DEATH:
            return 2;
        case TAROT_STAR:
        case TAROT_MOON:
        case TAROT_SUN:
        case TAROT_WORLD:
            return 3;
        default:
            break;
    }
    return 1;
}

static int tarot_can_use_anytime(TarotType tarot) {
    switch (tarot) {
        case TAROT_FOOL:
        case TAROT_HIGH_PRIESTESS:
        case TAROT_EMPEROR:
        case TAROT_HERMIT:
        case TAROT_WHEEL:
        case TAROT_TEMPERANCE:
        case TAROT_JUDGEMENT:
        case TAROT_AURA:
        case TAROT_WRAITH:
        case TAROT_ANKH:
        case TAROT_SOUL:
            return 1;
        default:
            return 0;
    }
}

static void apply_random_free_edition_local(AppState *app) {
    JokerEdition edition = random_reward_mark(app);
    queue_mark_reward(app, edition, "Marca %s garantida para a proxima oferta de Coringa.");
}

static void consume_tarot_from_inventory(AppState *app, int inventory_index, TarotType tarot) {
    if (inventory_index >= 0 && inventory_index < app->build.tarot_count) {
        for (int i = inventory_index; i < app->build.tarot_count - 1; i++) {
            app->build.tarot_inventory[i] = app->build.tarot_inventory[i + 1];
        }
        app->build.tarot_count--;
    }
    app->build.last_used_tarot = tarot;
    app->phase.tarot_used_this_phase = 1;
    app->stats.tarots_used++;
    if (tarot > TAROT_NONE && tarot < GUI_TAROT_TEXTURE_COUNT) app->stats.tarot_use_counts[tarot]++;
}

static void apply_non_target_tarot(AppState *app, TarotType tarot, int from_inventory, int inventory_index) {
    if (tarot == TAROT_FOOL) {
        if (app->build.last_used_tarot != TAROT_NONE && build_add_tarot(&app->build, app->build.last_used_tarot)) {
            set_status(app, "O Louco copiou %s.", tarot_display_name(app->build.last_used_tarot));
        } else set_status(app, "O Louco nao encontrou Tarot valido.");
    } else if (tarot == TAROT_HIGH_PRIESTESS) {
        HandType a = (HandType)(rand() % MAX_HAND_LEVEL_TRACK);
        HandType b = (HandType)(rand() % MAX_HAND_LEVEL_TRACK);
        maybe_level_up_hand(&app->build, a, app->status, sizeof(app->status));
        maybe_level_up_hand(&app->build, b, app->status, sizeof(app->status));
        set_status(app, "A Sacerdotisa acelerou duas maos.");
    } else if (tarot == TAROT_EMPEROR) {
        TarotType first = random_tarot_gui();
        TarotType second = random_tarot_gui();
        while (second == first) second = random_tarot_gui();
        build_add_tarot(&app->build, first);
        build_add_tarot(&app->build, second);
        unlock_collection_tarot(app, first);
        unlock_collection_tarot(app, second);
        set_status(app, "O Imperador criou ate 2 Tarots.");
    } else if (tarot == TAROT_HERMIT) {
        int gain = min_int(app->build.coins, 20);
        app->build.coins += gain;
        set_status(app, "O Eremita concedeu +%d moedas.", gain);
    } else if (tarot == TAROT_WHEEL) {
        if ((rand() % 4) == 0) apply_random_free_edition_local(app);
        else set_status(app, "Roda da Fortuna nao ativou desta vez.");
    } else if (tarot == TAROT_TEMPERANCE) {
        int gain = 0;
        for (int i = 0; i < app->build.joker_count; i++) gain += app->build.jokers[i].sell_value;
        gain = min_int(gain, 50);
        app->build.coins += gain;
        set_status(app, "Temperanca concedeu +%d moedas.", gain);
    } else if (tarot == TAROT_JUDGEMENT) {
        JokerType joker = random_shop_joker();
        unlock_collection_joker(app, joker);
        if (build_add_joker(&app->build, joker)) {
            set_status(app, "Julgamento criou um novo Coringa.");
        } else set_status(app, "Sem espaco para o Coringa de Julgamento.");
    } else if (tarot == TAROT_AURA) {
        apply_random_free_edition_local(app);
    } else if (tarot == TAROT_WRAITH) {
        JokerType joker = random_rare_joker();
        int tax = min_int(app->build.coins, 6);
        unlock_collection_joker(app, joker);
        app->build.coins -= tax;
        if (build_add_joker(&app->build, joker)) set_status(app, "Wraith invocou %s e cobrou %d moedas.", joker_name(joker), tax);
        else set_status(app, "Wraith cobrou %d moedas, mas faltou espaco para o Coringa raro.", tax);
    } else if (tarot == TAROT_ANKH) {
        int source = app->selected_joker_index >= 0 && app->selected_joker_index < app->build.joker_count ? app->selected_joker_index : 0;
        if (app->build.joker_count <= 0) {
            set_status(app, "Ankh precisa de pelo menos 1 Coringa em jogo.");
        } else if (!build_add_joker(&app->build, app->build.jokers[source].type)) {
            set_status(app, "Ankh nao encontrou espaco para duplicar o Coringa.");
        } else {
            JokerInstance *copy = &app->build.jokers[app->build.joker_count - 1];
            *copy = app->build.jokers[source];
            copy->sell_value = max_int(1, app->build.jokers[source].sell_value);
            if (app->build.joker_count >= 3) {
                int sacrifice = source == 0 ? 1 : 0;
                app->build.jokers[sacrifice].active = 0;
                compact_jokers(&app->build);
            }
            set_status(app, "Ankh duplicou %s e cobrou um sacrificio da colecao.", joker_name(copy->type));
        }
    } else if (tarot == TAROT_SOUL) {
        JokerType joker = random_legendary_joker();
        unlock_collection_joker(app, joker);
        if (build_add_joker(&app->build, joker)) set_status(app, "The Soul trouxe %s para a run.", joker_name(joker));
        else set_status(app, "The Soul encontrou um Coringa lendario, mas faltou espaco.");
    }

    if (from_inventory) {
        consume_tarot_from_inventory(app, inventory_index, tarot);
    }
}

static void open_tarot_apply(AppState *app, TarotType tarot, int from_inventory, int inventory_index, int use_sample_deck, ScreenId return_screen) {
    memset(&app->tarot_apply, 0, sizeof(app->tarot_apply));
    app->tarot_apply.active = 1;
    app->tarot_apply.tarot = tarot;
    app->tarot_apply.from_inventory = from_inventory;
    app->tarot_apply.inventory_index = inventory_index;
    app->tarot_apply.target_count_required = tarot_target_count(tarot);
    app->tarot_apply.use_sample_deck = use_sample_deck;
    app->tarot_apply.return_screen = return_screen;

    if (use_sample_deck) {
        int indices[DECK_SIZE];
        for (int i = 0; i < DECK_SIZE; i++) indices[i] = i;
        for (int i = DECK_SIZE - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmp = indices[i];
            indices[i] = indices[j];
            indices[j] = tmp;
        }
        app->tarot_apply.sample_count = min_int(MAX_TAROT_SAMPLE, DECK_SIZE);
        for (int i = 0; i < app->tarot_apply.sample_count; i++) {
            app->tarot_apply.sample_master_indices[i] = indices[i];
            app->tarot_apply.sample_cards[i] = app->master_deck[indices[i]];
        }
    } else {
        app->tarot_apply.sample_count = app->hand_count;
        for (int i = 0; i < app->hand_count; i++) {
            app->tarot_apply.sample_master_indices[i] = app->hand_sources[i];
            app->tarot_apply.sample_cards[i] = app->hand[i];
        }
    }
    app->screen = SCREEN_TAROT_APPLY;
    if (tarot == TAROT_DEATH) set_status(app, "%s: selecione exatamente 2 cartas.", tarot_display_name(tarot));
    else set_status(app, "%s: selecione ate %d carta(s).", tarot_display_name(tarot), app->tarot_apply.target_count_required);
}

static void apply_tarot_to_card(Card *card, TarotType tarot) {
    if (tarot == TAROT_MAGICIAN) card->enhancement = ENHANCEMENT_LUCKY;
    else if (tarot == TAROT_EMPRESS) card->enhancement = ENHANCEMENT_MULT;
    else if (tarot == TAROT_HIEROPHANT) card->enhancement = ENHANCEMENT_BONUS;
    else if (tarot == TAROT_LOVERS) card->enhancement = ENHANCEMENT_WILD;
    else if (tarot == TAROT_CHARIOT) {
        if (card->rank != 1) card->rank = min_int(13, card->rank + 2);
    } else if (tarot == TAROT_JUSTICE) card->enhancement = ENHANCEMENT_GLASS;
    else if (tarot == TAROT_DEVIL) {
        int roll = rand() % 4;
        card->enhancement = roll == 0 ? ENHANCEMENT_BONUS : roll == 1 ? ENHANCEMENT_MULT : roll == 2 ? ENHANCEMENT_LUCKY : ENHANCEMENT_GLASS;
    } else if (tarot == TAROT_TOWER) {
        card->enhancement = ENHANCEMENT_GLASS;
        if (card->rank != 1) card->rank = min_int(13, card->rank + 1);
    } else if (tarot == TAROT_STAR) card->suit = SUIT_ESPADAS;
    else if (tarot == TAROT_MOON) card->suit = SUIT_COPAS;
    else if (tarot == TAROT_SUN) card->suit = SUIT_OUROS;
    else if (tarot == TAROT_WORLD) card->suit = SUIT_PAUS;
}

static void confirm_tarot_apply(AppState *app) {
    int chosen[MAX_TAROT_SAMPLE];
    int chosen_count = 0;
    for (int i = 0; i < app->tarot_apply.sample_count; i++) if (app->tarot_apply.selected[i]) chosen[chosen_count++] = i;
    if (chosen_count <= 0) {
        set_status(app, "Selecione as cartas necessarias.");
        return;
    }
    if (app->tarot_apply.tarot == TAROT_DEATH && chosen_count != 2) {
        set_status(app, "A Morte precisa de 2 cartas selecionadas.");
        return;
    }

    if (app->tarot_apply.tarot == TAROT_STRENGTH) {
        for (int i = 0; i < chosen_count; i++) {
            int sample_index = chosen[i];
            int master_index = app->tarot_apply.sample_master_indices[sample_index];
            Card card = app->master_deck[master_index];
            if (card.rank >= 2 && card.rank < 13) card.rank++;
            update_master_card(app, master_index, card);
        }
        set_status(app, "A Forca reforcou %d carta(s).", chosen_count);
    } else if (app->tarot_apply.tarot == TAROT_HANGED_MAN) {
        for (int i = 0; i < chosen_count; i++) {
            int sample_index = chosen[i];
            int master_index = app->tarot_apply.sample_master_indices[sample_index];
            Card card = app->master_deck[master_index];
            if (card.rank != 1) card.rank = max_int(1, card.rank - 2);
            card.enhancement = ENHANCEMENT_NONE;
            update_master_card(app, master_index, card);
        }
        set_status(app, "O Enforcado drenou %d carta(s).", chosen_count);
    } else if (app->tarot_apply.tarot == TAROT_DEATH) {
        int target_index = app->tarot_apply.sample_master_indices[chosen[0]];
        int source_index = app->tarot_apply.sample_master_indices[chosen[1]];
        Card source = app->master_deck[source_index];
        update_master_card(app, target_index, source);
        set_status(app, "A Morte fez a primeira carta copiar a segunda.");
    } else {
        for (int i = 0; i < chosen_count; i++) {
            int sample_index = chosen[i];
            int master_index = app->tarot_apply.sample_master_indices[sample_index];
            Card card = app->master_deck[master_index];
            apply_tarot_to_card(&card, app->tarot_apply.tarot);
            update_master_card(app, master_index, card);
        }
        set_status(app, "%s aplicado com sucesso.", tarot_display_name(app->tarot_apply.tarot));
    }

    if (app->tarot_apply.from_inventory) {
        consume_tarot_from_inventory(app, app->tarot_apply.inventory_index, app->tarot_apply.tarot);
    } else {
        app->build.last_used_tarot = app->tarot_apply.tarot;
        app->phase.tarot_used_this_phase = 1;
        app->stats.tarots_used++;
        if (app->tarot_apply.tarot > TAROT_NONE && app->tarot_apply.tarot < GUI_TAROT_TEXTURE_COUNT) app->stats.tarot_use_counts[app->tarot_apply.tarot]++;
    }

    app->tarot_apply.active = 0;
    app->screen = app->tarot_apply.return_screen;
}

static void init_phase(AppState *app) {
    memset(&app->phase, 0, sizeof(app->phase));
    app->phase.phase = app->phase_number;
    app->phase.boss = (app->phase_number % 3 == 0);
    {
        int ante_index = (app->phase_number - 1) / 3;
        int ante = ante_index + 1;
        int base_goal = ante_base_goal(ante);
        int blind_kind = blind_kind_from_phase(app->phase_number);
        if (app->current_ante != ante) {
            app->current_ante = ante;
            app->ante_played_blinds = 0;
            app->ante_correct_questions = 0;
        }
        if (blind_kind == 0) app->phase.goal = base_goal;
        else if (blind_kind == 1) app->phase.goal = (base_goal * 3) / 2;
        else app->phase.goal = base_goal * 2;
    }
    app->phase.hands_left = 4 + app->next_phase_hand_bonus + (app->build.coupon_levels[1] == 1 ? 1 : app->build.coupon_levels[1] >= 2 ? 2 : 0);
    app->next_phase_hand_bonus = 0;
    app->phase.discards_left = 2;
    app->phase.next_play_xmult = 1.0;
    app->phase.first_hand = 1;
    app->phase.blessed_suit = -1;
    if (app->phase.boss) unlock_collection_boss(app, app->phase_number);

    if (app->build.coupon_levels[2] >= 2) {
        TarotType bonus_tarot = random_tarot_gui();
        if (build_add_tarot(&app->build, bonus_tarot)) unlock_collection_tarot(app, bonus_tarot);
    } else if (app->build.coupon_levels[2] == 1 && app->phase_number % 2 == 0) {
        TarotType bonus_tarot = random_tarot_gui();
        if (build_add_tarot(&app->build, bonus_tarot)) unlock_collection_tarot(app, bonus_tarot);
    }

    build_phase_deck_from_master(app);
    fill_hand(app);
    schedule_phase_mark(app);
    app->selected_joker_index = -1;
    app->selected_tarot_index = -1;
    set_status(app, "Fase %d iniciada. Meta: %d pontos.", app->phase_number, app->phase.goal);
}

static void setup_reward_screen(AppState *app, ScreenId next_screen, QuizMode quiz_mode, int joker_bonus) {
    memset(&app->reward, 0, sizeof(app->reward));
    app->reward.active = 1;
    app->reward.base_reward = blind_reward_from_phase(app->phase_number);
    app->reward.hand_bonus = app->phase.hands_left;
    app->reward.interest = min_int(app->build.coins / 5, 5);
    app->reward.joker_bonus = joker_bonus;
    app->reward.total = app->reward.base_reward + app->reward.hand_bonus + app->reward.interest + app->reward.joker_bonus;
    app->reward.anim = 0.0f;
    app->reward.next_screen = next_screen;
    app->reward.next_quiz_mode = quiz_mode;
    snprintf(app->reward.blind_name, sizeof(app->reward.blind_name), "%s", blind_name_from_phase(app->phase_number));
    app->screen = SCREEN_REWARD;
    app->stats.phases_cleared++;
    app->stats.antes_cleared = ((app->phase_number - 1) / 3) + 1;
}

static void phase_cleared(AppState *app) {
    int joker_bonus = (has_joker(&app->build, JOKER_COIN_BOSS) && app->phase.boss) ? 5 : 0;
    ScreenId next_screen = app->phase_number >= MAX_PHASES ? SCREEN_END : app->phase.boss ? SCREEN_SHOP : SCREEN_QUIZ;
    QuizMode next_quiz = app->phase.boss ? QUIZ_BOSS : QUIZ_POST_PHASE;
    if (app->phase.hands_played_this_phase > 0) app->ante_played_blinds++;
    for (int i = 0; i < app->build.joker_count; i++) {
        JokerType type = app->build.jokers[i].type;
        if (type > JOKER_NONE && type < GUI_JOKER_TEXTURE_COUNT) app->stats.joker_use_counts[type]++;
    }
    update_temp_jokers_after_phase(&app->build);
    setup_reward_screen(app, next_screen, next_quiz, joker_bonus);
    if (app->phase.boss) {
        snprintf(app->stats.defeated_by, sizeof(app->stats.defeated_by), "%s", blind_boss_name_from_phase(app->phase_number));
    }
}

static void save_current_run_record(AppState *app) {
    RunRecord records[PLAYER_MAX_RECORDS];
    RunRecord top[PLAYER_TOP_LIMIT];
    LeaderboardMode mode;
    int record_count;

    if (!app || app->last_run_saved || !player_is_linked(app)) return;

    memset(&app->last_record, 0, sizeof(app->last_record));
    player_fill_timestamp(app->last_record.timestamp, sizeof(app->last_record.timestamp));
    player_copy_clean(app->last_record.player, sizeof(app->last_record.player), app->active_player);
    player_copy_clean(app->last_record.run_code, sizeof(app->last_record.run_code), app->stats.run_code);
    player_copy_clean(app->last_record.modality, sizeof(app->last_record.modality), player_modality_name_from_mask(app->question_mask));
    player_copy_clean(app->last_record.defeated_by, sizeof(app->last_record.defeated_by), app->stats.defeated_by);
    app->last_record.question_mask = app->question_mask;
    app->last_record.total_points = app->stats.total_points;
    app->last_record.best_hand_score = app->stats.best_hand_score;
    app->last_record.phases_cleared = app->stats.phases_cleared;
    app->last_record.antes_cleared = app->stats.antes_cleared;
    app->last_record.hands_played = app->stats.hands_played;
    app->last_record.cards_discarded = app->stats.cards_discarded;
    app->last_record.cards_bought = app->stats.cards_bought;
    app->last_record.rerolls = app->stats.rerolls;
    app->last_record.packs_opened = app->stats.packs_opened;
    app->last_record.tarots_used = app->stats.tarots_used;
    app->last_record.quiz_hits = app->build.quiz_hits;
    app->last_record.coins = app->build.coins;
    app->last_record.run_won = app->run_won;
    app->last_record.bet_reached = ((app->phase_number - 1) / 3) + 1;
    app->last_record.blind_reached = blind_kind_from_phase(app->phase_number) + 1;

    if (!player_append_run_record(PLAYER_HISTORY_FILE, &app->last_record)) {
        set_status(app, "Nao foi possivel gravar o historico desta run.");
        return;
    }

    app->last_run_saved = 1;
    mode = player_leaderboard_mode_from_mask(app->question_mask);
    record_count = player_load_run_records(PLAYER_HISTORY_FILE, records, PLAYER_MAX_RECORDS);
    app->last_rank_total = player_build_leaderboard(records, record_count, mode, top, PLAYER_TOP_LIMIT, &app->last_record, &app->last_rank_position);
}

static void finish_run(AppState *app, int won) {
    app->run_won = won;
    app->screen = SCREEN_END;
    if (won) snprintf(app->stats.defeated_by, sizeof(app->stats.defeated_by), "Nenhum");
    else if (app->phase.boss) snprintf(app->stats.defeated_by, sizeof(app->stats.defeated_by), "%s", blind_boss_name_from_phase(app->phase_number));
    else snprintf(app->stats.defeated_by, sizeof(app->stats.defeated_by), "%s", blind_name_from_phase(app->phase_number));
    save_current_run_record(app);
    set_status(app, won ? "Run concluida com sucesso." : "Run encerrada.");
}

static void start_quiz(AppState *app, QuizMode mode) {
    QuizLogEntry *entry;
    app->quiz_mode = mode;
    if (app->pending_failed_question) {
        app->current_question = app->failed_question;
        app->current_quiz_log_index = app->pending_quiz_log_index;
    } else {
        app->current_question = draw_random_question(app, mode);
        app->current_quiz_log_index = app->quiz_log_count < MAX_QUIZ_LOG ? app->quiz_log_count++ : MAX_QUIZ_LOG - 1;
        entry = &app->quiz_log[app->current_quiz_log_index];
        memset(entry, 0, sizeof(*entry));
        entry->mode = mode;
        entry->phase_number = app->phase_number;
        entry->bet = ((app->phase_number - 1) / 3) + 1;
        entry->blind = blind_kind_from_phase(app->phase_number) + 1;
        entry->attempts = 0;
        entry->correct = 0;
        snprintf(entry->title, sizeof(entry->title), "%s da Aposta %d/8", mode == QUIZ_SKIP ? "Pulo" : blind_name_from_phase(app->phase_number), entry->bet);
        snprintf(entry->category, sizeof(entry->category), "%s", ray_quiz_category_name(app->current_question.category));
        snprintf(entry->difficulty, sizeof(entry->difficulty), "%s", ray_quiz_difficulty_name(app->current_question.difficulty));
        snprintf(entry->question, sizeof(entry->question), "%s", app->current_question.question);
        snprintf(entry->hint, sizeof(entry->hint), "%s", app->current_question.hint);
    }
    app->screen = SCREEN_QUIZ;
}

static void record_quiz_result(AppState *app, int answer, int correct) {
    QuizLogEntry *entry;
    if (!app || app->current_quiz_log_index < 0 || app->current_quiz_log_index >= app->quiz_log_count) return;
    if (answer >= 1 && answer <= 4) app->stats.quiz_answer_counts[answer - 1]++;
    if (correct) app->stats.quiz_correct_answers++;
    entry = &app->quiz_log[app->current_quiz_log_index];
    entry->attempts++;
    entry->correct = correct;
    if (correct) {
        app->pending_failed_question = 0;
        app->pending_quiz_log_index = -1;
        app->ante_correct_questions++;
    } else {
        app->pending_failed_question = 1;
        app->failed_question = app->current_question;
        app->pending_quiz_log_index = app->current_quiz_log_index;
    }
}

static void continue_after_reward(AppState *app) {
    app->build.coins += app->reward.total;
    set_status(app, "Lucro da fase coletado: $%d.", app->reward.total);
    if (app->reward.next_screen == SCREEN_QUIZ) {
        start_quiz(app, app->reward.next_quiz_mode);
    } else if (app->reward.next_screen == SCREEN_SHOP) {
        build_shop(app);
        app->screen = SCREEN_SHOP;
    } else if (app->reward.next_screen == SCREEN_END) {
        finish_run(app, 1);
    }
}

static void start_run(AppState *app) {
    if (!player_is_linked(app)) {
        prime_profile_from_active(app);
        app->screen = SCREEN_PROFILE;
        set_status(app, "Antes da run, vincule um nome e escolha as categorias de pergunta.");
        return;
    }
    memset(&app->build, 0, sizeof(app->build));
    build_init(&app->build);
    load_active_collection(app);
    app->phase_number = 1;
    app->pending_mark_count = 0;
    app->phase_mark_available = 0;
    app->run_won = 0;
    memset(&app->stats, 0, sizeof(app->stats));
    app->stats.best_hand_type = HAND_HIGH_CARD;
    memset(app->quiz_log, 0, sizeof(app->quiz_log));
    app->quiz_log_count = 0;
    app->pending_failed_question = 0;
    app->pending_quiz_log_index = -1;
    app->current_quiz_log_index = -1;
    app->dragging_card_index = -1;
    app->dragging_joker_index = -1;
    app->drag_started = 0;
    app->current_ante = 0;
    app->ante_played_blinds = 0;
    app->ante_correct_questions = 0;
    app->next_phase_hand_bonus = 0;
    app->free_pack_credits = 0;
    app->free_rerolls = 0;
    app->last_run_saved = 0;
    app->last_rank_position = -1;
    app->last_rank_total = 0;
    memset(&app->last_record, 0, sizeof(app->last_record));
    seed_run_code(app);
    init_master_deck(app);
    init_phase(app);
    app->screen = SCREEN_ROUND_PREVIEW;
}

static void next_phase(AppState *app) {
    app->phase_number++;
    if (app->phase_number > MAX_PHASES) {
        finish_run(app, 1);
        return;
    }
    init_phase(app);
    app->screen = SCREEN_ROUND_PREVIEW;
}

static void enter_current_phase(AppState *app) {
    if (app->phase.boss) start_quiz(app, QUIZ_BOSS);
    else app->screen = SCREEN_PLAY;
}

static void update_temp_jokers_after_play(PlayerBuild *build) {
    for (int i = 0; i < build->joker_count; i++) {
        JokerInstance *j = &build->jokers[i];
        if (!j->active) continue;
        if (j->type == JOKER_POPCORN) {
            j->value -= 4;
            if (j->value <= 0) j->active = 0;
        } else if (j->type == JOKER_ICE_CREAM) {
            j->value -= 5;
            if (j->value <= 0) j->active = 0;
        } else if (j->type == JOKER_LUNCHBOX) {
            j->counter--;
            if (j->counter <= 0) j->active = 0;
        }
    }
    compact_jokers(build);
}

static void update_temp_jokers_after_phase(PlayerBuild *build) {
    for (int i = 0; i < build->joker_count; i++) {
        JokerInstance *j = &build->jokers[i];
        if (!j->active) continue;
        if (j->type == JOKER_CAVENDISH) {
            if ((rand() % 8) == 0) j->active = 0;
        } else if (j->type == JOKER_COFFEE) {
            j->counter--;
            if (j->counter <= 0) j->active = 0;
        }
    }
    compact_jokers(build);
}

static void apply_discard_effects(AppState *app, Card discarded[], int count) {
    int low_cards = count_lows(discarded, count);
    int royal_cards = count_royals(discarded, count);
    int same_suit = 0;
    for (int suit = 0; suit < 4; suit++) {
        int match = count_matching_suit(discarded, count, suit);
        if (match > same_suit) same_suit = match;
    }

    if (has_joker(&app->build, JOKER_COIN_LOW) && low_cards > 0) app->build.coins += 1;
    if (has_joker(&app->build, JOKER_COIN_ROYAL) && royal_cards > 0) app->build.coins += royal_cards;
    if (same_suit >= 3) app->build.coins += 2;

    app->phase.discards_used += count;
    app->stats.cards_discarded += count;
    if (!app->phase.six_sense_paid && has_joker(&app->build, JOKER_SIXTH_SENSE) && app->phase.discards_used >= 6) {
        TarotType bonus_tarot = random_tarot_gui();
        if (build_add_tarot(&app->build, bonus_tarot)) {
            app->phase.six_sense_paid = 1;
            unlock_collection_tarot(app, bonus_tarot);
            set_status(app, "Sexto Senso criou 1 Tarot.");
        }
    }
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

static PlayOutcome calculate_outcome(AppState *app, Card selected[], int count, ActivationTrace *trace, SelectionPreview *preview) {
    return calculate_outcome_internal(app, selected, count, &app->phase, trace, preview, 1);
}

static void rebuild_selection_preview(AppState *app) {
    int write = 0;
    Card selected_cards[MAX_PLAY];
    PhaseState phase_copy = app->phase;

    memset(&app->preview, 0, sizeof(app->preview));
    for (int i = 0; i < app->hand_count; i++) {
        if (app->selected[i] && write < MAX_PLAY) {
            selected_cards[write++] = app->hand[i];
        }
    }
    if (write <= 0) return;

    calculate_outcome_internal(app, selected_cards, write, &phase_copy, NULL, &app->preview, 0);
}

static void trigger_activation_pulses(AppState *app, int indices[], int count, const ActivationTrace *trace) {
    if (!trace) return;
    for (int i = 0; i < count; i++) {
        queue_pulse(&app->hand_pulses[indices[i]], max_int(1, trace->card_hits[i]));
    }
    for (int i = 0; i < app->build.joker_count && i < MAX_JOKERS; i++) {
        queue_pulse(&app->joker_pulses[i], trace->joker_hits[i]);
    }
    for (int i = 0; i < 4; i++) {
        queue_pulse(&app->counter_pulses[i], trace->counter_hits[i]);
    }
}

static void play_selected_cards(AppState *app) {
    int count = count_selected_cards(app);
    Card selected_cards[MAX_PLAY];
    int indices[MAX_PLAY];
    int write = 0;
    ActivationTrace trace;
    SelectionPreview preview;

    if (count <= 0 || count > MAX_PLAY) {
        set_status(app, "Selecione de 1 a 5 cartas.");
        return;
    }
    if (app->phase.hands_left <= 0) {
        set_status(app, "Sem maos restantes.");
        return;
    }

    for (int i = 0; i < app->hand_count; i++) {
        if (app->selected[i] && write < MAX_PLAY) {
            selected_cards[write] = app->hand[i];
            indices[write] = i;
            write++;
        }
    }

    {
        PlayOutcome outcome = calculate_outcome(app, selected_cards, count, &trace, &preview);
        app->phase.score += outcome.points;
        app->stats.total_points += outcome.points;
        app->phase.hands_left--;
        app->phase.hands_played_this_phase++;
        app->build.coins += outcome.coins;
        app->phase.first_hand = 0;
        app->stats.hands_played++;
        app->stats.hand_play_counts[outcome.eval.type]++;
        if (outcome.points > app->stats.best_hand_score) {
            app->stats.best_hand_score = outcome.points;
            app->stats.best_hand_type = outcome.eval.type;
        }
        maybe_level_up_hand(&app->build, outcome.eval.type, app->status, sizeof(app->status));
        set_status(app, "%s rendeu %d pontos%s.", outcome.eval.name, outcome.points, outcome.coins > 0 ? " e moedas bonus" : "");
        app->preview = preview;
        trigger_activation_pulses(app, indices, count, &trace);
    }

    remove_and_draw(app, indices, count);
    update_temp_jokers_after_play(&app->build);

    if (app->phase.score >= app->phase.goal) phase_cleared(app);
    else if (app->phase.hands_left <= 0) finish_run(app, 0);
}

static void discard_selected_cards(AppState *app) {
    int count = count_selected_cards(app);
    Card discarded[MAX_PLAY];
    int indices[MAX_PLAY];
    int write = 0;

    if (count <= 0 || count > MAX_PLAY) {
        set_status(app, "Selecione de 1 a 5 cartas para descartar.");
        return;
    }
    if (app->phase.discards_left <= 0) {
        set_status(app, "Sem descartes restantes.");
        return;
    }

    for (int i = 0; i < app->hand_count; i++) {
        if (app->selected[i] && write < MAX_PLAY) {
            discarded[write] = app->hand[i];
            indices[write] = i;
            write++;
        }
    }

    app->phase.discards_left--;
    apply_discard_effects(app, discarded, count);
    remove_and_draw(app, indices, count);
    set_status(app, "Cartas descartadas.");
}

static void sort_hand(AppState *app, int by_suit) {
    for (int i = 0; i < app->hand_count - 1; i++) {
        for (int j = i + 1; j < app->hand_count; j++) {
            int swap = 0;
            if (!by_suit && app->hand[j].rank < app->hand[i].rank) swap = 1;
            if (by_suit && (app->hand[j].suit < app->hand[i].suit ||
                (app->hand[j].suit == app->hand[i].suit && app->hand[j].rank < app->hand[i].rank))) swap = 1;
            if (swap) {
                Card tmp_card = app->hand[i];
                int tmp_src = app->hand_sources[i];
                app->hand[i] = app->hand[j];
                app->hand_sources[i] = app->hand_sources[j];
                app->hand[j] = tmp_card;
                app->hand_sources[j] = tmp_src;
            }
        }
    }
    set_status(app, by_suit ? "Mao organizada por naipe." : "Mao organizada por classe.");
}

static void open_inventory_tarot(AppState *app, int tarot_index) {
    TarotType tarot;
    if (tarot_index < 0 || tarot_index >= app->build.tarot_count) return;
    tarot = app->build.tarot_inventory[tarot_index];
    unlock_collection_tarot(app, tarot);
    if (tarot_can_use_anytime(tarot)) {
        apply_non_target_tarot(app, tarot, 1, tarot_index);
        return;
    }
    if (app->screen != SCREEN_PLAY) {
        set_status(app, "%s so pode ser usado durante a fase.", tarot_display_name(tarot));
        return;
    }
    open_tarot_apply(app, tarot, 1, tarot_index, 0, SCREEN_PLAY);
}

static int selected_tarot_is_usable(const AppState *app, char *reason, size_t reason_size) {
    TarotType tarot;
    int selected_count;
    int limit;
    if (!app || app->selected_tarot_index < 0 || app->selected_tarot_index >= app->build.tarot_count) {
        if (reason) snprintf(reason, reason_size, "Selecione um Tarot.");
        return 0;
    }
    tarot = app->build.tarot_inventory[app->selected_tarot_index];
    if (tarot_can_use_anytime(tarot)) return 1;
    selected_count = count_selected_cards(app);
    limit = tarot_target_count(tarot);
    if (selected_count <= 0) {
        if (reason) snprintf(reason, reason_size, "Selecione cartas antes de usar.");
        return 0;
    }
    if (tarot == TAROT_DEATH && selected_count != 2) {
        if (reason) snprintf(reason, reason_size, "A Morte exige exatamente 2 cartas.");
        return 0;
    }
    if (tarot != TAROT_DEATH && selected_count > limit) {
        if (reason) snprintf(reason, reason_size, "Este Tarot aceita ate %d carta(s).", limit);
        return 0;
    }
    return 1;
}

static void apply_selected_tarot_on_hand(AppState *app) {
    int chosen_indices[MAX_PLAY];
    int chosen_sources[MAX_PLAY];
    int chosen_count = 0;
    TarotType tarot;
    char reason[128];

    if (!selected_tarot_is_usable(app, reason, sizeof(reason))) {
        set_status(app, "%s", reason);
        return;
    }
    tarot = app->build.tarot_inventory[app->selected_tarot_index];
    unlock_collection_tarot(app, tarot);

    if (tarot_can_use_anytime(tarot)) {
        apply_non_target_tarot(app, tarot, 1, app->selected_tarot_index);
        app->selected_tarot_index = -1;
        return;
    }

    for (int i = 0; i < app->hand_count && chosen_count < MAX_PLAY; i++) {
        if (app->selected[i]) {
            chosen_indices[chosen_count] = i;
            chosen_sources[chosen_count] = app->hand_sources[i];
            chosen_count++;
        }
    }

    if (tarot == TAROT_DEATH) {
        Card source = app->master_deck[chosen_sources[1]];
        update_master_card(app, chosen_sources[0], source);
        set_status(app, "A Morte fez a primeira carta selecionada copiar a segunda.");
    } else {
        for (int i = 0; i < chosen_count; i++) {
            Card card = app->master_deck[chosen_sources[i]];
            if (tarot == TAROT_STRENGTH) {
                if (card.rank >= 2 && card.rank < 13) card.rank++;
            } else if (tarot == TAROT_HANGED_MAN) {
                if (card.rank != 1) card.rank = max_int(1, card.rank - 2);
                card.enhancement = ENHANCEMENT_NONE;
            } else {
                apply_tarot_to_card(&card, tarot);
            }
            update_master_card(app, chosen_sources[i], card);
            queue_pulse(&app->hand_pulses[chosen_indices[i]], 2);
        }
        set_status(app, "%s aplicado em %d carta(s).", tarot_display_name(tarot), chosen_count);
    }

    consume_tarot_from_inventory(app, app->selected_tarot_index, tarot);
    app->build.last_used_tarot = tarot;
    app->phase.tarot_used_this_phase = 1;
    app->stats.tarots_used++;
    app->selected_tarot_index = -1;
    memset(app->selected, 0, sizeof(app->selected));
}

static void answer_quiz(AppState *app, int answer) {
    int correct = answer == app->current_question.answer;
    record_quiz_result(app, answer, correct);
    if (app->quiz_mode == QUIZ_SKIP) {
        if (correct) {
            char reward_label[96];
            snprintf(reward_label, sizeof(reward_label), "%s", app->phase_skip_reward.label);
            apply_skip_reward(app);
            app->stats.phases_cleared++;
            next_phase(app);
            set_status(app, "Desafio dificil concluido. Recompensa: %s.", reward_label);
        } else {
            app->screen = SCREEN_PLAY;
            set_status(app, "Pergunta dificil errada. A fase precisa ser jogada.");
        }
        return;
    }

    if (app->quiz_mode == QUIZ_BOSS) {
        if (correct) {
            app->phase.question_correct = 1;
            app->build.quiz_hits++;
            app->build.quiz_streak++;
            app->build.coins += 3;
            set_status(app, "Pergunta do chefe correta. O blind foi neutralizado.");
        } else {
            app->build.quiz_streak = 0;
            apply_boss_failure_effect(app);
            set_status(app, "Pergunta do chefe errada. %s ativou: %s", blind_boss_name_from_phase(app->phase_number), blind_boss_desc_from_phase(app->phase_number));
        }
        app->screen = SCREEN_PLAY;
        return;
    }

    if (correct) {
        app->build.quiz_hits++;
        app->build.quiz_streak++;
        app->build.coins += 2;
        if (app->phase_mark_available) {
            queue_mark_reward(app, app->phase_mark_reward, "Pergunta correta. Uma oferta futura recebera a marca %s.");
            app->phase_mark_available = 0;
        }
        if ((app->build.quiz_hits % 2) == 0) {
            app->build.coupon_tokens++;
            set_status(app, "Pergunta correta. Cupom liberado para a loja.");
        }
        if (has_joker(&app->build, JOKER_ARCANE_MINOR) && (rand() % 2) == 0) {
            TarotType bonus_tarot = random_tarot_gui();
            if (build_add_tarot(&app->build, bonus_tarot)) unlock_collection_tarot(app, bonus_tarot);
        }
        if (has_joker(&app->build, JOKER_MARKER)) app->build.coins += 2;
    } else {
        app->build.quiz_streak = 0;
        if (has_joker(&app->build, JOKER_MARKER)) app->build.coins += 2;
        set_status(app, "Pergunta errada. Dica: %s", app->current_question.hint);
    }
    build_shop(app);
    app->screen = SCREEN_SHOP;
}

static void buy_offer(AppState *app, Offer *offer) {
    int use_free_pack = 0;
    if (!offer || offer->sold || offer->type == OFFER_NONE) return;
    if (offer->type == OFFER_PACK && app->free_pack_credits > 0) use_free_pack = 1;
    if (!use_free_pack && app->build.coins < offer->price) {
        set_status(app, "Moedas insuficientes.");
        return;
    }

    if (offer->type == OFFER_JOKER) {
        if (!build_add_joker_offer(&app->build, offer->joker, offer->edition, offer->price)) {
            set_status(app, "Sem espaco para Coringa.");
            return;
        }
        app->build.coins -= offer->price;
        unlock_collection_joker(app, offer->joker);
        if (offer->edition != EDITION_NONE) unlock_collection_mark(app, offer->edition);
        if (offer->edition != EDITION_NONE) set_status(app, "Comprado: %s com marca %s.", joker_name(offer->joker), gui_edition_name(offer->edition));
        else set_status(app, "Comprado: %s.", joker_name(offer->joker));
        app->stats.cards_bought++;
        offer->sold = 1;
    } else if (offer->type == OFFER_TAROT) {
        if (!build_add_tarot(&app->build, offer->tarot)) {
            set_status(app, "Inventario de Tarot cheio.");
            return;
        }
        app->build.coins -= offer->price;
        unlock_collection_tarot(app, offer->tarot);
        app->stats.cards_bought++;
        offer->sold = 1;
        set_status(app, "Comprado: %s.", tarot_display_name(offer->tarot));
    } else if (offer->type == OFFER_COUPON) {
        if (app->build.coupon_tokens <= 0) {
            set_status(app, "Nenhum Cupom liberado ainda.");
            return;
        }
        if (app->ante_played_blinds <= 0 || app->ante_correct_questions < 2) {
            set_status(app, "Cupom exige 1 blind jogada e 2 perguntas corretas na aposta atual.");
            return;
        }
        if (!build_add_coupon(&app->build, offer->coupon)) {
            set_status(app, "Voce ja possui esse Cupom.");
            return;
        }
        app->build.coins -= offer->price;
        app->build.coupon_tokens--;
        unlock_collection_coupon(app, offer->coupon);
        offer->sold = 1;
        set_status(app, "Comprado: %s.", coupon_name(offer->coupon));
    } else if (offer->type == OFFER_PACK) {
        if (use_free_pack) app->free_pack_credits--;
        else app->build.coins -= offer->price;
        offer->sold = 1;
        app->stats.packs_opened++;
        set_pack_choices(app, offer->pack_kind);
        app->screen = SCREEN_PACK;
        set_status(app, "%s aberto.", pack_name(offer->pack_kind));
    }
}

static void reroll_shop(AppState *app) {
    if (app->free_rerolls > 0) {
        app->free_rerolls--;
        app->stats.rerolls++;
        build_shop_offer(app, &app->shop.slots[0], app->pending_mark_count > 0 ? pop_pending_mark(app) : EDITION_NONE);
        build_shop_offer(app, &app->shop.slots[1], app->pending_mark_count > 0 ? pop_pending_mark(app) : EDITION_NONE);
        set_status(app, "Loja atualizada de graca. Restam %d.", app->free_rerolls);
        return;
    }
    if (app->build.coins < app->shop.reroll_cost) {
        set_status(app, "Moedas insuficientes para atualizar a loja.");
        return;
    }
    app->build.coins -= app->shop.reroll_cost;
    app->stats.rerolls++;
    build_shop_offer(app, &app->shop.slots[0], app->pending_mark_count > 0 ? pop_pending_mark(app) : EDITION_NONE);
    build_shop_offer(app, &app->shop.slots[1], app->pending_mark_count > 0 ? pop_pending_mark(app) : EDITION_NONE);
    app->shop.reroll_cost++;
    set_status(app, "Loja atualizada.");
}

static void sell_selected_joker(AppState *app) {
    int idx = app->selected_joker_index;
    if (idx < 0 || idx >= app->build.joker_count) return;
    build_sell_joker(&app->build, idx);
    app->selected_joker_index = -1;
    set_status(app, "Coringa vendido.");
}

static void choose_pack_offer(AppState *app, int idx) {
    Offer *offer;
    if (idx < 0 || idx >= app->pack.choice_count) return;
    offer = &app->pack.choices[idx];
    if (offer->sold) return;

    if (offer->type == OFFER_JOKER) {
        if (build_add_joker_offer(&app->build, offer->joker, offer->edition, joker_base_price(offer->joker))) {
            app->stats.cards_bought++;
            unlock_collection_joker(app, offer->joker);
            if (offer->edition != EDITION_NONE) unlock_collection_mark(app, offer->edition);
            if (offer->edition != EDITION_NONE) set_status(app, "%s entrou na build com marca %s.", joker_name(offer->joker), gui_edition_name(offer->edition));
            else set_status(app, "%s entrou na build.", joker_name(offer->joker));
        } else {
            set_status(app, "Sem espaco para esse Coringa.");
        }
    } else if (is_tarot_targeted(offer->tarot)) {
        offer->sold = 1;
        app->pack.picks_remaining--;
        open_tarot_apply(app, offer->tarot, 0, -1, 1, app->pack.picks_remaining > 0 ? SCREEN_PACK : SCREEN_SHOP);
        return;
    } else {
        apply_non_target_tarot(app, offer->tarot, 0, -1);
    }

    offer->sold = 1;
    app->pack.picks_remaining--;
    if (app->pack.picks_remaining <= 0) {
        app->pack.active = 0;
        app->screen = SCREEN_SHOP;
    }
}

static void ignore_open_pack(AppState *app) {
    int boosted = 0;
    for (int i = 0; i < app->build.joker_count; i++) {
        if (app->build.jokers[i].type == JOKER_RED_CARD && app->build.jokers[i].active) {
            app->build.jokers[i].value += 3;
            queue_pulse(&app->joker_pulses[i], 2);
            boosted++;
        }
    }

    app->pack.active = 0;
    app->screen = SCREEN_SHOP;
    if (boosted > 0) set_status(app, "Pacote ignorado. Cartao Vermelho ganhou +3 Mult.");
    else set_status(app, "Pacote ignorado.");
}

static Rectangle card_rect(int index, int total, float anim_up, float hover_scale) {
    float spacing = 90.0f;
    float start = 595.0f;
    float x = start + index * spacing;
    float arc = fabsf((float)index - (float)(total - 1) * 0.5f);
    float y = 616.0f + arc * 9.0f - anim_up * 30.0f;
    float w = 108.0f + hover_scale * 10.0f;
    float h = 152.0f + hover_scale * 12.0f;
    return Rf(x - hover_scale * 5.0f, y - hover_scale * 6.0f, w, h);
}

static Rectangle joker_rect(int idx) {
    return Rf(430.0f + idx * 112.0f, 68.0f, 96.0f, 132.0f);
}

static Rectangle joker_rect_for_count(int idx, int total, int selected_idx) {
    float card_w = idx == selected_idx ? 110.0f : 96.0f;
    float card_h = idx == selected_idx ? 148.0f : 132.0f;
    float container_x = 454.0f;
    float container_w = 850.0f;
    float spacing = 112.0f;
    float x;
    float y = idx == selected_idx ? 54.0f : 68.0f;

    if (total > 1) {
        float fit_spacing = (container_w - 96.0f) / (float)(total - 1);
        if (fit_spacing < spacing) spacing = fit_spacing;
        if (spacing < 38.0f) spacing = 38.0f;
    }

    x = container_x + idx * spacing;
    if (selected_idx >= 0 && total > 7) {
        if (idx < selected_idx) x -= 8.0f;
        else if (idx > selected_idx) x += 18.0f;
    }
    if (idx == selected_idx) x -= 7.0f;

    return Rf(x, y, card_w, card_h);
}

static Rectangle joker_hit_rect_for_count(int idx, int total, int selected_idx) {
    Rectangle box = joker_rect_for_count(idx, total, selected_idx);
    if (total > 1 && idx < total - 1 && idx != selected_idx) {
        Rectangle next = joker_rect_for_count(idx + 1, total, selected_idx);
        float visible = next.x - box.x;
        if (visible > 24.0f && visible < box.width) box.width = visible;
    }
    return box;
}

static Rectangle tarot_rect(int idx) {
    return Rf(1320.0f + idx * 112.0f, 68.0f, 96.0f, 138.0f);
}

static void move_card_in_hand(AppState *app, int from, int to) {
    Card card;
    int source;
    int selected;
    float select_anim;
    float hover_anim;
    PulseAnim pulse;

    if (from < 0 || from >= app->hand_count || to < 0 || to >= app->hand_count || from == to) return;

    card = app->hand[from];
    source = app->hand_sources[from];
    selected = app->selected[from];
    select_anim = app->select_anim[from];
    hover_anim = app->hover_anim[from];
    pulse = app->hand_pulses[from];

    if (from < to) {
        for (int i = from; i < to; i++) {
            app->hand[i] = app->hand[i + 1];
            app->hand_sources[i] = app->hand_sources[i + 1];
            app->selected[i] = app->selected[i + 1];
            app->select_anim[i] = app->select_anim[i + 1];
            app->hover_anim[i] = app->hover_anim[i + 1];
            app->hand_pulses[i] = app->hand_pulses[i + 1];
        }
    } else {
        for (int i = from; i > to; i--) {
            app->hand[i] = app->hand[i - 1];
            app->hand_sources[i] = app->hand_sources[i - 1];
            app->selected[i] = app->selected[i - 1];
            app->select_anim[i] = app->select_anim[i - 1];
            app->hover_anim[i] = app->hover_anim[i - 1];
            app->hand_pulses[i] = app->hand_pulses[i - 1];
        }
    }

    app->hand[to] = card;
    app->hand_sources[to] = source;
    app->selected[to] = selected;
    app->select_anim[to] = select_anim;
    app->hover_anim[to] = hover_anim;
    app->hand_pulses[to] = pulse;
}

static void move_joker_in_build(AppState *app, int from, int to) {
    JokerInstance joker;
    PulseAnim pulse;

    if (from < 0 || from >= app->build.joker_count || to < 0 || to >= app->build.joker_count || from == to) return;

    joker = app->build.jokers[from];
    pulse = app->joker_pulses[from];
    if (from < to) {
        for (int i = from; i < to; i++) {
            app->build.jokers[i] = app->build.jokers[i + 1];
            app->joker_pulses[i] = app->joker_pulses[i + 1];
        }
    } else {
        for (int i = from; i > to; i--) {
            app->build.jokers[i] = app->build.jokers[i - 1];
            app->joker_pulses[i] = app->joker_pulses[i - 1];
        }
    }

    app->build.jokers[to] = joker;
    app->joker_pulses[to] = pulse;
    app->selected_joker_index = to;
}

static Color suit_color(int suit) {
    if (suit == SUIT_COPAS) return rgb(199, 30, 78);
    if (suit == SUIT_OUROS) return rgb(227, 149, 54);
    if (suit == SUIT_ESPADAS) return rgb(34, 15, 133);
    return rgb(40, 40, 41);
}

static void draw_background(void) {
    ClearBackground(BLACK);
    for (int i = 0; i < 10; i++) {
        float t = (float)GetTime() * (0.10f + i * 0.018f);
        float cx = 780.0f + cosf(t + i) * (220.0f + i * 12.0f);
        float cy = 430.0f + sinf(t * 1.4f + i) * (160.0f - i * 6.0f);
        Color c = i % 2 == 0 ? Fade(rgb(96, 48, 255), 0.15f) : Fade(rgb(239, 245, 64), 0.08f);
        DrawEllipse((int)cx, (int)cy, 480 - i * 28, 230 - i * 10, c);
    }
    DrawRectangleGradientV(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.02f), Fade(rgb(14, 9, 24), 0.22f));
    for (int y = 0; y < SCREEN_H; y += 4) DrawLine(0, y, SCREEN_W, y, Fade(WHITE, 0.018f));
}

static void draw_button(Rectangle box, const char *label, Color fill, int font_size) {
    draw_panel(box, fill, rgb(58, 66, 74));
    {
        int width = MeasureText(label, font_size);
        DrawText(label,
                 (int)(box.x + box.width * 0.5f - width * 0.5f),
                 (int)(box.y + box.height * 0.5f - font_size * 0.5f),
                 font_size,
                 button_text_color(fill));
    }
}

static void draw_card_visual(Card card, Rectangle box, bool selected) {
    Color s = suit_color(card.suit);
    Color bg = selected ? rgb(248, 249, 252) : WHITE;
    if (g_assets && card.suit >= 0 && card.suit < 4 && card.rank >= 1 && card.rank <= 13 &&
        texture_ready(g_assets->cards[card.suit][card.rank])) {
        draw_panel(box, selected ? rgb(226, 246, 255) : WHITE, selected ? rgb(58, 180, 255) : rgb(194, 197, 204));
        draw_texture_fit(g_assets->cards[card.suit][card.rank], Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
        if (card.enhancement > ENHANCEMENT_NONE && card.enhancement < GUI_CARD_ENH_TEXTURE_COUNT &&
            texture_ready(g_assets->enhanced[card.enhancement])) {
            draw_texture_fit(g_assets->enhanced[card.enhancement], box, Fade(WHITE, 0.86f));
        }
        return;
    }
    if (g_assets && card.suit >= 0 && card.suit < 4 && card.rank >= 1 && card.rank <= 13) {
        Rectangle src = card_sheet_src(card.suit, card.rank);
        if (texture_can_cover(g_assets->card_sheet, src)) {
            draw_panel(box, selected ? rgb(226, 246, 255) : WHITE, selected ? rgb(58, 180, 255) : rgb(194, 197, 204));
            draw_texture_region_fit(g_assets->card_sheet, src, Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
            if (card.enhancement > ENHANCEMENT_NONE && card.enhancement < GUI_CARD_ENH_TEXTURE_COUNT &&
                texture_ready(g_assets->enhanced[card.enhancement])) {
                draw_texture_fit(g_assets->enhanced[card.enhancement], box, Fade(WHITE, 0.86f));
            }
            return;
        }
    }
    draw_panel(box, bg, selected ? rgb(58, 180, 255) : rgb(194, 197, 204));
    DrawText(rank_name(card.rank), (int)box.x + 10, (int)box.y + 8, 26, s);
    DrawText(rank_name(card.rank), (int)box.x + (int)box.width - 24, (int)box.y + (int)box.height - 34, 26, s);
    DrawText(card.suit == SUIT_COPAS ? "H" : card.suit == SUIT_OUROS ? "D" : card.suit == SUIT_ESPADAS ? "S" : "C",
             (int)box.x + (int)box.width / 2 - 16, (int)box.y + 48, 40, s);
    if (card.rank == 11 || card.rank == 12 || card.rank == 13 || card.rank == 1) {
        DrawText(card.rank == 11 ? "VALETE" : card.rank == 12 ? "RAINHA" : card.rank == 13 ? "REI" : "AS",
                 (int)box.x + 10, (int)box.y + 92, 14, s);
    }
    if (card.enhancement != ENHANCEMENT_NONE) {
        Color strip = card.enhancement == ENHANCEMENT_BONUS ? rgb(244, 204, 87)
                    : card.enhancement == ENHANCEMENT_MULT ? rgb(93, 129, 245)
                    : card.enhancement == ENHANCEMENT_LUCKY ? rgb(84, 178, 98)
                    : card.enhancement == ENHANCEMENT_GLASS ? rgb(192, 120, 238)
                    : rgb(236, 132, 68);
        draw_panel(Rf(box.x + 10, box.y + box.height - 28, box.width - 20, 18), strip, Fade(BLACK, 0.35f));
        DrawText(enhancement_name(card.enhancement), (int)box.x + 16, (int)box.y + (int)box.height - 26, 12, rgb(34, 36, 39));
    }
}

static void draw_joker_visual(const Offer *offer, Rectangle box, int selected) {
    Color head = offer->edition == EDITION_FOIL ? rgb(236, 207, 81)
                 : offer->edition == EDITION_CHROME ? rgb(170, 193, 224)
                 : offer->edition == EDITION_PRISMATIC ? rgb(230, 118, 220)
                 : offer->edition == EDITION_NEGATIVE ? rgb(100, 78, 153)
                 : rgb(243, 239, 221);
    if (g_assets && offer->joker > JOKER_NONE && offer->joker < GUI_JOKER_TEXTURE_COUNT &&
        texture_ready(g_assets->jokers[offer->joker])) {
        draw_panel(box, WHITE, selected ? rgb(58, 180, 255) : rgb(76, 84, 92));
        draw_texture_fit(g_assets->jokers[offer->joker], Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
        if (offer->edition > EDITION_NONE && offer->edition < GUI_EDITION_TEXTURE_COUNT &&
            texture_ready(g_assets->editions[offer->edition])) {
            draw_texture_fit(g_assets->editions[offer->edition], box, Fade(WHITE, 0.82f));
        }
        draw_edition_badge(box, offer->edition);
        return;
    }
    if (g_assets && offer->joker > JOKER_NONE) {
        Rectangle src = joker_sheet_src(offer->joker);
        if (texture_can_cover(g_assets->joker_sheet, src)) {
            draw_panel(box, WHITE, selected ? rgb(58, 180, 255) : rgb(76, 84, 92));
            draw_texture_region_fit(g_assets->joker_sheet, src, Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
            draw_edition_badge(box, offer->edition);
            return;
        }
    }
    draw_panel(box, head, selected ? rgb(58, 180, 255) : rgb(76, 84, 92));
    DrawText("JOKER", (int)box.x + 18, (int)box.y + 14, 18, rgb(46, 52, 59));
    DrawText(joker_name(offer->joker), (int)box.x + 8, (int)box.y + 46, 14, rgb(46, 52, 59));
    DrawText(gui_edition_name(offer->edition), (int)box.x + 10, (int)box.y + 90, 15, rgb(46, 52, 59));
    DrawText(joker_rarity_name(offer->joker), (int)box.x + 12, (int)box.y + 110, 14, rgb(46, 52, 59));
    draw_edition_badge(box, offer->edition);
}

static void draw_inventory_joker(const JokerInstance *joker, Rectangle box, int selected) {
    Offer o;
    init_offer(&o);
    o.type = OFFER_JOKER;
    o.joker = joker->type;
    o.edition = joker->edition;
    draw_joker_visual(&o, box, selected);
}

static void draw_tarot_visual(TarotType tarot, Rectangle box, int selected) {
    if (g_assets && tarot > TAROT_NONE && tarot < GUI_TAROT_TEXTURE_COUNT && texture_ready(g_assets->tarots[tarot])) {
        draw_panel(box, rgb(230, 211, 182), selected ? rgb(58, 180, 255) : rgb(113, 91, 68));
        draw_texture_fit(g_assets->tarots[tarot], Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
        return;
    }
    if (g_assets && tarot > TAROT_NONE) {
        Rectangle src = tarot_sheet_src(tarot);
        if (texture_can_cover(g_assets->tarot_sheet, src)) {
            draw_panel(box, rgb(230, 211, 182), selected ? rgb(58, 180, 255) : rgb(113, 91, 68));
            draw_texture_region_fit(g_assets->tarot_sheet, src, Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
            return;
        }
    }
    draw_panel(box, rgb(230, 211, 182), selected ? rgb(58, 180, 255) : rgb(113, 91, 68));
    DrawText("TAROT", (int)box.x + 20, (int)box.y + 18, 16, rgb(90, 70, 53));
    draw_multiline(tarot_display_name(tarot), (int)box.x + 10, (int)box.y + 48, 14, rgb(90, 70, 53));
}

static void draw_coupon_visual(CouponType coupon, Rectangle box) {
    if (g_assets && coupon >= 0 && coupon < GUI_COUPON_TEXTURE_COUNT && texture_ready(g_assets->coupons[coupon])) {
        draw_panel(box, rgb(210, 216, 224), rgb(128, 135, 146));
        draw_texture_fit(g_assets->coupons[coupon], Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
        return;
    }
    if (g_assets && coupon >= 0) {
        Rectangle src = coupon_sheet_src(coupon);
        if (texture_can_cover(g_assets->coupon_sheet, src)) {
            draw_panel(box, rgb(210, 216, 224), rgb(128, 135, 146));
            draw_texture_region_fit(g_assets->coupon_sheet, src, Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
            return;
        }
    }
    draw_panel(box, rgb(210, 216, 224), rgb(128, 135, 146));
    DrawText("VOUCHER", (int)box.x + 16, (int)box.y + 18, 18, rgb(86, 91, 99));
    DrawText("$", (int)box.x + 44, (int)box.y + 52, 52, rgb(204, 157, 158));
    DrawText(coupon_name(coupon), (int)box.x + 10, (int)box.y + 112, 14, rgb(86, 91, 99));
}

static void draw_pack_visual(PackKind kind, Rectangle box) {
    if (g_assets && kind >= 0 && kind <= PACK_ARCANA_5_2 && texture_ready(g_assets->packs[kind])) {
        draw_panel(box, rgb(125, 84, 58), rgb(88, 58, 39));
        draw_texture_fit(g_assets->packs[kind], Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
        return;
    }
    if (g_assets && kind >= 0) {
        Rectangle src = pack_sheet_src(kind);
        if (texture_can_cover(g_assets->pack_sheet, src)) {
            draw_panel(box, rgb(125, 84, 58), rgb(88, 58, 39));
            draw_texture_region_fit(g_assets->pack_sheet, src, Rf(box.x + 4, box.y + 4, box.width - 8, box.height - 8), WHITE);
            return;
        }
    }
    draw_panel(box, rgb(125, 84, 58), rgb(88, 58, 39));
    DrawText("PACK", (int)box.x + 40, (int)box.y + 18, 20, WHITE);
    draw_multiline(pack_name(kind), (int)box.x + 14, (int)box.y + 48, 16, WHITE);
    DrawText(kind <= PACK_BUFFOON_5_2 ? "Coringas" : "Tarots", (int)box.x + 34, (int)box.y + 138, 18, WHITE);
}

static void draw_offer_card(const Offer *offer, Rectangle box) {
    if (offer->sold || offer->type == OFFER_NONE) {
        draw_panel(box, rgb(46, 53, 61), rgb(82, 90, 100));
        DrawText("VAZIO", (int)box.x + 36, (int)box.y + 80, 24, Fade(WHITE, 0.35f));
        return;
    }
    draw_panel(Rf(box.x + 36, box.y - 22, 72, 32), rgb(39, 44, 52), rgb(72, 79, 88));
    DrawText(TextFormat("$%d", offer->price), (int)box.x + 48, (int)box.y - 16, 22, rgb(250, 191, 68));
    if (offer->type == OFFER_JOKER) draw_joker_visual(offer, box, 0);
    else if (offer->type == OFFER_TAROT) draw_tarot_visual(offer->tarot, box, 0);
    else if (offer->type == OFFER_COUPON) draw_coupon_visual(offer->coupon, box);
    else if (offer->type == OFFER_PACK) draw_pack_visual(offer->pack_kind, box);
}

static const char *boss_name_from_index(int index) {
    static const char *names[] = {"", "Pausa", "Ruido", "Silencio", "Mar Revolto", "Nevoa", "Apagao", "Paulada", "Vazio"};
    if (index < 1 || index > 8) return "Chefe";
    return names[index];
}

static JokerEdition collection_mark_edition_from_index(int index) {
    switch (index) {
        case 1: return EDITION_FOIL;
        case 2: return EDITION_CHROME;
        case 3: return EDITION_PRISMATIC;
        case 4: return EDITION_NEGATIVE;
        default: return EDITION_NONE;
    }
}

static unsigned long long collection_bits_for_tab(const AppState *app, CollectionTab tab) {
    if (!app) return 0ULL;
    switch (tab) {
        case COLLECTION_JOKERS: return app->collection.joker_bits;
        case COLLECTION_COUPONS: return app->collection.coupon_bits;
        case COLLECTION_MARKS: return app->collection.mark_bits;
        case COLLECTION_BOSSES: return app->collection.boss_bits;
        case COLLECTION_TAROTS: return app->collection.tarot_bits;
        default: return 0ULL;
    }
}

static int collection_unlocked_count(const AppState *app, CollectionTab tab) {
    if (!app || !app->active_player[0]) return 0;
    return player_collection_count_bits(collection_bits_for_tab(app, tab), collection_tab_total(tab));
}

static int collection_item_is_unlocked(const AppState *app, CollectionTab tab, int item_index) {
    if (!app || !app->active_player[0]) return 0;
    return player_collection_has(collection_bits_for_tab(app, tab), item_index);
}

static const char *collection_item_name(CollectionTab tab, int item_index) {
    switch (tab) {
        case COLLECTION_JOKERS: return joker_name((JokerType)item_index);
        case COLLECTION_COUPONS: return coupon_name((CouponType)(item_index - 1));
        case COLLECTION_MARKS: return gui_edition_name(collection_mark_edition_from_index(item_index));
        case COLLECTION_BOSSES: return boss_name_from_index(item_index);
        case COLLECTION_TAROTS: return tarot_display_name((TarotType)item_index);
        default: return "Carta";
    }
}

static const char *collection_item_desc(CollectionTab tab, int item_index) {
    switch (tab) {
        case COLLECTION_JOKERS: return joker_desc((JokerType)item_index);
        case COLLECTION_COUPONS: return coupon_desc((CouponType)(item_index - 1));
        case COLLECTION_MARKS: return mark_desc(collection_mark_edition_from_index(item_index));
        case COLLECTION_BOSSES: return blind_boss_desc_from_index(item_index);
        case COLLECTION_TAROTS: return tarot_desc((TarotType)item_index);
        default: return "Item da colecao.";
    }
}

static void draw_locked_collection_card(Rectangle box) {
    draw_panel(box, Fade(BLACK, 0.90f), rgb(82, 90, 100));
    DrawText("???", (int)box.x + 48, (int)box.y + 78, 34, Fade(WHITE, 0.70f));
    DrawText("Bloqueado", (int)box.x + 26, (int)box.y + 130, 22, Fade(WHITE, 0.62f));
}

static void draw_collection_item_card(AppState *app, CollectionTab tab, int item_index, Rectangle box) {
    int unlocked = collection_item_is_unlocked(app, tab, item_index);

    if (!unlocked) {
        draw_locked_collection_card(box);
        if (hover(box)) set_tooltip(app, "???\nDescubra este item durante as runs.");
        return;
    }

    if (tab == COLLECTION_JOKERS) {
        Offer offer;
        init_offer(&offer);
        offer.type = OFFER_JOKER;
        offer.joker = (JokerType)item_index;
        draw_joker_visual(&offer, box, 0);
    } else if (tab == COLLECTION_COUPONS) {
        draw_coupon_visual((CouponType)(item_index - 1), box);
    } else if (tab == COLLECTION_MARKS) {
        JokerEdition edition = collection_mark_edition_from_index(item_index);
        draw_panel(box, mark_color(edition), rgb(76, 84, 92));
        DrawText("MARCA", (int)box.x + 26, (int)box.y + 18, 18, rgb(44, 46, 52));
        DrawText(gui_mark_label(edition), (int)box.x + 40, (int)box.y + 86, 34, rgb(44, 46, 52));
        draw_multiline(gui_edition_name(edition), (int)box.x + 18, (int)box.y + 138, 18, rgb(44, 46, 52));
    } else if (tab == COLLECTION_BOSSES) {
        draw_panel(box, rgb(118, 32, 48), rgb(239, 182, 78));
        DrawText("CHEFE", (int)box.x + 28, (int)box.y + 18, 20, WHITE);
        draw_multiline(boss_name_from_index(item_index), (int)box.x + 14, (int)box.y + 66, 20, rgb(255, 231, 160));
    } else if (tab == COLLECTION_TAROTS) {
        draw_tarot_visual((TarotType)item_index, box, 0);
    }

    if (hover(box)) {
        set_tooltip(app, "%s\n%s", collection_item_name(tab, item_index), collection_item_desc(tab, item_index));
    }
}

static void draw_background_overlay(void) {
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.35f));
}

static void draw_sidebar(const AppState *app) {
    int ante = ((app->phase_number - 1) / 3) + 1;
    int blind = blind_kind_from_phase(app->phase_number) + 1;
    HandType shown_type = app->preview.valid ? app->preview.eval.type : HAND_HIGH_CARD;
    int shown_level = app->build.hand_levels[shown_type];
    char preview_total[96];
    Rectangle chips_box = scale_rect_center(Rf(78, 426, 78, 54), pulse_scale(&app->counter_pulses[0], 0.08f));
    Rectangle mult_box = scale_rect_center(Rf(168, 426, 78, 54), pulse_scale(&app->counter_pulses[1], 0.08f));
    Rectangle xmult_box = scale_rect_center(Rf(258, 426, 82, 54), pulse_scale(&app->counter_pulses[2], 0.08f));

    draw_panel(Rf(38, 42, 348, 816), Fade(BLACK, 0.97f), WHITE);
    draw_panel(Rf(60, 68, 300, 170), rgb(96, 48, 255), WHITE);
    DrawText(blind_name_from_phase(app->phase_number), 82, 88, 40, WHITE);
    DrawText("Pontue pelo menos", 82, 152, 24, WHITE);
    DrawText(TextFormat("%d", app->phase.goal), 118, 186, 54, rgb(239, 245, 64));
    DrawText(TextFormat("Recompensa: $%d", blind_reward_from_phase(app->phase_number)), 84, 246, 22, WHITE);

    draw_panel(Rf(60, 258, 300, 74), Fade(BLACK, 0.92f), WHITE);
    DrawText("Score", 84, 276, 28, rgb(96, 48, 255));
    DrawText(TextFormat("%d", app->phase.score), 218, 270, 38, WHITE);

    draw_panel(Rf(60, 348, 300, 152), Fade(BLACK, 0.92f), WHITE);
    DrawText(app->preview.valid ? hand_type_name(shown_type) : "Selecione cartas", 82, 372, app->preview.valid ? 40 : 30, WHITE);
    DrawText(app->preview.valid ? TextFormat("%d carta(s) | nv.%d", app->preview.selected_count, shown_level) : "Monte uma mao de poker", 84, 410, 18, Fade(WHITE, 0.78f));
    draw_panel(chips_box, rgb(255, 63, 80), WHITE);
    draw_panel(mult_box, rgb(74, 152, 243), WHITE);
    draw_panel(xmult_box, rgb(96, 48, 255), WHITE);
    DrawText("CHIPS", 88, 432, 12, WHITE);
    DrawText("MULT", 178, 432, 12, WHITE);
    DrawText("XMULT", 264, 432, 12, WHITE);
    DrawText(TextFormat("%d", app->preview.valid ? (int)(app->preview.chips + 0.5) : 0), 96, 450, 22, WHITE);
    DrawText(TextFormat("x%d", app->preview.valid ? (int)(app->preview.mult + 0.5) : 0), 182, 450, 22, WHITE);
    DrawText(TextFormat("x%.1f", app->preview.valid ? app->preview.xmult : 1.0f), 264, 450, 22, WHITE);
    if (app->preview.valid) {
        if (app->preview.coins > 0) snprintf(preview_total, sizeof(preview_total), "Total: %d pts | +$%d", app->preview.points, app->preview.coins);
        else snprintf(preview_total, sizeof(preview_total), "Total: %d pts", app->preview.points);
        DrawText(preview_total, 84, 484, 16, rgb(239, 245, 64));
    } else {
        DrawText("A soma das cartas e dos efeitos aparece aqui.", 84, 484, 16, rgb(239, 245, 64));
    }

    draw_panel(Rf(60, 518, 142, 92), rgb(255, 63, 80), WHITE);
    DrawText("Descartes", 84, 530, 22, WHITE);
    DrawText(TextFormat("%d", app->phase.discards_left), 122, 564, 38, WHITE);
    draw_panel(Rf(218, 518, 142, 92), rgb(74, 152, 243), WHITE);
    DrawText("Maos", 258, 530, 22, WHITE);
    DrawText(TextFormat("%d", app->phase.hands_left), 274, 564, 38, WHITE);

    draw_panel(scale_rect_center(Rf(60, 626, 300, 70), pulse_scale(&app->counter_pulses[3], 0.06f)), Fade(BLACK, 0.92f), WHITE);
    DrawText(TextFormat("$%d", app->build.coins), 144, 640, 48, rgb(239, 245, 64));

    draw_panel(Rf(60, 718, 94, 108), rgb(239, 245, 64), WHITE);
    DrawText("Info", 82, 744, 24, BLACK);
    DrawText("run", 88, 778, 18, BLACK);
    draw_panel(Rf(164, 718, 94, 108), Fade(BLACK, 0.92f), WHITE);
    DrawText("Aposta", 174, 738, 17, WHITE);
    DrawText(TextFormat("%d/8", ante), 184, 766, 28, rgb(239, 245, 64));
    DrawText(TextFormat("R%d", blind), 198, 796, 18, WHITE);
    draw_panel(Rf(266, 718, 94, 108), rgb(214, 82, 83), WHITE);
    DrawText("Menu", 284, 746, 22, WHITE);
    DrawText("principal", 276, 780, 14, WHITE);
}

static void draw_status_bar(const AppState *app) {
    draw_panel(Rf(432, 828, 1128, 44), Fade(rgb(28, 36, 44), 0.90f), rgb(74, 84, 94));
    draw_wrapped_text(app->status, Rf(452, 838, 1088, 24), 20, 4, WHITE, 1);
}

static void draw_tooltip_box(const AppState *app) {
    if (app->tooltip[0] == '\0') return;
    Vector2 mouse = virtual_mouse();
    int lines = draw_wrapped_text(app->tooltip, Rf(0, 0, 328, 360), 18, 4, rgb(38, 42, 50), 0);
    {
        Rectangle box = Rf(mouse.x + 16, mouse.y + 12, 356, 30 + lines * 22);
        if (box.x + box.width > SCREEN_W - 20) box.x = mouse.x - box.width - 16;
        if (box.y + box.height > SCREEN_H - 20) box.y = SCREEN_H - box.height - 20;
        draw_panel(box, Fade(WHITE, 0.985f), rgb(88, 96, 106));
        draw_wrapped_text(app->tooltip, Rf(box.x + 14, box.y + 10, box.width - 28, box.height - 18), 18, 4, rgb(38, 42, 50), 1);
    }
}

static void draw_joker_detail_panel(AppState *app, Rectangle panel) {
    if (app->selected_joker_index < 0 || app->selected_joker_index >= app->build.joker_count) return;
    {
        JokerInstance *joker = &app->build.jokers[app->selected_joker_index];
        draw_panel(panel, Fade(rgb(28, 34, 42), 0.96f), rgb(80, 88, 96));
        DrawText(joker_name(joker->type), (int)panel.x + 20, (int)panel.y + 18, 28, WHITE);
        draw_multiline(joker_desc(joker->type), (int)panel.x + 20, (int)panel.y + 62, 18, WHITE);
        DrawText(TextFormat("Edicao: %s", gui_edition_name(joker->edition)), (int)panel.x + 20, (int)panel.y + 138, 18, rgb(214, 219, 226));
        DrawText(TextFormat("Raridade: %s", joker_rarity_name(joker->type)), (int)panel.x + 20, (int)panel.y + 164, 18, rgb(214, 219, 226));
        draw_button(Rf(panel.x + 20, panel.y + panel.height - 62, panel.width - 40, 42), TextFormat("Vender por $%d", joker->sell_value), rgb(214, 84, 94), 22);
        if (clicked(Rf(panel.x + 20, panel.y + panel.height - 62, panel.width - 40, 42))) sell_selected_joker(app);
    }
}

static const char *hand_type_desc(HandType type) {
    switch (type) {
        case HAND_HIGH_CARD: return "Nenhuma combinacao forte. Vale a maior carta selecionada.";
        case HAND_PAIR: return "Duas cartas com o mesmo valor, como dois 7.";
        case HAND_TWO_PAIR: return "Dois pares diferentes na mesma jogada.";
        case HAND_THREE: return "Tres cartas com o mesmo valor.";
        case HAND_STRAIGHT: return "Cinco cartas em sequencia, mesmo com naipes diferentes.";
        case HAND_FLUSH: return "Cinco cartas com o mesmo naipe.";
        case HAND_FULL_HOUSE: return "Uma trinca junto com um par.";
        case HAND_FOUR: return "Quatro cartas com o mesmo valor.";
        case HAND_STRAIGHT_FLUSH: return "Cinco cartas em sequencia e todas do mesmo naipe.";
        default: return "Mao de poker.";
    }
}

static void open_run_info(AppState *app, ScreenId return_screen) {
    if (!app) return;
    app->run_info_return_screen = return_screen;
    app->screen = SCREEN_RUN_INFO;
}

static void draw_run_info_tabs(AppState *app) {
    const char *tabs[] = {"Maos de Poker", "Blinds", "Quiz"};
    for (int i = 0; i < 3; i++) {
        Rectangle tab = Rf(210 + i * 250, 94, 220, 58);
        draw_button(tab, tabs[i], app->run_info_tab == i ? rgb(255, 63, 80) : Fade(BLACK, 0.90f), 24);
        if (clicked(tab)) app->run_info_tab = i;
    }
}

static void draw_run_info_hands(AppState *app) {
    const HandType order[] = {
        HAND_STRAIGHT_FLUSH, HAND_FOUR, HAND_FULL_HOUSE, HAND_FLUSH, HAND_STRAIGHT,
        HAND_THREE, HAND_TWO_PAIR, HAND_PAIR, HAND_HIGH_CARD
    };
    for (int i = 0; i < 9; i++) {
        HandType type = order[i];
        Rectangle row = Rf(222, 190 + i * 58, 1050, 46);
        int level = app->build.hand_levels[type];
        int played = app->stats.hand_play_counts[type];
        int chips = HAND_GROWTH[type].chips * level;
        int mult = HAND_GROWTH[type].mult * level;
        draw_panel(row, i % 2 == 0 ? rgb(218, 222, 229) : rgb(198, 203, 212), rgb(70, 78, 88));
        DrawText(TextFormat("nv.%d", level), 246, 202 + i * 58, 22, rgb(28, 38, 48));
        DrawText(hand_type_name(type), 398, 202 + i * 58, 25, rgb(28, 38, 48));
        DrawText(TextFormat("%d x %d", chips, mult), 786, 202 + i * 58, 24, rgb(22, 82, 180));
        DrawText(TextFormat("# %d", played), 1032, 202 + i * 58, 24, rgb(230, 112, 42));
        if (hover(row)) set_tooltip(app, "%s\n%s\nNivel aumenta chips e multiplicador, sem alterar XMulti.", hand_type_name(type), hand_type_desc(type));
    }
}

static void draw_run_info_blinds(AppState *app) {
    int current = blind_kind_from_phase(app->phase_number);
    int ante = ((app->phase_number - 1) / 3) + 1;
    for (int i = 0; i < 3; i++) {
        int phase = (ante - 1) * 3 + i + 1;
        int base = ante_base_goal(ante);
        int goal = i == 0 ? base : i == 1 ? (base * 3) / 2 : base * 2;
        Rectangle card = Rf(210 + i * 390, 210, 330, 430);
        draw_panel(card, Fade(rgb(32, 40, 48), 0.98f), i == current ? rgb(255, 63, 80) : rgb(84, 94, 104));
        draw_button(Rf(card.x + 62, card.y + 30, 206, 52), i == current ? "Atual" : "Proximo", i == current ? rgb(255, 63, 80) : rgb(239, 166, 42), 28);
        draw_panel(Rf(card.x + 44, card.y + 110, 242, 58), i == 0 ? rgb(18, 110, 172) : i == 1 ? rgb(158, 118, 22) : rgb(130, 42, 24), rgb(60, 72, 84));
        DrawText(blind_name_from_phase(phase), (int)card.x + 84, (int)card.y + 124, 26, WHITE);
        DrawCircle((int)card.x + 166, (int)card.y + 230, 52, i == 0 ? rgb(60, 95, 210) : i == 1 ? rgb(236, 198, 76) : rgb(192, 64, 48));
        DrawText(i == 2 ? "BOSS" : i == 1 ? "BIG" : "SMALL", (int)card.x + 120, (int)card.y + 212, 22, WHITE);
        DrawText("Pontue pelo menos", (int)card.x + 62, (int)card.y + 314, 22, WHITE);
        DrawText(TextFormat("%d", goal), (int)card.x + 112, (int)card.y + 344, 42, rgb(255, 63, 80));
        DrawText(TextFormat("Recompensa: $%d", blind_reward_from_phase(phase)), (int)card.x + 74, (int)card.y + 398, 22, rgb(239, 245, 64));
    }
}

static void draw_run_info_quiz(AppState *app) {
    DrawText(TextFormat("Perguntas respondidas nesta tentativa: %d", app->quiz_log_count), 220, 178, 26, rgb(239, 245, 64));
    for (int i = 0; i < 10; i++) {
        int idx = app->quiz_log_count - 1 - i;
        Rectangle row = Rf(220, 232 + i * 52, 1088, 42);
        draw_panel(row, i % 2 == 0 ? Fade(BLACK, 0.80f) : Fade(rgb(42, 50, 60), 0.92f), rgb(76, 86, 98));
        if (idx >= 0) {
            QuizLogEntry *entry = &app->quiz_log[idx];
            DrawText(entry->correct ? "V" : "X", 238, 242 + i * 52, 24, entry->correct ? rgb(86, 220, 158) : rgb(239, 82, 82));
            DrawText(entry->title, 282, 242 + i * 52, 22, WHITE);
            DrawText(entry->category, 570, 242 + i * 52, 18, rgb(239, 245, 64));
            DrawText(TextFormat("%s | tentativas: %d", entry->difficulty, entry->attempts), 820, 242 + i * 52, 18, WHITE);
            if (hover(row)) set_tooltip(app, "%s\n%s\nDica: %s", entry->question, entry->correct ? "Acertada" : "Errada ou em aberto", entry->hint);
        } else {
            DrawText("--", 238, 242 + i * 52, 22, Fade(WHITE, 0.45f));
        }
    }
}

static void draw_run_info_screen(AppState *app) {
    draw_background();
    draw_background_overlay();
    draw_panel(Rf(142, 52, 1316, 770), Fade(rgb(24, 30, 38), 0.98f), WHITE);
    draw_run_info_tabs(app);
    if (app->run_info_tab == 0) draw_run_info_hands(app);
    else if (app->run_info_tab == 1) draw_run_info_blinds(app);
    else draw_run_info_quiz(app);
    draw_button(Rf(238, 728, 1084, 56), "Voltar", rgb(239, 166, 42), 28);
    if (clicked(Rf(238, 728, 1084, 56))) app->screen = app->run_info_return_screen;
    draw_status_bar(app);
}

static void draw_remaining_deck_widget(AppState *app) {
    Rectangle deck = Rf(1430, 600, 92, 128);
    int remaining = DECK_SIZE - app->phase_deck.top;
    draw_panel(deck, rgb(239, 166, 42), WHITE);
    draw_panel(Rf(deck.x + 10, deck.y + 12, deck.width - 20, deck.height - 24), rgb(236, 118, 46), WHITE);
    DrawText("Ver", (int)deck.x + 30, (int)deck.y + 44, 24, WHITE);
    DrawText("Baralho", (int)deck.x + 12, (int)deck.y + 72, 20, WHITE);
    DrawText(TextFormat("%d/52", remaining), (int)deck.x + 16, (int)deck.y + 138, 22, rgb(239, 245, 64));

    if (hover(deck)) {
        int counts[4][14];
        memset(counts, 0, sizeof(counts));
        for (int i = app->phase_deck.top; i < DECK_SIZE; i++) {
            Card card = app->phase_deck.cards[i];
            if (card.suit >= 0 && card.suit < 4 && card.rank >= 1 && card.rank <= 13) counts[card.suit][card.rank]++;
        }
        draw_panel(Rf(502, 148, 720, 260), Fade(rgb(26, 34, 42), 0.98f), WHITE);
        DrawText("Cartas que ainda podem vir", 548, 176, 28, rgb(239, 245, 64));
        for (int rank = 1; rank <= 13; rank++) {
            DrawText(rank_name(rank), 566 + (rank - 1) * 48, 224, 20, WHITE);
        }
        for (int suit = 0; suit < 4; suit++) {
            Color color = suit_color(suit);
            DrawText(suit_name(suit), 528, 262 + suit * 36, 18, color);
            for (int rank = 1; rank <= 13; rank++) {
                DrawText(TextFormat("%d", counts[suit][rank]), 574 + (rank - 1) * 48, 262 + suit * 36, 22, counts[suit][rank] ? WHITE : Fade(WHITE, 0.26f));
            }
        }
    }
}

static void draw_villain_card(AppState *app, Rectangle box, const char *speech, int angry) {
    (void)app;
    draw_panel(box, angry ? rgb(255, 238, 238) : WHITE, angry ? rgb(239, 82, 82) : rgb(72, 84, 98));
    if (g_assets && texture_ready(g_assets->villain)) {
        draw_texture_fit(g_assets->villain, Rf(box.x + 10, box.y + 10, box.width - 20, box.height - 52), WHITE);
    } else {
        DrawText("JOKER", (int)box.x + 24, (int)box.y + 14, 20, rgb(34, 38, 44));
        DrawCircle((int)(box.x + box.width * 0.5f), (int)(box.y + 70), 34, angry ? rgb(239, 82, 82) : rgb(86, 220, 158));
        DrawText(angry ? ">:)" : ":)", (int)box.x + 44, (int)box.y + 52, 28, WHITE);
        DrawText("HELP", (int)box.x + 36, (int)box.y + 110, 20, rgb(34, 38, 44));
    }
    if (speech && speech[0]) {
        Rectangle bubble = Rf(box.x - 80, box.y + box.height - 20, box.width + 120, 74);
        draw_panel(bubble, Fade(WHITE, 0.96f), angry ? rgb(239, 82, 82) : rgb(76, 88, 100));
        draw_multiline(speech, (int)bubble.x + 12, (int)bubble.y + 10, 18, rgb(38, 46, 52));
    }
}

static void draw_profile_screen(AppState *app) {
    Rectangle name_box = Rf(208, 224, 540, 58);
    Rectangle logic_box = Rf(208, 338, 286, 118);
    Rectangle math_box = Rf(530, 338, 286, 118);
    Rectangle prog_box = Rf(852, 338, 286, 118);
    Rectangle port_box = Rf(1174, 338, 286, 118);
    char category_summary[160];
    Color active_color = rgb(96, 48, 255);
    Color inactive_color = Fade(BLACK, 0.92f);

    draw_background();
    draw_panel(Rf(122, 112, 1356, 676), Fade(rgb(24, 30, 38), 0.96f), WHITE);
    DrawText("PERFIL DA RUN", 174, 154, 58, WHITE);
    DrawText("Escolha o nome do jogador e o conjunto de perguntas que define a modalidade da run.", 178, 210, 26, Fade(WHITE, 0.84f));

    if (clicked(name_box)) app->profile_focus = 1;
    handle_text_box_input(app->draft_player, sizeof(app->draft_player), app->profile_focus == 1);

    draw_panel(name_box, Fade(BLACK, 0.88f), app->profile_focus == 1 ? rgb(239, 245, 64) : rgb(76, 84, 96));
    DrawText("Nome do jogador", 208, 192, 20, rgb(239, 245, 64));
    DrawText(app->draft_player[0] ? app->draft_player : "Digite aqui...", 230, 242, 28, WHITE);

    draw_panel(logic_box, (app->draft_question_mask & PLAYER_MASK_LOGIC) ? active_color : inactive_color, WHITE);
    draw_panel(math_box, (app->draft_question_mask & PLAYER_MASK_MAT) ? active_color : inactive_color, WHITE);
    draw_panel(prog_box, (app->draft_question_mask & PLAYER_MASK_PROG) ? active_color : inactive_color, WHITE);
    draw_panel(port_box, (app->draft_question_mask & PLAYER_MASK_PORT) ? active_color : inactive_color, WHITE);
    DrawText("Logica Classica", 238, 366, 28, WHITE);
    DrawText("Silogismos, negacoes\ne equivalencias.", 238, 404, 22, WHITE);
    DrawText("Logica Matematica", 562, 366, 28, WHITE);
    DrawText("Equacoes, funcoes,\nprobabilidade e calculo.", 562, 404, 22, WHITE);
    DrawText("Prog. em C", 936, 366, 28, WHITE);
    DrawText("Ponteiros, arquivos,\nmemoria e sintaxe.", 936, 404, 22, WHITE);
    DrawText("Portugues", 1258, 366, 28, WHITE);
    DrawText("Sinonimos, sentidos\ne vocabulario.", 1258, 404, 22, WHITE);

    if (clicked(logic_box)) app->draft_question_mask ^= PLAYER_MASK_LOGIC;
    if (clicked(math_box)) app->draft_question_mask ^= PLAYER_MASK_MAT;
    if (clicked(prog_box)) app->draft_question_mask ^= PLAYER_MASK_PROG;
    if (clicked(port_box)) app->draft_question_mask ^= PLAYER_MASK_PORT;

    player_mask_summary(app->draft_question_mask, category_summary, sizeof(category_summary));
    DrawText("Categorias da run", 176, 512, 24, rgb(239, 245, 64));
    draw_multiline(category_summary, 176, 544, 28, WHITE);
    DrawText(TextFormat("Modalidade detectada: %s", player_modality_name_from_mask(app->draft_question_mask)), 176, 640, 30, rgb(86, 220, 158));

    draw_button(Rf(176, 712, 272, 58), "Salvar e Jogar", rgb(96, 48, 255), 28);
    draw_button(Rf(474, 712, 192, 58), "Salvar", rgb(239, 245, 64), 28);
    draw_button(Rf(692, 712, 192, 58), "Voltar", Fade(BLACK, 0.9f), 28);
    draw_button(Rf(910, 712, 236, 58), "Desvincular", rgb(214, 82, 83), 28);

    if (clicked(Rf(176, 712, 272, 58))) save_profile_from_draft(app, 1);
    if (clicked(Rf(474, 712, 192, 58))) save_profile_from_draft(app, 0);
    if (clicked(Rf(692, 712, 192, 58))) {
        app->auto_start_after_profile = 0;
        app->screen = SCREEN_TITLE;
    }
    if (clicked(Rf(910, 712, 236, 58))) {
        unlink_active_player(app);
        app->screen = SCREEN_TITLE;
    }
    draw_status_bar(app);
}

static void draw_leaderboard_screen(AppState *app) {
    RunRecord records[PLAYER_MAX_RECORDS];
    RunRecord top[PLAYER_TOP_LIMIT];
    int record_count = player_load_run_records(PLAYER_HISTORY_FILE, records, PLAYER_MAX_RECORDS);
    int total_in_mode = player_build_leaderboard(records, record_count, (LeaderboardMode)app->leaderboard_mode, top, PLAYER_TOP_LIMIT, NULL, NULL);
    const LeaderboardMode mode = (LeaderboardMode)app->leaderboard_mode;
    const char *labels[LEADERBOARD_COUNT] = {
        "Mat", "Port", "Logic", "Prog", "Esc", "Conc", "Eng", "Intel", "Misto"
    };

    draw_background();
    draw_panel(Rf(122, 98, 1356, 700), Fade(rgb(24, 30, 38), 0.96f), WHITE);
    DrawText("TOP 10 POR MODALIDADE", 168, 144, 54, WHITE);
    DrawText("Cada ranking usa as runs salvas no arquivo da GUI do raylib.", 172, 198, 24, Fade(WHITE, 0.82f));

    for (int i = 0; i < LEADERBOARD_COUNT; i++) {
        Rectangle box = Rf(172 + (i % 5) * 244, 252 + (i / 5) * 66, 214, 50);
        draw_button(box, labels[i], i == app->leaderboard_mode ? rgb(96, 48, 255) : Fade(BLACK, 0.9f), 24);
        if (clicked(box)) app->leaderboard_mode = i;
    }

    DrawText(leaderboard_mode_name(mode), 178, 402, 38, rgb(239, 245, 64));
    DrawText(TextFormat("Runs registradas nesta modalidade: %d", total_in_mode), 178, 448, 22, WHITE);

    for (int i = 0; i < PLAYER_TOP_LIMIT; i++) {
        Rectangle row = Rf(176, 494 + i * 46, 1240, 38);
        draw_panel(row, i % 2 == 0 ? Fade(BLACK, 0.84f) : Fade(rgb(38, 44, 54), 0.92f), rgb(64, 72, 82));
        if (i < total_in_mode) {
            Color text_color = player_casecmp(top[i].player, app->active_player) == 0 ? rgb(86, 220, 158) : WHITE;
            DrawText(TextFormat("%d", i + 1), 194, 502 + i * 46, 22, rgb(239, 245, 64));
            DrawText(top[i].player, 248, 502 + i * 46, 22, text_color);
            DrawText(top[i].modality, 544, 502 + i * 46, 22, WHITE);
            DrawText(TextFormat("%d pts", top[i].total_points), 782, 502 + i * 46, 22, WHITE);
            DrawText(TextFormat("Aposta %d", top[i].bet_reached), 956, 502 + i * 46, 22, WHITE);
            DrawText(top[i].run_won ? "Venceu" : "Run encerrada", 1106, 502 + i * 46, 22, top[i].run_won ? rgb(86, 220, 158) : rgb(239, 82, 82));
        } else {
            DrawText("--", 194, 502 + i * 46, 22, Fade(WHITE, 0.55f));
        }
    }

    draw_button(Rf(176, 742, 196, 48), "Voltar", Fade(BLACK, 0.9f), 24);
    if (clicked(Rf(176, 742, 196, 48))) app->screen = SCREEN_TITLE;
    draw_status_bar(app);
}

static void draw_history_screen(AppState *app) {
    RunRecord records[PLAYER_MAX_RECORDS];
    RunRecord history[PLAYER_MAX_RECORDS];
    Rectangle input_box = Rf(180, 220, 500, 56);
    int record_count;
    int history_count;
    int wins = 0;
    int best_points = 0;
    int points[PLAYER_MAX_RECORDS];
    int total_points_history = 0;
    double avg_points = 0.0;
    double std_points = 0.0;

    if (!app->history_query[0] && player_is_linked(app)) snprintf(app->history_query, sizeof(app->history_query), "%s", app->active_player);

    if (clicked(input_box)) app->history_focus = 1;
    handle_text_box_input(app->history_query, sizeof(app->history_query), app->history_focus == 1);

    record_count = player_load_run_records(PLAYER_HISTORY_FILE, records, PLAYER_MAX_RECORDS);
    history_count = player_filter_history(records, record_count, app->history_query, history, PLAYER_MAX_RECORDS);
    for (int i = 0; i < history_count; i++) {
        if (history[i].run_won) wins++;
        if (history[i].total_points > best_points) best_points = history[i].total_points;
        points[i] = history[i].total_points;
    }
    if (history_count > 0) {
        total_points_history = rec_sum_int(points, history_count);
        avg_points = (double)total_points_history / (double)history_count;
        std_points = sqrt(rec_sum_sq_diff_int(points, history_count, avg_points) / (double)history_count);
    }

    draw_background();
    draw_panel(Rf(122, 98, 1356, 700), Fade(rgb(24, 30, 38), 0.96f), WHITE);
    DrawText("HISTORICO POR JOGADOR", 168, 144, 54, WHITE);
    DrawText("Veja as sessoes gravadas de um nome especifico e compare suas runs.", 172, 198, 24, Fade(WHITE, 0.82f));

    draw_panel(input_box, Fade(BLACK, 0.88f), app->history_focus == 1 ? rgb(239, 245, 64) : rgb(76, 84, 96));
    DrawText("Nome para consulta", 180, 192, 20, rgb(239, 245, 64));
    DrawText(app->history_query[0] ? app->history_query : "Digite um nome...", 204, 236, 28, WHITE);

    draw_button(Rf(714, 220, 226, 56), "Usar jogador atual", rgb(96, 48, 255), 22);
    draw_button(Rf(964, 220, 176, 56), "Limpar", Fade(BLACK, 0.9f), 22);
    draw_button(Rf(1164, 220, 196, 56), "Voltar", Fade(BLACK, 0.9f), 22);
    if (clicked(Rf(714, 220, 226, 56)) && player_is_linked(app)) snprintf(app->history_query, sizeof(app->history_query), "%s", app->active_player);
    if (clicked(Rf(964, 220, 176, 56))) app->history_query[0] = '\0';
    if (clicked(Rf(1164, 220, 196, 56))) app->screen = SCREEN_TITLE;

    DrawText(TextFormat("Runs encontradas: %d", history_count), 180, 318, 26, WHITE);
    DrawText(TextFormat("Vitorias: %d", wins), 452, 318, 26, rgb(86, 220, 158));
    DrawText(TextFormat("Melhor pontuacao: %d", best_points), 642, 318, 26, rgb(239, 245, 64));
    DrawText(TextFormat("Media: %.0f | Min/Max: %d/%d | Desvio: %.1f", avg_points, history_count ? rec_min_int(points, history_count) : 0, history_count ? rec_max_int(points, history_count) : 0, std_points), 180, 346, 20, Fade(WHITE, 0.82f));

    for (int i = 0; i < 8; i++) {
        Rectangle row = Rf(176, 372 + i * 48, 1244, 40);
        draw_panel(row, i % 2 == 0 ? Fade(BLACK, 0.84f) : Fade(rgb(38, 44, 54), 0.92f), rgb(64, 72, 82));
        if (i < history_count) {
            DrawText(history[i].timestamp, 194, 382 + i * 48, 20, WHITE);
            DrawText(history[i].modality, 396, 382 + i * 48, 20, rgb(239, 245, 64));
            DrawText(TextFormat("%d pts", history[i].total_points), 634, 382 + i * 48, 20, WHITE);
            DrawText(TextFormat("Aposta %d", history[i].bet_reached), 816, 382 + i * 48, 20, WHITE);
            DrawText(history[i].run_won ? "Venceu" : history[i].defeated_by, 1014, 382 + i * 48, 20, history[i].run_won ? rgb(86, 220, 158) : rgb(239, 82, 82));
        } else {
            DrawText("--", 194, 382 + i * 48, 20, Fade(WHITE, 0.55f));
        }
    }
    draw_status_bar(app);
}

static void draw_collection_screen(AppState *app) {
    const int items_per_page = 10;
    CollectionTab tab = (CollectionTab)app->collection_tab;
    int total = collection_tab_total(tab);
    int unlocked = collection_unlocked_count(app, tab);
    int total_pages = max_int(1, (total + items_per_page - 1) / items_per_page);
    int start_index;
    const char *labels[COLLECTION_TAB_COUNT] = {"Coringas", "Cupons", "Marcas", "Chefes", "Tarots"};

    if (app->collection_page >= total_pages) app->collection_page = total_pages - 1;
    if (app->collection_page < 0) app->collection_page = 0;
    start_index = app->collection_page * items_per_page + 1;

    draw_background();
    draw_panel(Rf(122, 92, 1356, 710), Fade(rgb(24, 30, 38), 0.96f), WHITE);
    DrawText("COLECAO", 168, 138, 58, WHITE);

    if (!player_is_linked(app)) {
        DrawText("Vincule um jogador para ver a colecao desbloqueada ou use o Admin para a apresentacao.", 172, 220, 26, Fade(WHITE, 0.82f));
        draw_button(Rf(172, 310, 220, 58), "PERFIL", rgb(96, 48, 255), 26);
        draw_button(Rf(418, 310, 220, 58), "ADMIN DEMO", rgb(239, 245, 64), 24);
        draw_button(Rf(664, 310, 220, 58), "VOLTAR", Fade(BLACK, 0.9f), 24);
        if (clicked(Rf(172, 310, 220, 58))) {
            prime_profile_from_active(app);
            app->profile_focus = 1;
            app->screen = SCREEN_PROFILE;
        }
        if (clicked(Rf(418, 310, 220, 58))) activate_admin_profile(app);
        if (clicked(Rf(664, 310, 220, 58))) app->screen = SCREEN_TITLE;
        draw_status_bar(app);
        return;
    }

    DrawText(TextFormat("Jogador: %s", app->active_player), 174, 206, 28, rgb(239, 245, 64));
    DrawText(TextFormat("%s %d/%d", collection_tab_name(tab), unlocked, total), 174, 246, 28, WHITE);

    for (int i = 0; i < COLLECTION_TAB_COUNT; i++) {
        CollectionTab current = (CollectionTab)i;
        Rectangle box = Rf(170 + i * 248, 296, 220, 54);
        draw_button(box,
                    TextFormat("%s %d/%d", labels[i], collection_unlocked_count(app, current), collection_tab_total(current)),
                    i == app->collection_tab ? rgb(96, 48, 255) : Fade(BLACK, 0.9f),
                    20);
        if (clicked(box)) {
            app->collection_tab = i;
            app->collection_page = 0;
            tab = (CollectionTab)app->collection_tab;
            total = collection_tab_total(tab);
            unlocked = collection_unlocked_count(app, tab);
            total_pages = max_int(1, (total + items_per_page - 1) / items_per_page);
            start_index = 1;
        }
    }

    for (int i = 0; i < items_per_page; i++) {
        int item_index = start_index + i;
        int row = i / 5;
        int col = i % 5;
        Rectangle box = Rf(176.0f + col * 246.0f, 390.0f + row * 228.0f, 150.0f, 210.0f);
        if (item_index > total) break;
        draw_collection_item_card(app, tab, item_index, box);
    }

    DrawText(TextFormat("Pagina %d/%d", app->collection_page + 1, total_pages), 658, 724, 28, WHITE);
    draw_button(Rf(430, 714, 180, 54), "Anterior", Fade(BLACK, 0.9f), 24);
    draw_button(Rf(820, 714, 180, 54), "Proxima", Fade(BLACK, 0.9f), 24);
    draw_button(Rf(1210, 714, 180, 54), "Voltar", Fade(BLACK, 0.9f), 24);

    if (clicked(Rf(430, 714, 180, 54)) && app->collection_page > 0) app->collection_page--;
    if (clicked(Rf(820, 714, 180, 54)) && app->collection_page + 1 < total_pages) app->collection_page++;
    if (clicked(Rf(1210, 714, 180, 54))) app->screen = SCREEN_TITLE;
    draw_status_bar(app);
}

static void draw_title_screen(AppState *app) {
    char category_summary[160];

    if (app->auto_start_after_profile) {
        app->auto_start_after_profile = 0;
        start_run(app);
        return;
    }

    draw_background();
    DrawText("MGTA", 208, 138, 156, WHITE);
    DrawText("DECK", 208, 270, 148, rgb(239, 245, 64));
    DrawText("Poker, quiz e coringas", 356, 454, 34, Fade(WHITE, 0.86f));

    draw_panel(Rf(884, 142, 492, 252), Fade(rgb(24, 30, 38), 0.94f), WHITE);
    DrawText("Jogador vinculado", 920, 182, 28, rgb(239, 245, 64));
    DrawText(player_is_linked(app) ? app->active_player : "Nenhum jogador ativo", 920, 226, 34, WHITE);
    DrawText(TextFormat("Modalidade: %s", player_is_linked(app) ? player_modality_name_from_mask(app->question_mask) : "Aguardando perfil"), 920, 280, 24, WHITE);
    player_mask_summary(app->question_mask, category_summary, sizeof(category_summary));
    draw_multiline(player_is_linked(app) ? category_summary : "Abra o perfil para escolher as areas\nantes da run.", 920, 320, 22, WHITE);
    if (player_name_is_admin(app->active_player)) DrawText("Admin: tudo liberado para demonstracao.", 920, 374, 22, rgb(86, 220, 158));

    draw_button(Rf(208, 626, 246, 82), "JOGAR", rgb(96, 48, 255), 40);
    draw_button(Rf(484, 626, 246, 82), "PERFIL", rgb(239, 245, 64), 34);
    draw_button(Rf(760, 626, 246, 82), "RANKING", Fade(BLACK, 0.9f), 30);
    draw_button(Rf(1036, 626, 246, 82), "HISTORICO", Fade(BLACK, 0.9f), 30);
    draw_button(Rf(208, 724, 246, 64), "COLECAO", Fade(BLACK, 0.9f), 26);
    draw_button(Rf(484, 724, 246, 64), "ADMIN DEMO", rgb(239, 245, 64), 24);
    draw_button(Rf(760, 724, 246, 64), "DESVINCULAR", rgb(214, 82, 83), 24);
    draw_button(Rf(1036, 724, 246, 64), "SAIR", Fade(BLACK, 0.9f), 28);

    if (clicked(Rf(208, 626, 246, 82))) start_run(app);
    if (clicked(Rf(484, 626, 246, 82))) {
        prime_profile_from_active(app);
        app->profile_focus = 1;
        app->screen = SCREEN_PROFILE;
    }
    if (clicked(Rf(760, 626, 246, 82))) app->screen = SCREEN_LEADERBOARD;
    if (clicked(Rf(1036, 626, 246, 82))) {
        if (player_is_linked(app)) snprintf(app->history_query, sizeof(app->history_query), "%s", app->active_player);
        app->history_focus = 1;
        app->screen = SCREEN_HISTORY;
    }
    if (clicked(Rf(208, 724, 246, 64))) app->screen = SCREEN_COLLECTION;
    if (clicked(Rf(484, 724, 246, 64))) activate_admin_profile(app);
    if (clicked(Rf(760, 724, 246, 64))) unlink_active_player(app);
    if (clicked(Rf(1036, 724, 246, 64))) CloseWindow();
    draw_status_bar(app);
}

static void draw_round_preview_screen(AppState *app) {
    char reward_text[96];
    char category_summary[160];
    draw_background();
    draw_panel(Rf(94, 132, 1410, 634), Fade(BLACK, 0.94f), WHITE);
    DrawText("SELECIONE SEU DESAFIO", 138, 188, 58, WHITE);
    DrawText(TextFormat("Aposta %d/8 | %s", ((app->phase_number - 1) / 3) + 1, blind_name_from_phase(app->phase_number)), 142, 250, 28, rgb(239, 245, 64));
    player_mask_summary(app->question_mask, category_summary, sizeof(category_summary));
    DrawText(TextFormat("%s | %s", app->active_player, player_modality_name_from_mask(app->question_mask)), 804, 190, 28, WHITE);
    draw_multiline(category_summary, 804, 226, 18, Fade(WHITE, 0.84f));

    draw_panel(Rf(122, 312, 308, 330), Fade(BLACK, 0.9f), WHITE);
    DrawText(blind_name_from_phase(app->phase_number), 152, 342, 34, WHITE);
    DrawText("META", 154, 396, 22, rgb(96, 48, 255));
    DrawText(TextFormat("%d", app->phase.goal), 154, 426, 48, WHITE);
    DrawText(TextFormat("Recompensa da blind: $%d", blind_reward_from_phase(app->phase_number)), 154, 494, 22, rgb(239, 245, 64));
    DrawText(app->phase.boss ? "Boss nao concede marca.\nA pergunta bloqueia o debuff." : "Pergunta correta garante\numa oferta marcada.", 154, 540, 22, WHITE);

    draw_panel(Rf(470, 312, 308, 330), Fade(BLACK, 0.9f), WHITE);
    DrawText("MARCA DA RODADA", 502, 342, 32, WHITE);
    if (app->phase_mark_available) {
        draw_panel(Rf(548, 398, 148, 148), mark_color(app->phase_mark_reward), WHITE);
        DrawText(gui_mark_label(app->phase_mark_reward), 580, 454, 34, BLACK);
        snprintf(reward_text, sizeof(reward_text), "Na fila: %d oferta(s) marcada(s)", app->pending_mark_count);
        DrawText(reward_text, 522, 578, 22, WHITE);
    } else {
        DrawText("SEM MARCA", 548, 452, 34, rgb(255, 63, 80));
        DrawText("Chefe somente aplica\nou anula o efeito.", 520, 520, 22, WHITE);
    }

    draw_panel(Rf(818, 312, 592, 330), Fade(BLACK, 0.9f), WHITE);
    DrawText(app->phase.boss ? blind_boss_name_from_phase(app->phase_number) : "Atalho da rodada", 850, 342, 34, WHITE);
    draw_wrapped_text(app->phase.boss ? blind_boss_desc_from_phase(app->phase_number) : "Voce pode entrar na fase normalmente ou tentar uma pergunta dificil para pular esta blind e garantir a marca mostrada ao lado na proxima oferta de Coringa.",
                      Rf(850, 402, 520, 94), 24, 6, WHITE, 1);
    draw_button(Rf(848, 534, 244, 76), app->phase.boss ? "Entrar no chefe" : "Jogar fase", rgb(96, 48, 255), 32);
    draw_button(Rf(1114, 534, 244, 76), app->phase.boss ? "Sem atalho" : "Pular por quiz", app->phase.boss ? Fade(BLACK, 0.9f) : rgb(239, 245, 64), 28);
    draw_button(Rf(122, 668, 220, 58), "Voltar ao menu", Fade(BLACK, 0.9f), 24);

    if (clicked(Rf(848, 534, 244, 76))) enter_current_phase(app);
    if (clicked(Rf(1114, 534, 244, 76)) && !app->phase.boss) start_quiz(app, QUIZ_SKIP);
    if (clicked(Rf(122, 668, 220, 58))) app->screen = SCREEN_TITLE;
    draw_status_bar(app);
}

static void update_card_anims(AppState *app) {
    for (int i = 0; i < app->hand_count; i++) {
        float target_select = app->selected[i] ? 1.0f : 0.0f;
        float target_hover = app->hovered_card == i ? 1.0f : 0.0f;
        app->select_anim[i] += (target_select - app->select_anim[i]) * 0.18f;
        app->hover_anim[i] += (target_hover - app->hover_anim[i]) * 0.18f;
    }
}

static void handle_play_input(AppState *app) {
    int i;
    /* Hover scan forward (tooltip for last card under cursor) */
    app->hovered_card = -1;
    for (i = 0; i < app->hand_count; i++) {
        Rectangle box = card_rect(i, app->hand_count, app->select_anim[i], app->hover_anim[i]);
        if (hover(box)) {
            app->hovered_card = i;
            set_tooltip(app, "%s de %s\n%s", rank_name(app->hand[i].rank), suit_name(app->hand[i].suit), enhancement_desc(app->hand[i].enhancement));
        }
    }
    /* Click scan reverse: topmost (last-drawn) card wins, break prevents double-fire */
    for (i = app->hand_count - 1; i >= 0; i--) {
        Rectangle box = card_rect(i, app->hand_count, app->select_anim[i], app->hover_anim[i]);
        if (hover(box) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int sel = count_selected_cards(app);
            if (app->selected[i] || sel < MAX_PLAY) app->selected[i] = !app->selected[i];
            break;
        }
    }
    update_card_anims(app);

    /* Jokers: use hit-rect so overlapping sprites share clean hitboxes */
    for (i = 0; i < app->build.joker_count; i++) {
        Rectangle box = joker_hit_rect_for_count(i, app->build.joker_count, app->selected_joker_index);
        if (hover(box)) set_tooltip(app, "%s\n%s\nEdicao: %s\nRaridade: %s",
            joker_name(app->build.jokers[i].type),
            joker_desc(app->build.jokers[i].type),
            gui_edition_name(app->build.jokers[i].edition),
            joker_rarity_name(app->build.jokers[i].type));
        if (hover(box) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            app->selected_joker_index = i;
    }
    for (i = 0; i < app->build.tarot_count; i++) {
        Rectangle box = tarot_rect(i);
        if (hover(box)) set_tooltip(app, "%s\n%s", tarot_display_name(app->build.tarot_inventory[i]), tarot_desc(app->build.tarot_inventory[i]));
        if (clicked(box)) app->selected_tarot_index = i;
    }

    if (clicked(Rf(678, 748, 220, 62))) play_selected_cards(app);
    if (clicked(Rf(1358, 248, 142, 48))) apply_selected_tarot_on_hand(app);
    if (clicked(Rf(920, 748, 170, 62))) sort_hand(app, 0);
    if (clicked(Rf(920, 818, 170, 40))) sort_hand(app, 1);
    if (clicked(Rf(1110, 748, 220, 62))) discard_selected_cards(app);
    if (clicked(Rf(60, 718, 94, 108))) open_run_info(app, SCREEN_PLAY);
    /* Back to main menu — checked last so nothing above can shadow it */
    if (clicked(Rf(266, 718, 94, 108))) { app->screen = SCREEN_TITLE; return; }
}

static void draw_play_screen(AppState *app) {
    update_all_pulses(app);
    handle_play_input(app);
    rebuild_selection_preview(app);

    draw_background();
    draw_sidebar(app);
    draw_panel(Rf(428, 40, 1134, 778), Fade(BLACK, 0.84f), WHITE);

    DrawText(TextFormat("%d/%d", app->build.joker_count, app->build.joker_capacity), 436, 218, 24, Fade(WHITE, 0.82f));
    DrawText(TextFormat("%d/%d", app->build.tarot_count, app->build.tarot_capacity), 1460, 218, 24, Fade(WHITE, 0.82f));

    for (int i = 0; i < app->build.joker_count; i++) draw_inventory_joker(&app->build.jokers[i], scale_rect_center(joker_rect_for_count(i, app->build.joker_count, app->selected_joker_index), pulse_scale(&app->joker_pulses[i], 0.08f)), app->selected_joker_index == i);
    for (int i = 0; i < app->build.tarot_count; i++) draw_tarot_visual(app->build.tarot_inventory[i], tarot_rect(i), app->selected_tarot_index == i);
    for (int i = 0; i < app->hand_count; i++) {
        Rectangle box = card_rect(i, app->hand_count, app->select_anim[i], app->hover_anim[i]);
        draw_card_visual(app->hand[i], scale_rect_center(box, pulse_scale(&app->hand_pulses[i], 0.06f)), app->selected[i]);
    }

    draw_button(Rf(678, 748, 220, 62), "Jogar mao", rgb(96, 48, 255), 34);
    draw_button(Rf(920, 748, 170, 62), "Classe", Fade(BLACK, 0.9f), 26);
    draw_button(Rf(920, 818, 170, 40), "Naipe", rgb(239, 245, 64), 20);
    draw_button(Rf(1110, 748, 220, 62), "Descartar", rgb(255, 63, 80), 34);
    draw_joker_detail_panel(app, Rf(1310, 248, 228, 220));
    if (app->selected_tarot_index >= 0 && app->selected_tarot_index < app->build.tarot_count) {
        char reason[128];
        int usable = selected_tarot_is_usable(app, reason, sizeof(reason));
        draw_button(Rf(1358, 248, 142, 48), "Usar Tarot", usable ? rgb(239, 245, 64) : rgb(74, 78, 86), 20);
        if (!usable) DrawText(reason, 1320, 304, 16, Fade(WHITE, 0.76f));
    }
    draw_remaining_deck_widget(app);
    draw_status_bar(app);
}

static void handle_quiz_input(AppState *app) {
    for (int i = 0; i < 4; i++) {
        Rectangle box = Rf(360, 384 + i * 82, 880, 58);
        if (clicked(box)) answer_quiz(app, i + 1);
    }
}

static void draw_quiz_screen(AppState *app) {
    const char *speech = app->pending_failed_question ? "A mesma pergunta voltou.\nVai deixar ela mandar em voce?" : "Sem pressa...\nmas eu estou contando.";
    draw_background();
    draw_panel(Rf(280, 140, 1040, 620), Fade(BLACK, 0.94f), WHITE);
    DrawText(app->quiz_mode == QUIZ_BOSS ? "Pergunta do Chefe" : app->quiz_mode == QUIZ_SKIP ? "Pergunta Dificil" : "Pergunta da Fase", 360, 180, 48, WHITE);
    DrawText(ray_quiz_category_name(app->current_question.category), 364, 232, 24, rgb(239, 245, 64));
    DrawText(ray_quiz_difficulty_name(app->current_question.difficulty), 1020, 232, 24, rgb(86, 220, 158));
    draw_multiline(app->current_question.question, 360, 264, 30, WHITE);
    for (int i = 0; i < 4; i++) draw_button(Rf(360, 384 + i * 82, 880, 58), TextFormat("%d) %s", i + 1, app->current_question.options[i]), i % 2 == 0 ? rgb(96, 48, 255) : Fade(BLACK, 0.9f), 24);
    DrawText(app->quiz_mode == QUIZ_BOSS ? "Acertar neutraliza o chefe. Errar ativa o efeito do blind." : app->quiz_mode == QUIZ_SKIP ? "Acertar pula a blind atual e guarda a marca mostrada na previa." : "Acertar concede moedas, marca guardada e pode liberar voucher.", 360, 716, 20, Fade(WHITE, 0.80f));
    draw_villain_card(app, Rf(92, 274, 138, 188), speech, app->pending_failed_question);
    handle_quiz_input(app);
    draw_status_bar(app);
}

static void handle_reward_input(AppState *app) {
    if (app->reward.anim < 1.0f) app->reward.anim += GetFrameTime() * 1.5f;
    if (clicked(Rf(680, 278, 360, 72))) continue_after_reward(app);
}

static void draw_reward_screen(AppState *app) {
    int visible_lines;
    float reveal;
    draw_background();
    draw_sidebar(app);
    draw_panel(Rf(470, 250, 960, 430), Fade(rgb(28, 36, 44), 0.96f), rgb(236, 165, 34));
    DrawText(TextFormat("Pegar a Grana: $%d", app->reward.total), 662, 290, 54, WHITE);
    DrawText(app->reward.blind_name, 556, 372, 28, rgb(188, 204, 188));
    DrawText(TextFormat("Pontue pelo menos %d", app->phase.goal), 556, 412, 34, WHITE);
    DrawText(".................", 554, 468, 28, Fade(WHITE, 0.70f));

    reveal = app->reward.anim;
    visible_lines = reveal < 0.33f ? 1 : reveal < 0.66f ? 2 : 3;
    if (visible_lines >= 1) {
        DrawText(TextFormat("%d  Recompensa da fase", app->reward.base_reward), 556, 508, 28, rgb(84, 175, 255));
        DrawText(TextFormat("$%d", app->reward.base_reward), 1226, 508, 28, rgb(247, 188, 68));
    }
    if (visible_lines >= 2) {
        DrawText(TextFormat("%d  Maos restantes ($1 cada)", app->reward.hand_bonus), 556, 548, 28, WHITE);
        DrawText(TextFormat("$%d", app->reward.hand_bonus), 1226, 548, 28, rgb(247, 188, 68));
    }
    if (visible_lines >= 3) {
        DrawText(TextFormat("%d  Juros (1 por $5, max 5)", app->reward.interest), 556, 588, 28, WHITE);
        DrawText(TextFormat("$%d", app->reward.interest), 1226, 588, 28, rgb(247, 188, 68));
        if (app->reward.joker_bonus > 0) {
            DrawText(TextFormat("%d  Bonus de Coringa", app->reward.joker_bonus), 556, 628, 28, WHITE);
            DrawText(TextFormat("$%d", app->reward.joker_bonus), 1226, 628, 28, rgb(247, 188, 68));
        }
    }
    draw_button(Rf(680, 278, 360, 72), TextFormat("Pegar a Grana: $%d", app->reward.total), rgb(236, 165, 34), 44);
    handle_reward_input(app);
    draw_status_bar(app);
}

static void handle_shop_input(AppState *app) {
    Rectangle slot0 = Rf(744, 316, 150, 210);
    Rectangle slot1 = Rf(926, 316, 150, 210);
    Rectangle voucher = Rf(744, 584, 150, 198);
    Rectangle buffoon = Rf(1088, 584, 150, 198);
    Rectangle arcana = Rf(1260, 584, 150, 198);

    if (hover(slot0) && !app->shop.slots[0].sold) {
        Offer *o = &app->shop.slots[0];
        if (o->type == OFFER_JOKER) set_tooltip(app, "%s\n%s\nEdicao: %s\nRaridade: %s", joker_name(o->joker), joker_desc(o->joker), gui_edition_name(o->edition), joker_rarity_name(o->joker));
        else if (o->type == OFFER_TAROT) set_tooltip(app, "%s\n%s", tarot_display_name(o->tarot), tarot_desc(o->tarot));
        if (clicked(slot0)) buy_offer(app, o);
    }
    if (hover(slot1) && !app->shop.slots[1].sold) {
        Offer *o = &app->shop.slots[1];
        if (o->type == OFFER_JOKER) set_tooltip(app, "%s\n%s\nEdicao: %s\nRaridade: %s", joker_name(o->joker), joker_desc(o->joker), gui_edition_name(o->edition), joker_rarity_name(o->joker));
        else if (o->type == OFFER_TAROT) set_tooltip(app, "%s\n%s", tarot_display_name(o->tarot), tarot_desc(o->tarot));
        if (clicked(slot1)) buy_offer(app, o);
    }
    if (hover(voucher) && !app->shop.voucher.sold) {
        set_tooltip(app, "%s\n%s", coupon_name(app->shop.voucher.coupon), coupon_desc(app->shop.voucher.coupon));
        if (clicked(voucher)) buy_offer(app, &app->shop.voucher);
    }
    if (hover(buffoon) && !app->shop.packs[0].sold) {
        set_tooltip(app, "%s\n%s", pack_name(app->shop.packs[0].pack_kind), pack_desc(app->shop.packs[0].pack_kind));
        if (clicked(buffoon)) buy_offer(app, &app->shop.packs[0]);
    }
    if (hover(arcana) && !app->shop.packs[1].sold) {
        set_tooltip(app, "%s\n%s", pack_name(app->shop.packs[1].pack_kind), pack_desc(app->shop.packs[1].pack_kind));
        if (clicked(arcana)) buy_offer(app, &app->shop.packs[1]);
    }

    for (int i = 0; i < app->build.joker_count; i++) {
        Rectangle box = joker_hit_rect_for_count(i, app->build.joker_count, app->selected_joker_index);
        if (hover(box)) set_tooltip(app, "%s\n%s\nEdicao: %s\nRaridade: %s", joker_name(app->build.jokers[i].type), joker_desc(app->build.jokers[i].type), gui_edition_name(app->build.jokers[i].edition), joker_rarity_name(app->build.jokers[i].type));
        if (hover(box) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) app->selected_joker_index = i;
    }
    for (int i = 0; i < app->build.tarot_count; i++) {
        Rectangle box = tarot_rect(i);
        if (hover(box)) set_tooltip(app, "%s\n%s", tarot_display_name(app->build.tarot_inventory[i]), tarot_desc(app->build.tarot_inventory[i]));
        if (clicked(box)) open_inventory_tarot(app, i);
    }

    if (clicked(Rf(566, 392, 210, 96))) next_phase(app);
    if (clicked(Rf(566, 504, 210, 96))) reroll_shop(app);
    if (clicked(Rf(60, 718, 142, 108))) open_run_info(app, SCREEN_SHOP);
}

static void draw_shop_screen(AppState *app) {
    draw_background();
    draw_sidebar(app);
    draw_panel(Rf(428, 40, 1134, 778), Fade(BLACK, 0.9f), WHITE);
    draw_panel(Rf(468, 80, 580, 92), rgb(239, 245, 64), WHITE);
    DrawText("MELHORE SUA PARTIDA", 538, 102, 24, BLACK);
    DrawText("LOJA", 824, 96, 56, BLACK);

    for (int i = 0; i < app->build.joker_count; i++) draw_inventory_joker(&app->build.jokers[i], joker_rect(i), app->selected_joker_index == i);
    for (int i = 0; i < app->build.tarot_count; i++) draw_tarot_visual(app->build.tarot_inventory[i], tarot_rect(i), 0);

    draw_button(Rf(566, 392, 210, 96), "Proxima\nRodada", Fade(BLACK, 0.9f), 28);
    draw_button(Rf(566, 504, 210, 96), app->free_rerolls > 0 ? TextFormat("Atualizar\nGratis x%d", app->free_rerolls) : TextFormat("Atualizar\n$%d", app->shop.reroll_cost), Fade(BLACK, 0.9f), 28);
    if (app->free_pack_credits > 0) DrawText(TextFormat("Pacotes gratis: %d", app->free_pack_credits), 1088, 546, 22, rgb(239, 245, 64));

    draw_offer_card(&app->shop.slots[0], Rf(744, 316, 150, 210));
    draw_offer_card(&app->shop.slots[1], Rf(926, 316, 150, 210));
    draw_offer_card(&app->shop.voucher, Rf(744, 584, 150, 198));
    draw_offer_card(&app->shop.packs[0], Rf(1088, 584, 150, 198));
    draw_offer_card(&app->shop.packs[1], Rf(1260, 584, 150, 198));

    draw_joker_detail_panel(app, Rf(1188, 288, 332, 250));
    handle_shop_input(app);
    draw_status_bar(app);
}

static void handle_pack_input(AppState *app) {
    for (int i = 0; i < app->pack.choice_count; i++) {
        int row = app->pack.choice_count <= 3 ? 0 : i / 5;
        int col = app->pack.choice_count <= 3 ? i : i % 5;
        Rectangle box = Rf(344.0f + col * 180.0f, 272.0f + row * 240.0f, 150.0f, 210.0f);
        if (app->pack.choices[i].sold) continue;
        if (hover(box)) {
            if (app->pack.choices[i].type == OFFER_JOKER) set_tooltip(app, "%s\n%s\nEdicao: %s\nRaridade: %s", joker_name(app->pack.choices[i].joker), joker_desc(app->pack.choices[i].joker), gui_edition_name(app->pack.choices[i].edition), joker_rarity_name(app->pack.choices[i].joker));
            else set_tooltip(app, "%s\n%s", tarot_display_name(app->pack.choices[i].tarot), tarot_desc(app->pack.choices[i].tarot));
            if (clicked(box)) choose_pack_offer(app, i);
        }
    }
    if (clicked(Rf(716, 770, 170, 54))) {
        ignore_open_pack(app);
    }
}

static void draw_pack_screen(AppState *app) {
    draw_background();
    draw_background_overlay();
    draw_panel(Rf(218, 120, 1160, 650), Fade(rgb(30, 36, 44), 0.97f), rgb(236, 165, 34));
    DrawText(pack_name(app->pack.kind), 350, 158, 48, WHITE);
    DrawText(pack_desc(app->pack.kind), 352, 214, 26, Fade(WHITE, 0.82f));
    DrawText(TextFormat("Escolhas restantes: %d", app->pack.picks_remaining), 1006, 176, 28, rgb(247, 188, 68));

    for (int i = 0; i < app->pack.choice_count; i++) {
        int row = app->pack.choice_count <= 3 ? 0 : i / 5;
        int col = app->pack.choice_count <= 3 ? i : i % 5;
        Rectangle box = Rf(344.0f + col * 180.0f, 272.0f + row * 240.0f, 150.0f, 210.0f);
        draw_offer_card(&app->pack.choices[i], box);
    }

    draw_button(Rf(716, 770, 170, 54), "Ignorar", rgb(94, 109, 122), 24);
    handle_pack_input(app);
    draw_status_bar(app);
}

static void handle_tarot_apply_input(AppState *app) {
    for (int i = 0; i < app->tarot_apply.sample_count; i++) {
        int row = i / 5;
        int col = i % 5;
        Rectangle box = Rf(430.0f + col * 138.0f, 260.0f + row * 178.0f, 118.0f, 156.0f);
        if (hover(box)) {
            set_tooltip(app, "%s de %s\n%s", rank_name(app->tarot_apply.sample_cards[i].rank), suit_name(app->tarot_apply.sample_cards[i].suit), enhancement_desc(app->tarot_apply.sample_cards[i].enhancement));
            if (clicked(box)) {
                int selected_count = 0;
                for (int j = 0; j < app->tarot_apply.sample_count; j++) if (app->tarot_apply.selected[j]) selected_count++;
                if (app->tarot_apply.selected[i]) app->tarot_apply.selected[i] = 0;
                else if (selected_count < app->tarot_apply.target_count_required) app->tarot_apply.selected[i] = 1;
            }
        }
    }
    if (clicked(Rf(610, 760, 180, 54))) confirm_tarot_apply(app);
    if (clicked(Rf(822, 760, 180, 54))) {
        app->tarot_apply.active = 0;
        app->screen = app->tarot_apply.return_screen;
    }
}

static void draw_tarot_apply_screen(AppState *app) {
    draw_background();
    draw_background_overlay();
    draw_panel(Rf(260, 120, 1080, 700), Fade(rgb(30, 36, 44), 0.97f), rgb(196, 133, 208));
    DrawText(tarot_display_name(app->tarot_apply.tarot), 336, 160, 46, WHITE);
    draw_multiline(tarot_desc(app->tarot_apply.tarot), 338, 214, 22, Fade(WHITE, 0.82f));
    for (int i = 0; i < app->tarot_apply.sample_count; i++) {
        int row = i / 5;
        int col = i % 5;
        Rectangle box = Rf(430.0f + col * 138.0f, 260.0f + row * 178.0f, 118.0f, 156.0f);
        draw_card_visual(app->tarot_apply.sample_cards[i], box, app->tarot_apply.selected[i] != 0);
    }
    draw_button(Rf(610, 760, 180, 54), "Aplicar", rgb(104, 179, 116), 24);
    draw_button(Rf(822, 760, 180, 54), "Cancelar", rgb(214, 80, 89), 24);
    handle_tarot_apply_input(app);
    draw_status_bar(app);
}

static int rec_sum_int(const int *values, int count) {
    if (!values || count <= 0) return 0;
    return values[0] + rec_sum_int(values + 1, count - 1);
}

static int rec_min_int(const int *values, int count) {
    int rest;
    if (!values || count <= 0) return 0;
    if (count == 1) return values[0];
    rest = rec_min_int(values + 1, count - 1);
    return values[0] < rest ? values[0] : rest;
}

static int rec_max_int(const int *values, int count) {
    int rest;
    if (!values || count <= 0) return 0;
    if (count == 1) return values[0];
    rest = rec_max_int(values + 1, count - 1);
    return values[0] > rest ? values[0] : rest;
}

static double rec_sum_sq_diff_int(const int *values, int count, double mean) {
    double diff;
    if (!values || count <= 0) return 0.0;
    diff = values[0] - mean;
    return diff * diff + rec_sum_sq_diff_int(values + 1, count - 1, mean);
}

static int favorite_index_from_counts(const int *values, int count) {
    int best = 0;
    for (int i = 1; i < count; i++) {
        if (values[i] > values[best]) best = i;
    }
    return best;
}

static const char *run_strategy_hint(const AppState *app) {
    int total_quiz = rec_sum_int(app->stats.quiz_answer_counts, 4);
    double hit_rate = total_quiz > 0 ? (double)app->stats.quiz_correct_answers / (double)total_quiz : 0.0;
    if (app->run_won) return "Voce ganhou, entao vou fingir que era meu plano. Agora tenta vencer gastando menos atualizacao.";
    if (hit_rate < 0.45) return "Quiz te derrubou. Leia a dica, memorize a pergunta repetida e pare de alimentar meu ego.";
    if (app->stats.hands_played > 12 && app->stats.best_hand_score < app->phase.goal / 4) return "Muitas maos pequenas. Busque Flush, Full House ou Sequencia antes de gastar todas as tentativas.";
    if (app->stats.rerolls > 5 && app->stats.packs_opened < 2) return "Voce rodou loja demais. Pacotes e tarots escalam melhor quando a run comeca a apertar.";
    return "Foi quase. Eu odeio admitir, mas faltou transformar boas cartas em uma mao grande antes do boss.";
}

static HandType most_played_hand(const RunStats *stats) {
    HandType best = HAND_HIGH_CARD;
    int best_count = stats->hand_play_counts[HAND_HIGH_CARD];
    for (int i = 1; i < MAX_HAND_LEVEL_TRACK; i++) {
        if (stats->hand_play_counts[i] > best_count) {
            best = (HandType)i;
            best_count = stats->hand_play_counts[i];
        }
    }
    return best;
}

static void draw_end_screen(AppState *app) {
    HandType most_played;
    int most_played_count;
    char podium_text[128];
    int total_quiz = rec_sum_int(app->stats.quiz_answer_counts, 4);
    int max_choice = rec_max_int(app->stats.quiz_answer_counts, 4);
    int min_choice = rec_min_int(app->stats.quiz_answer_counts, 4);
    int favorite_answer = favorite_index_from_counts(app->stats.quiz_answer_counts, 4);
    double mean_choice = total_quiz > 0 ? (double)total_quiz / 4.0 : 0.0;
    double spread_choice = sqrt(rec_sum_sq_diff_int(app->stats.quiz_answer_counts, 4, mean_choice) / 4.0);
    int favorite_joker = favorite_index_from_counts(app->stats.joker_use_counts, GUI_JOKER_TEXTURE_COUNT);
    int favorite_tarot = favorite_index_from_counts(app->stats.tarot_use_counts, GUI_TAROT_TEXTURE_COUNT);
    draw_background();
    draw_background_overlay();
    draw_villain_card(app, Rf(252, 454, 140, 194), app->run_won ? "Ok, voce passou.\nNao se acostuma." : "Sou literalmente\num bobo... e voce\nperdeu pra mim?", !app->run_won);
    draw_panel(Rf(1248, 206, 286, 376), Fade(rgb(24, 30, 38), 0.96f), rgb(239, 245, 64));
    DrawText("Analise", 1292, 228, 34, rgb(239, 245, 64));
    DrawText(TextFormat("Letra favorita: %c", 'A' + favorite_answer), 1272, 284, 20, WHITE);
    DrawText(TextFormat("Acerto quiz: %.0f%%", total_quiz ? (100.0 * app->stats.quiz_correct_answers / total_quiz) : 0.0), 1272, 316, 20, WHITE);
    DrawText(TextFormat("Chutes min/max: %d/%d", min_choice, max_choice), 1272, 348, 20, WHITE);
    DrawText(TextFormat("Desvio escolhas: %.1f", spread_choice), 1272, 380, 20, WHITE);
    draw_wrapped_text(TextFormat("Coringa fav.: %s", favorite_joker > 0 ? joker_name((JokerType)favorite_joker) : "Nenhum"),
                      Rf(1272, 424, 236, 42), 18, 4, WHITE, 1);
    draw_wrapped_text(TextFormat("Tarot fav.: %s", favorite_tarot > 0 ? tarot_display_name((TarotType)favorite_tarot) : "Nenhum"),
                      Rf(1272, 470, 236, 42), 18, 4, WHITE, 1);
    draw_wrapped_text(run_strategy_hint(app), Rf(1272, 520, 236, 52), 17, 4, rgb(239, 245, 64), 1);
    draw_panel(Rf(382, 112, 860, 676), Fade(rgb(28, 35, 44), 0.97f), app->run_won ? rgb(58, 167, 129) : rgb(214, 82, 83));
    DrawText(app->run_won ? "RUN CONCLUIDA" : "FIM DE JOGO", 552, 148, 70, app->run_won ? rgb(86, 220, 158) : rgb(239, 82, 82));

    most_played = most_played_hand(&app->stats);
    most_played_count = app->stats.hand_play_counts[most_played];

    draw_panel(Rf(480, 258, 670, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    DrawText("Jogador / Modalidade", 520, 272, 24, rgb(40, 44, 50));
    draw_wrapped_text(TextFormat("%s | %s", app->active_player, player_modality_name_from_mask(app->question_mask)),
                      Rf(810, 272, 324, 28), 22, 2, rgb(239, 82, 82), 1);

    draw_panel(Rf(480, 324, 670, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    DrawText("Pontuacao Total", 520, 338, 24, rgb(40, 44, 50));
    DrawText(TextFormat("%d pts", app->stats.total_points), 964, 338, 24, rgb(40, 44, 50));

    draw_panel(Rf(480, 392, 304, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    draw_panel(Rf(846, 392, 304, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    DrawText("Melhor Mao", 510, 406, 24, rgb(40, 44, 50));
    DrawText(TextFormat("%d", app->stats.best_hand_score), 726, 406, 24, rgb(18, 108, 212));
    DrawText("Aposta", 878, 406, 24, rgb(40, 44, 50));
    DrawText(TextFormat("%d", ((app->phase_number - 1) / 3) + 1), 1080, 406, 24, rgb(58, 167, 129));

    draw_panel(Rf(480, 458, 304, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    draw_panel(Rf(846, 458, 304, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    DrawText("Mao Mais Jogada", 510, 472, 24, rgb(40, 44, 50));
    draw_wrapped_text(TextFormat("%s (%d)", hand_type_name(most_played), most_played_count), Rf(638, 472, 132, 28), 22, 2, rgb(239, 82, 82), 1);
    DrawText("Rodada", 878, 472, 24, rgb(40, 44, 50));
    DrawText(TextFormat("%d", blind_kind_from_phase(app->phase_number) + 1), 1080, 472, 24, rgb(239, 185, 68));

    draw_panel(Rf(480, 524, 304, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    draw_panel(Rf(846, 524, 304, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    DrawText("Maos Jogadas", 510, 538, 24, rgb(40, 44, 50));
    DrawText(TextFormat("%d", app->stats.hands_played), 700, 538, 24, rgb(58, 167, 129));
    DrawText("Derrotado por", 878, 538, 24, rgb(40, 44, 50));
    draw_wrapped_text(app->stats.defeated_by[0] ? app->stats.defeated_by : "Nenhum", Rf(1006, 538, 128, 28), 22, 2, rgb(40, 44, 50), 1);

    draw_panel(Rf(480, 590, 304, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    draw_panel(Rf(846, 590, 304, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    DrawText("Ranking da Modalidade", 510, 604, 24, rgb(40, 44, 50));
    if (app->last_rank_position > 0 && app->last_rank_position <= PLAYER_TOP_LIMIT) {
        snprintf(podium_text, sizeof(podium_text), "Top 10: #%d de %d", app->last_rank_position, app->last_rank_total);
    } else if (app->last_rank_position > 0) {
        snprintf(podium_text, sizeof(podium_text), "Fora do Top 10: #%d de %d", app->last_rank_position, app->last_rank_total);
    } else {
        snprintf(podium_text, sizeof(podium_text), "Sem ranking salvo ainda");
    }
    draw_wrapped_text(podium_text, Rf(706, 604, 72, 28), 22, 2, rgb(18, 108, 212), 1);
    DrawText("Packs Abertos", 878, 604, 24, rgb(40, 44, 50));
    DrawText(TextFormat("%d", app->stats.packs_opened), 1066, 604, 24, rgb(18, 108, 212));

    draw_panel(Rf(480, 656, 670, 54), rgb(214, 217, 226), rgb(54, 60, 68));
    DrawText("Codigo", 520, 670, 24, rgb(40, 44, 50));
    DrawText(app->stats.run_code, 930, 670, 28, rgb(18, 108, 212));

    draw_button(Rf(430, 732, 186, 58), "Nova Tentativa", rgb(214, 82, 83), 24);
    draw_button(Rf(636, 732, 160, 58), "Historico", Fade(BLACK, 0.9f), 24);
    draw_button(Rf(816, 732, 180, 58), "Desvincular", rgb(214, 82, 83), 24);
    draw_button(Rf(1016, 732, 180, 58), "Menu Principal", rgb(214, 82, 83), 22);
    if (clicked(Rf(430, 732, 186, 58))) start_run(app);
    if (clicked(Rf(636, 732, 160, 58))) {
        snprintf(app->history_query, sizeof(app->history_query), "%s", app->active_player);
        app->history_focus = 1;
        app->screen = SCREEN_HISTORY;
    }
    if (clicked(Rf(816, 732, 180, 58))) {
        unlink_active_player(app);
        app->screen = SCREEN_TITLE;
    }
    if (clicked(Rf(1016, 732, 180, 58))) app->screen = SCREEN_TITLE;
}

int main(void) {
    int monitor;
    int monitor_width;
    int monitor_height;
    RenderTexture2D render_target;
    srand((unsigned int)time(NULL));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_W, SCREEN_H, "Balatro Academico - GUI integrada");
    monitor = GetCurrentMonitor();
    monitor_width = GetMonitorWidth(monitor);
    monitor_height = GetMonitorHeight(monitor);
    SetWindowSize(monitor_width, monitor_height);
    SetWindowPosition(0, 0);
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);
    render_target = LoadRenderTexture(SCREEN_W, SCREEN_H);

    AppState app;
    memset(&app, 0, sizeof(app));
    app.screen = SCREEN_TITLE;
    app.selected_joker_index = -1;
    app.selected_tarot_index = -1;
    app.dragging_card_index = -1;
    app.dragging_joker_index = -1;
    app.profile_focus = 1;
    app.history_focus = 1;
    app.leaderboard_mode = LEADERBOARD_MATEMATICOS;
    app.collection_tab = COLLECTION_JOKERS;
    app.collection_page = 0;
    app.draft_question_mask = PLAYER_MASK_LOGIC | PLAYER_MASK_MAT;
    app.last_rank_position = -1;
    app.pending_quiz_log_index = -1;
    app.current_quiz_log_index = -1;
    load_active_collection(&app);
    load_asset_pack(&app.assets);
    g_assets = &app.assets;
    set_status(&app, "Pronto para iniciar a run.");

    while (!WindowShouldClose()) {
        app.tooltip[0] = '\0';
        update_render_layout();
        BeginTextureMode(render_target);
        ClearBackground(BLACK);
        switch (app.screen) {
            case SCREEN_TITLE:
                draw_title_screen(&app);
                break;
            case SCREEN_PROFILE:
                draw_profile_screen(&app);
                break;
            case SCREEN_LEADERBOARD:
                draw_leaderboard_screen(&app);
                break;
            case SCREEN_HISTORY:
                draw_history_screen(&app);
                break;
            case SCREEN_COLLECTION:
                draw_collection_screen(&app);
                break;
            case SCREEN_RUN_INFO:
                draw_run_info_screen(&app);
                break;
            case SCREEN_PLAY:
                draw_play_screen(&app);
                break;
            case SCREEN_REWARD:
                draw_reward_screen(&app);
                break;
            case SCREEN_QUIZ:
                draw_quiz_screen(&app);
                break;
            case SCREEN_ROUND_PREVIEW:
                draw_round_preview_screen(&app);
                break;
            case SCREEN_SHOP:
                draw_shop_screen(&app);
                break;
            case SCREEN_PACK:
                draw_pack_screen(&app);
                break;
            case SCREEN_TAROT_APPLY:
                draw_tarot_apply_screen(&app);
                break;
            case SCREEN_END:
                draw_end_screen(&app);
                break;
        }
        draw_tooltip_box(&app);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(render_target.texture,
                       Rf(0, 0, (float)SCREEN_W, (float)-SCREEN_H),
                       g_render_view,
                       (Vector2){0, 0},
                       0.0f,
                       WHITE);
        EndDrawing();
    }

    unload_asset_pack(&app.assets);
    UnloadRenderTexture(render_target);
    CloseWindow();
    return 0;
}
