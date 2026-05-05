/*
 * GUI opcional com raylib.
 * Compila somente se a biblioteca raylib estiver instalada:
 * gcc -std=c11 -Wall -Wextra -pedantic -Isrc src/raylib_main.c src/cards.c -lraylib -lm -o build/jogo_raylib
 */
#include "cards.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define W 1180
#define H 760

static const Color HEARTS = {199, 30, 78, 255};
static const Color DIAMONDS = {227, 149, 54, 255};
static const Color SPADES = {34, 15, 133, 255};
static const Color CLUBS = {40, 40, 41, 255};

typedef struct {
    int base_reward;
    int hands_reward;
    int interest_reward;
    int total_reward;
} ReceiptBreakdown;

static Rectangle card_rect(int i) {
    return (Rectangle){70 + i * 126.0f, 435, 100, 165};
}

static Color suit_color(int suit) {
    switch (suit) {
        case SUIT_COPAS: return HEARTS;
        case SUIT_OUROS: return DIAMONDS;
        case SUIT_ESPADAS: return SPADES;
        default: return CLUBS;
    }
}

static void draw_diamond(Vector2 center, float scale, Color color) {
    Vector2 top = {center.x, center.y - scale};
    Vector2 right = {center.x + scale * 0.75f, center.y};
    Vector2 bottom = {center.x, center.y + scale};
    Vector2 left = {center.x - scale * 0.75f, center.y};
    DrawTriangle(top, right, left, color);
    DrawTriangle(bottom, right, left, color);
}

static void draw_heart(Vector2 center, float scale, Color color) {
    DrawCircleV((Vector2){center.x - scale * 0.35f, center.y - scale * 0.1f}, scale * 0.42f, color);
    DrawCircleV((Vector2){center.x + scale * 0.35f, center.y - scale * 0.1f}, scale * 0.42f, color);
    DrawTriangle((Vector2){center.x - scale * 0.8f, center.y},
                 (Vector2){center.x + scale * 0.8f, center.y},
                 (Vector2){center.x, center.y + scale}, color);
}

static void draw_spade(Vector2 center, float scale, Color color) {
    DrawCircleV((Vector2){center.x - scale * 0.35f, center.y + scale * 0.1f}, scale * 0.42f, color);
    DrawCircleV((Vector2){center.x + scale * 0.35f, center.y + scale * 0.1f}, scale * 0.42f, color);
    DrawTriangle((Vector2){center.x - scale * 0.8f, center.y},
                 (Vector2){center.x + scale * 0.8f, center.y},
                 (Vector2){center.x, center.y - scale}, color);
    DrawRectangleRounded((Rectangle){center.x - scale * 0.14f, center.y + scale * 0.55f, scale * 0.28f, scale * 0.7f},
                         0.2f, 6, color);
}

static void draw_club(Vector2 center, float scale, Color color) {
    DrawCircleV((Vector2){center.x, center.y - scale * 0.32f}, scale * 0.36f, color);
    DrawCircleV((Vector2){center.x - scale * 0.38f, center.y + scale * 0.12f}, scale * 0.36f, color);
    DrawCircleV((Vector2){center.x + scale * 0.38f, center.y + scale * 0.12f}, scale * 0.36f, color);
    DrawRectangleRounded((Rectangle){center.x - scale * 0.14f, center.y + scale * 0.45f, scale * 0.28f, scale * 0.7f},
                         0.2f, 6, color);
}

static void draw_suit_symbol(int suit, Vector2 center, float scale) {
    Color color = suit_color(suit);
    switch (suit) {
        case SUIT_COPAS: draw_heart(center, scale, color); break;
        case SUIT_OUROS: draw_diamond(center, scale, color); break;
        case SUIT_ESPADAS: draw_spade(center, scale, color); break;
        default: draw_club(center, scale, color); break;
    }
}

static const char *seal_short(SealType seal) {
    switch (seal) {
        case SEAL_GOLD: return "OURO";
        case SEAL_RED: return "RED";
        case SEAL_BLUE: return "AZUL";
        case SEAL_PURPLE: return "ROXO";
        default: return "";
    }
}

static Color seal_color(SealType seal) {
    switch (seal) {
        case SEAL_GOLD: return GOLD;
        case SEAL_RED: return RED;
        case SEAL_BLUE: return SKYBLUE;
        case SEAL_PURPLE: return VIOLET;
        default: return LIGHTGRAY;
    }
}

static void draw_card_front(const Card *card, Rectangle rect, bool selected) {
    Color accent = suit_color(card->suit);
    Color border = selected ? GOLD : Fade(accent, 0.95f);
    float lift = selected ? 18.0f : 0.0f;
    rect.y -= lift;

    DrawRectangleRounded((Rectangle){rect.x + 4, rect.y + 8, rect.width, rect.height}, 0.14f, 10, Fade(BLACK, 0.22f));
    DrawRectangleRounded(rect, 0.14f, 10, RAYWHITE);
    DrawRectangleRoundedLinesEx(rect, 0.14f, 10, selected ? 4.0f : 2.0f, border);

    DrawText(rank_name(card->rank), (int)rect.x + 10, (int)rect.y + 10, 26, accent);
    draw_suit_symbol(card->suit, (Vector2){rect.x + 22, rect.y + 46}, 10.0f);

    int bottomTextW = MeasureText(rank_name(card->rank), 26);
    DrawText(rank_name(card->rank), (int)(rect.x + rect.width - bottomTextW - 10), (int)(rect.y + rect.height - 36), 26, accent);
    draw_suit_symbol(card->suit, (Vector2){rect.x + rect.width - 22, rect.y + rect.height - 20}, 10.0f);

    DrawRectangleRounded((Rectangle){rect.x + 14, rect.y + 58, rect.width - 28, rect.height - 88}, 0.14f, 8, Fade(accent, 0.08f));
    DrawRectangleRoundedLinesEx((Rectangle){rect.x + 14, rect.y + 58, rect.width - 28, rect.height - 88}, 0.14f, 8, 1.5f, Fade(accent, 0.3f));

    if (card->rank == 1 || card->rank >= 11) {
        DrawText(rank_name(card->rank), (int)rect.x + 34, (int)rect.y + 88, 48, accent);
        draw_suit_symbol(card->suit, (Vector2){rect.x + rect.width / 2.0f, rect.y + rect.height / 2.0f + 18}, 18.0f);
    } else {
        DrawText(rank_name(card->rank), (int)rect.x + 34, (int)rect.y + 78, 44, accent);
        draw_suit_symbol(card->suit, (Vector2){rect.x + rect.width / 2.0f, rect.y + rect.height / 2.0f + 20}, 16.0f);
    }

    if (card->wild_suit) {
        DrawCircle((int)(rect.x + rect.width - 18), (int)(rect.y + 18), 11, GOLD);
        DrawText("*", (int)(rect.x + rect.width - 22), (int)(rect.y + 8), 24, BLACK);
    }
    if (card->bonus_chips > 0) {
        DrawRectangleRounded((Rectangle){rect.x + 16, rect.y + rect.height - 56, 54, 20}, 0.2f, 6, Fade(SKYBLUE, 0.95f));
        DrawText(TextFormat("+%d", card->bonus_chips), (int)rect.x + 22, (int)rect.y + rect.height - 53, 16, DARKBLUE);
    }
    if (card->bonus_mult > 0) {
        DrawRectangleRounded((Rectangle){rect.x + rect.width - 68, rect.y + rect.height - 56, 52, 20}, 0.2f, 6, Fade(MAGENTA, 0.95f));
        DrawText(TextFormat("x+%d", card->bonus_mult), (int)rect.x + (int)rect.width - 62, (int)rect.y + rect.height - 53, 16, MAROON);
    }
    if (card->seal != SEAL_NONE) {
        DrawRectangleRounded((Rectangle){rect.x + 16, rect.y + rect.height - 28, rect.width - 32, 16}, 0.2f, 6, Fade(seal_color(card->seal), 0.9f));
        DrawText(seal_short(card->seal), (int)rect.x + 28, (int)rect.y + rect.height - 27, 14, WHITE);
    }
}

static int score_selected(Card *hand[], bool selected[], int hand_count, char *label, int label_size) {
    Card *cards[MAX_PLAY];
    int count = 0;
    for (int i = 0; i < hand_count && count < MAX_PLAY; i++) {
        if (selected[i]) cards[count++] = hand[i];
    }
    HandEval eval = evaluate_cards(cards, count);
    snprintf(label, (size_t)label_size, "%s | %d pts", eval.name, (eval.base_score + eval.rank_sum) * eval.multiplier);
    return (eval.base_score + eval.rank_sum) * eval.multiplier;
}

static ReceiptBreakdown build_receipt(int coins_before_reward, int hands_left) {
    ReceiptBreakdown receipt;
    receipt.base_reward = 4;
    receipt.hands_reward = hands_left > 0 ? hands_left : 0;
    receipt.interest_reward = coins_before_reward / 5;
    if (receipt.interest_reward > 5) receipt.interest_reward = 5;
    receipt.total_reward = receipt.base_reward + receipt.hands_reward + receipt.interest_reward;
    return receipt;
}

static void draw_receipt_modal(ReceiptBreakdown receipt, float progress) {
    Rectangle panel = {W / 2.0f - 220, H / 2.0f - 150 + (1.0f - progress) * 60.0f, 440, 300};
    DrawRectangle(0, 0, W, H, Fade(BLACK, 0.52f * progress));
    DrawRectangleRounded(panel, 0.08f, 10, (Color){28, 29, 32, 250});
    DrawRectangleRoundedLinesEx(panel, 0.08f, 10, 4, WHITE);
    DrawText("RECIBO DA FASE", (int)panel.x + 82, (int)panel.y + 24, 34, GOLD);
    DrawText("Recompensa da fase", (int)panel.x + 34, (int)panel.y + 90, 24, RAYWHITE);
    DrawText(TextFormat("$%d", receipt.base_reward), (int)panel.x + 320, (int)panel.y + 90, 24, GOLD);
    DrawText("Maos sobrando ($1 cada)", (int)panel.x + 34, (int)panel.y + 128, 24, RAYWHITE);
    DrawText(TextFormat("$%d", receipt.hands_reward), (int)panel.x + 320, (int)panel.y + 128, 24, SKYBLUE);
    DrawText("Juros (1 por $5, max 5)", (int)panel.x + 34, (int)panel.y + 166, 24, RAYWHITE);
    DrawText(TextFormat("$%d", receipt.interest_reward), (int)panel.x + 320, (int)panel.y + 166, 24, GREEN);
    DrawLineEx((Vector2){panel.x + 30, panel.y + 206}, (Vector2){panel.x + panel.width - 30, panel.y + 206}, 2, Fade(WHITE, 0.55f));
    DrawText("TOTAL", (int)panel.x + 34, (int)panel.y + 226, 28, WHITE);
    DrawText(TextFormat("$%d", receipt.total_reward), (int)panel.x + 304, (int)panel.y + 226, 30, GOLD);
    DrawText("Clique para continuar", (int)panel.x + 116, (int)panel.y + 264, 18, LIGHTGRAY);
}

int main(void) {
    srand((unsigned int)time(NULL));
    InitWindow(W, H, "Jogo de Cartas PIF - Demo GUI");
    SetTargetFPS(60);

    Card base_cards[MAX_DECK_CARDS];
    Deck deck;
    Card *hand[HAND_SIZE];
    bool selected[HAND_SIZE] = {0};
    int hand_count = 0;
    int score = 0;
    int goal = 420;
    int hands_left = 5;
    int coins = 15;
    bool show_receipt = false;
    float receipt_progress = 0.0f;
    ReceiptBreakdown receipt = {0};
    char feedback[192] = "Selecione ate 5 cartas e clique em Jogar.";

    build_standard_deck(base_cards, NULL);
    deck_from_cards(&deck, base_cards, DECK_SIZE);
    deck_shuffle(&deck);
    while (hand_count < HAND_SIZE && deck_has_cards(&deck)) hand[hand_count++] = deck_draw(&deck);

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (!show_receipt && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < hand_count; i++) {
                Rectangle r = card_rect(i);
                if (CheckCollisionPointRec(mouse, r)) {
                    int selected_count = 0;
                    for (int j = 0; j < hand_count; j++) if (selected[j]) selected_count++;
                    if (selected[i] || selected_count < MAX_PLAY) selected[i] = !selected[i];
                }
            }

            Rectangle play_btn = {70, 650, 180, 52};
            Rectangle reset_btn = {270, 650, 180, 52};
            if (CheckCollisionPointRec(mouse, play_btn) && hands_left > 0 && score < goal) {
                char label[128];
                int gained = score_selected(hand, selected, hand_count, label, sizeof(label));
                score += gained;
                hands_left--;
                snprintf(feedback, sizeof(feedback), "Jogada: %s", label);
                for (int i = 0; i < hand_count; i++) selected[i] = false;

                if (score >= goal) {
                    receipt = build_receipt(coins, hands_left);
                    coins += receipt.total_reward;
                    show_receipt = true;
                    receipt_progress = 0.0f;
                }
            }
            if (CheckCollisionPointRec(mouse, reset_btn)) {
                score = 0;
                hands_left = 5;
                coins = 15;
                hand_count = 0;
                build_standard_deck(base_cards, NULL);
                deck_from_cards(&deck, base_cards, DECK_SIZE);
                deck_shuffle(&deck);
                while (hand_count < HAND_SIZE && deck_has_cards(&deck)) hand[hand_count++] = deck_draw(&deck);
                for (int i = 0; i < HAND_SIZE; i++) selected[i] = false;
                snprintf(feedback, sizeof(feedback), "Nova fase iniciada.");
            }
        } else if (show_receipt && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            show_receipt = false;
            score = 0;
            hands_left = 5;
            hand_count = 0;
            build_standard_deck(base_cards, NULL);
            deck_from_cards(&deck, base_cards, DECK_SIZE);
            deck_shuffle(&deck);
            while (hand_count < HAND_SIZE && deck_has_cards(&deck)) hand[hand_count++] = deck_draw(&deck);
            for (int i = 0; i < HAND_SIZE; i++) selected[i] = false;
            snprintf(feedback, sizeof(feedback), "Nova blind iniciada.");
        }

        if (show_receipt && receipt_progress < 1.0f) {
            receipt_progress += GetFrameTime() * 4.0f;
            if (receipt_progress > 1.0f) receipt_progress = 1.0f;
        }

        BeginDrawing();
        ClearBackground((Color){18, 21, 29, 255});
        DrawRectangleGradientV(0, 0, W, H, (Color){34, 27, 52, 255}, (Color){15, 20, 26, 255});
        DrawCircleGradient(W - 180, 120, 180, Fade(GOLD, 0.24f), Fade(BLACK, 0.0f));
        DrawCircleGradient(170, 90, 140, Fade(SKYBLUE, 0.18f), Fade(BLACK, 0.0f));

        DrawText("Jogo de Cartas - Demo Visual", 70, 36, 34, RAYWHITE);
        DrawText(TextFormat("Pontos: %d / %d", score, goal), 70, 88, 28, score >= goal ? GREEN : SKYBLUE);
        DrawText(TextFormat("Maos restantes: %d", hands_left), 70, 122, 22, LIGHTGRAY);
        DrawText(TextFormat("Moedas: $%d", coins), 70, 148, 22, GOLD);
        DrawText(feedback, 70, 186, 20, Fade(WHITE, 0.9f));
        DrawText("Visual das cartas atualizado: cantos diagonais, naipes coloridos e selos.", 70, 214, 18, Fade(WHITE, 0.7f));

        for (int i = 0; i < hand_count; i++) {
            draw_card_front(hand[i], card_rect(i), selected[i]);
        }

        DrawRectangleRounded((Rectangle){70, 650, 180, 52}, 0.18f, 8, (Color){52, 145, 96, 255});
        DrawText("Jogar", 130, 664, 24, WHITE);
        DrawRectangleRounded((Rectangle){270, 650, 180, 52}, 0.18f, 8, (Color){44, 94, 176, 255});
        DrawText("Reset", 326, 664, 24, WHITE);

        if (score >= goal && !show_receipt) DrawText("Blind vencida!", 500, 658, 28, GOLD);
        if (hands_left <= 0 && score < goal) DrawText("Tentativa encerrada!", 500, 658, 28, RED);

        if (show_receipt) draw_receipt_modal(receipt, receipt_progress);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
