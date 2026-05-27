#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PLAYER_NAME_CAP 48
#define PLAYER_MODALITY_CAP 32
#define PLAYER_TIME_CAP 32
#define PLAYER_RUN_CODE_CAP 16
#define PLAYER_DEFEATED_CAP 32
#define PLAYER_HISTORY_FILE "run_history_raylib.txt"
#define PLAYER_COLLECTION_FILE "player_collection_raylib.txt"
#define PLAYER_MAX_RECORDS 1024
#define PLAYER_TOP_LIMIT 10

#define PLAYER_MASK_LOGIC 1
#define PLAYER_MASK_MAT 2
#define PLAYER_MASK_PROG 4
#define PLAYER_MASK_PORT 8
#define PLAYER_MASK_ALL (PLAYER_MASK_LOGIC | PLAYER_MASK_MAT | PLAYER_MASK_PROG | PLAYER_MASK_PORT)

typedef enum {
    LEADERBOARD_MATEMATICOS = 0,
    LEADERBOARD_LINGUISTICOS,
    LEADERBOARD_LOGICOS,
    LEADERBOARD_PROGRAMADORES,
    LEADERBOARD_ESCOLARIZADOS,
    LEADERBOARD_CONCURSEIROS,
    LEADERBOARD_ENGENHEIROS,
    LEADERBOARD_INTELECTUAIS,
    LEADERBOARD_MISTO,
    LEADERBOARD_COUNT
} LeaderboardMode;

typedef struct {
    char timestamp[PLAYER_TIME_CAP];
    char player[PLAYER_NAME_CAP];
    char run_code[PLAYER_RUN_CODE_CAP];
    char modality[PLAYER_MODALITY_CAP];
    char defeated_by[PLAYER_DEFEATED_CAP];
    int question_mask;
    int total_points;
    int best_hand_score;
    int phases_cleared;
    int antes_cleared;
    int hands_played;
    int cards_discarded;
    int cards_bought;
    int rerolls;
    int packs_opened;
    int tarots_used;
    int quiz_hits;
    int coins;
    int run_won;
    int bet_reached;
    int blind_reached;
} RunRecord;

typedef struct {
    char player[PLAYER_NAME_CAP];
    unsigned long long joker_bits;
    unsigned long long coupon_bits;
    unsigned long long mark_bits;
    unsigned long long boss_bits;
    unsigned long long tarot_bits;
} PlayerCollection;

static void player_copy_clean(char *dest, size_t dest_size, const char *src);
static int player_casecmp(const char *a, const char *b);

static int player_mask_is_valid(int mask) {
    return (mask & PLAYER_MASK_ALL) != 0;
}

static int player_name_is_admin(const char *name) {
    return name && player_casecmp(name, "Admin") == 0;
}

static unsigned long long player_collection_bit(int index) {
    if (index <= 0 || index > 63) return 0ULL;
    return 1ULL << (index - 1);
}

static void player_collection_clear(PlayerCollection *collection) {
    if (!collection) return;
    memset(collection, 0, sizeof(*collection));
}

static void player_collection_init(PlayerCollection *collection, const char *player_name) {
    if (!collection) return;
    player_collection_clear(collection);
    if (player_name) player_copy_clean(collection->player, sizeof(collection->player), player_name);
    if (player_name_is_admin(collection->player)) {
        collection->joker_bits = ~0ULL;
        collection->coupon_bits = ~0ULL;
        collection->mark_bits = ~0ULL;
        collection->boss_bits = ~0ULL;
        collection->tarot_bits = ~0ULL;
    }
}

static int player_collection_has(unsigned long long bits, int index) {
    unsigned long long bit = player_collection_bit(index);
    return bit != 0ULL && (bits & bit) != 0ULL;
}

static int player_collection_count_bits(unsigned long long bits, int total_items) {
    int count = 0;
    for (int i = 1; i <= total_items; i++) if (player_collection_has(bits, i)) count++;
    return count;
}

static void player_collection_unlock(unsigned long long *bits, int index) {
    if (!bits) return;
    *bits |= player_collection_bit(index);
}

static int player_collection_load(const char *path, const char *player_name, PlayerCollection *collection) {
    FILE *file;
    char line[512];

    if (!collection || !player_name) return 0;
    player_collection_init(collection, player_name);
    if (player_name_is_admin(collection->player)) return 1;

    file = fopen(path ? path : PLAYER_COLLECTION_FILE, "r");
    if (!file) return 0;

    while (fgets(line, sizeof(line), file)) {
        char *parts[6];
        char *cursor = line;
        char *sep;
        int count = 0;

        while (count < 6) {
            parts[count++] = cursor;
            sep = strchr(cursor, '|');
            if (!sep) break;
            *sep = '\0';
            cursor = sep + 1;
        }
        if (count != 6) continue;

        {
            PlayerCollection parsed;
            player_collection_clear(&parsed);
            player_copy_clean(parsed.player, sizeof(parsed.player), parts[0]);
            parsed.joker_bits = strtoull(parts[1], NULL, 10);
            parsed.coupon_bits = strtoull(parts[2], NULL, 10);
            parsed.mark_bits = strtoull(parts[3], NULL, 10);
            parsed.boss_bits = strtoull(parts[4], NULL, 10);
            parsed.tarot_bits = strtoull(parts[5], NULL, 10);
            if (player_casecmp(parsed.player, collection->player) == 0) {
                *collection = parsed;
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

static int player_collection_save(const char *path, const PlayerCollection *collection) {
    PlayerCollection entries[256];
    int count = 0;
    int replaced = 0;
    FILE *file;
    char line[512];

    if (!collection || !collection->player[0]) return 0;
    if (player_name_is_admin(collection->player)) return 1;

    file = fopen(path ? path : PLAYER_COLLECTION_FILE, "r");
    if (file) {
        while (fgets(line, sizeof(line), file) && count < 256) {
            char *parts[6];
            char *cursor = line;
            char *sep;
            int parsed_count = 0;

            while (parsed_count < 6) {
                parts[parsed_count++] = cursor;
                sep = strchr(cursor, '|');
                if (!sep) break;
                *sep = '\0';
                cursor = sep + 1;
            }
            if (parsed_count != 6) continue;

            player_collection_clear(&entries[count]);
            player_copy_clean(entries[count].player, sizeof(entries[count].player), parts[0]);
            entries[count].joker_bits = strtoull(parts[1], NULL, 10);
            entries[count].coupon_bits = strtoull(parts[2], NULL, 10);
            entries[count].mark_bits = strtoull(parts[3], NULL, 10);
            entries[count].boss_bits = strtoull(parts[4], NULL, 10);
            entries[count].tarot_bits = strtoull(parts[5], NULL, 10);
            count++;
        }
        fclose(file);
    }

    for (int i = 0; i < count; i++) {
        if (player_casecmp(entries[i].player, collection->player) == 0) {
            entries[i] = *collection;
            replaced = 1;
            break;
        }
    }
    if (!replaced && count < 256) entries[count++] = *collection;

    file = fopen(path ? path : PLAYER_COLLECTION_FILE, "w");
    if (!file) return 0;
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%llu|%llu|%llu|%llu|%llu\n",
                entries[i].player,
                entries[i].joker_bits,
                entries[i].coupon_bits,
                entries[i].mark_bits,
                entries[i].boss_bits,
                entries[i].tarot_bits);
    }
    fclose(file);
    return 1;
}

static void player_copy_clean(char *dest, size_t dest_size, const char *src) {
    size_t write = 0;
    size_t start = 0;
    size_t end = 0;

    if (!dest || dest_size == 0) return;
    dest[0] = '\0';
    if (!src) return;

    while (src[start] && isspace((unsigned char)src[start])) start++;
    end = strlen(src);
    while (end > start && isspace((unsigned char)src[end - 1])) end--;

    for (size_t i = start; i < end && write + 1 < dest_size; i++) {
        char ch = src[i];
        if (ch == '|' || ch == '\n' || ch == '\r' || ch == '\t') ch = ' ';
        dest[write++] = ch;
    }
    dest[write] = '\0';
}

static int player_casecmp(const char *a, const char *b) {
    unsigned char ca;
    unsigned char cb;
    if (!a) a = "";
    if (!b) b = "";
    while (*a || *b) {
        ca = (unsigned char)tolower((unsigned char)*a);
        cb = (unsigned char)tolower((unsigned char)*b);
        if (ca != cb) return (int)ca - (int)cb;
        if (*a) a++;
        if (*b) b++;
    }
    return 0;
}

static const char *player_category_name_from_mask_bit(int bit) {
    switch (bit) {
        case PLAYER_MASK_LOGIC: return "Logica Classica";
        case PLAYER_MASK_MAT: return "Logica Matematica";
        case PLAYER_MASK_PROG: return "Logica de Prog. em C";
        case PLAYER_MASK_PORT: return "Portugues";
        default: return "Categoria";
    }
}

static void player_mask_summary(int mask, char *buffer, size_t buffer_size) {
    const int bits[4] = {PLAYER_MASK_LOGIC, PLAYER_MASK_MAT, PLAYER_MASK_PROG, PLAYER_MASK_PORT};
    int first = 1;
    size_t used = 0;

    if (!buffer || buffer_size == 0) return;
    buffer[0] = '\0';

    for (int i = 0; i < 4; i++) {
        const char *name;
        int written;
        if ((mask & bits[i]) == 0) continue;
        name = player_category_name_from_mask_bit(bits[i]);
        written = snprintf(buffer + used, buffer_size - used, "%s%s", first ? "" : ", ", name);
        if (written < 0) return;
        if ((size_t)written >= buffer_size - used) {
            buffer[buffer_size - 1] = '\0';
            return;
        }
        used += (size_t)written;
        first = 0;
    }

    if (first) snprintf(buffer, buffer_size, "Nenhuma categoria");
}

static const char *player_modality_name_from_mask(int mask) {
    switch (mask & PLAYER_MASK_ALL) {
        case PLAYER_MASK_MAT:
            return "Matematicos";
        case PLAYER_MASK_PORT:
            return "Linguisticos";
        case PLAYER_MASK_LOGIC:
            return "Logicos";
        case PLAYER_MASK_PROG:
            return "Programadores";
        case PLAYER_MASK_MAT | PLAYER_MASK_PORT:
            return "Escolarizados";
        case PLAYER_MASK_MAT | PLAYER_MASK_PORT | PLAYER_MASK_LOGIC:
            return "Concurseiros";
        case PLAYER_MASK_MAT | PLAYER_MASK_LOGIC | PLAYER_MASK_PROG:
            return "Engenheiros";
        case PLAYER_MASK_ALL:
            return "Intelectuais";
        default:
            return "Perfil Misto";
    }
}

static LeaderboardMode player_leaderboard_mode_from_mask(int mask) {
    switch (mask & PLAYER_MASK_ALL) {
        case PLAYER_MASK_MAT:
            return LEADERBOARD_MATEMATICOS;
        case PLAYER_MASK_PORT:
            return LEADERBOARD_LINGUISTICOS;
        case PLAYER_MASK_LOGIC:
            return LEADERBOARD_LOGICOS;
        case PLAYER_MASK_PROG:
            return LEADERBOARD_PROGRAMADORES;
        case PLAYER_MASK_MAT | PLAYER_MASK_PORT:
            return LEADERBOARD_ESCOLARIZADOS;
        case PLAYER_MASK_MAT | PLAYER_MASK_PORT | PLAYER_MASK_LOGIC:
            return LEADERBOARD_CONCURSEIROS;
        case PLAYER_MASK_MAT | PLAYER_MASK_LOGIC | PLAYER_MASK_PROG:
            return LEADERBOARD_ENGENHEIROS;
        case PLAYER_MASK_ALL:
            return LEADERBOARD_INTELECTUAIS;
        default:
            return LEADERBOARD_MISTO;
    }
}

static const char *leaderboard_mode_name(LeaderboardMode mode) {
    switch (mode) {
        case LEADERBOARD_MATEMATICOS: return "Matematicos";
        case LEADERBOARD_LINGUISTICOS: return "Linguisticos";
        case LEADERBOARD_LOGICOS: return "Logicos";
        case LEADERBOARD_PROGRAMADORES: return "Programadores";
        case LEADERBOARD_ESCOLARIZADOS: return "Escolarizados";
        case LEADERBOARD_CONCURSEIROS: return "Concurseiros";
        case LEADERBOARD_ENGENHEIROS: return "Engenheiros";
        case LEADERBOARD_INTELECTUAIS: return "Intelectuais";
        case LEADERBOARD_MISTO: return "Perfil Misto";
        default: return "Modalidade";
    }
}

static int leaderboard_mode_matches_mask(LeaderboardMode mode, int mask) {
    return player_leaderboard_mode_from_mask(mask) == mode;
}

static void player_fill_timestamp(char *buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm *local;
    if (!buffer || buffer_size == 0) return;
    buffer[0] = '\0';
    local = localtime(&now);
    if (!local) return;
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M", local);
}

static int player_record_equals(const RunRecord *a, const RunRecord *b) {
    if (!a || !b) return 0;
    return strcmp(a->timestamp, b->timestamp) == 0 &&
           strcmp(a->player, b->player) == 0 &&
           strcmp(a->run_code, b->run_code) == 0 &&
           a->total_points == b->total_points;
}

static int player_record_compare_desc(const void *lhs, const void *rhs) {
    const RunRecord *a = (const RunRecord *)lhs;
    const RunRecord *b = (const RunRecord *)rhs;
    if (a->total_points != b->total_points) return b->total_points - a->total_points;
    if (a->phases_cleared != b->phases_cleared) return b->phases_cleared - a->phases_cleared;
    if (a->best_hand_score != b->best_hand_score) return b->best_hand_score - a->best_hand_score;
    if (a->coins != b->coins) return b->coins - a->coins;
    return strcmp(b->timestamp, a->timestamp);
}

static int player_append_run_record(const char *path, const RunRecord *record) {
    FILE *file;
    if (!record) return 0;
    file = fopen(path ? path : PLAYER_HISTORY_FILE, "a");
    if (!file) return 0;
    fprintf(file,
            "%s|%s|%s|%s|%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d\n",
            record->timestamp,
            record->player,
            record->run_code,
            record->modality,
            record->defeated_by,
            record->question_mask,
            record->total_points,
            record->best_hand_score,
            record->phases_cleared,
            record->antes_cleared,
            record->hands_played,
            record->cards_discarded,
            record->cards_bought,
            record->rerolls,
            record->packs_opened,
            record->tarots_used,
            record->quiz_hits,
            record->coins,
            record->run_won,
            record->bet_reached,
            record->blind_reached);
    fclose(file);
    return 1;
}

static int player_parse_run_record(char *line, RunRecord *record) {
    char *parts[21];
    int count = 0;
    char *cursor = line;
    char *sep;

    if (!line || !record) return 0;
    while (count < 21) {
        parts[count++] = cursor;
        sep = strchr(cursor, '|');
        if (!sep) break;
        *sep = '\0';
        cursor = sep + 1;
    }
    if (count != 21) return 0;

    player_copy_clean(record->timestamp, sizeof(record->timestamp), parts[0]);
    player_copy_clean(record->player, sizeof(record->player), parts[1]);
    player_copy_clean(record->run_code, sizeof(record->run_code), parts[2]);
    player_copy_clean(record->modality, sizeof(record->modality), parts[3]);
    player_copy_clean(record->defeated_by, sizeof(record->defeated_by), parts[4]);
    record->question_mask = atoi(parts[5]);
    record->total_points = atoi(parts[6]);
    record->best_hand_score = atoi(parts[7]);
    record->phases_cleared = atoi(parts[8]);
    record->antes_cleared = atoi(parts[9]);
    record->hands_played = atoi(parts[10]);
    record->cards_discarded = atoi(parts[11]);
    record->cards_bought = atoi(parts[12]);
    record->rerolls = atoi(parts[13]);
    record->packs_opened = atoi(parts[14]);
    record->tarots_used = atoi(parts[15]);
    record->quiz_hits = atoi(parts[16]);
    record->coins = atoi(parts[17]);
    record->run_won = atoi(parts[18]);
    record->bet_reached = atoi(parts[19]);
    record->blind_reached = atoi(parts[20]);
    return 1;
}

static int player_load_run_records(const char *path, RunRecord *out, int max_out) {
    FILE *file;
    char line[1024];
    int count = 0;

    if (!out || max_out <= 0) return 0;
    file = fopen(path ? path : PLAYER_HISTORY_FILE, "r");
    if (!file) return 0;

    while (fgets(line, sizeof(line), file) && count < max_out) {
        RunRecord record;
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) line[len - 1] = '\0';
        len = strlen(line);
        if (len > 0 && line[len - 1] == '\r') line[len - 1] = '\0';
        if (!line[0]) continue;
        if (player_parse_run_record(line, &record)) out[count++] = record;
    }
    fclose(file);
    return count;
}

static int player_build_leaderboard(const RunRecord *records, int record_count, LeaderboardMode mode,
                                    RunRecord *top_out, int top_cap, const RunRecord *focus, int *focus_position) {
    RunRecord filtered[PLAYER_MAX_RECORDS];
    int filtered_count = 0;
    int copy_count;

    if (focus_position) *focus_position = -1;
    if (!records || record_count <= 0) return 0;

    for (int i = 0; i < record_count && filtered_count < PLAYER_MAX_RECORDS; i++) {
        if (leaderboard_mode_matches_mask(mode, records[i].question_mask)) filtered[filtered_count++] = records[i];
    }
    if (filtered_count <= 0) return 0;

    qsort(filtered, (size_t)filtered_count, sizeof(filtered[0]), player_record_compare_desc);
    if (focus && focus_position) {
        for (int i = 0; i < filtered_count; i++) {
            if (player_record_equals(&filtered[i], focus)) {
                *focus_position = i + 1;
                break;
            }
        }
    }

    copy_count = filtered_count < top_cap ? filtered_count : top_cap;
    if (top_out && top_cap > 0) {
        for (int i = 0; i < copy_count; i++) top_out[i] = filtered[i];
    }
    return filtered_count;
}

static int player_filter_history(const RunRecord *records, int record_count, const char *player_name,
                                 RunRecord *out, int out_cap) {
    int count = 0;
    if (!records || !player_name || !player_name[0] || !out || out_cap <= 0) return 0;
    for (int i = 0; i < record_count && count < out_cap; i++) {
        if (player_casecmp(records[i].player, player_name) == 0) out[count++] = records[i];
    }
    if (count > 0) qsort(out, (size_t)count, sizeof(out[0]), player_record_compare_desc);
    return count;
}

#endif
