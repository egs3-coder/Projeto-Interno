#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define Rectangle WinRectangle
#define CloseWindow WinCloseWindow
#define DrawText WinDrawText
#include <windows.h>
#undef Rectangle
#undef CloseWindow
#undef DrawText

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

typedef struct Rectangle {
    float x;
    float y;
    float width;
    float height;
} Rectangle;

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

static HMODULE real_raylib = NULL;
static int config_loaded = 0;
static float config_scale_x = 1.0f;
static float config_scale_y = 1.0f;
static float config_offset_x = 0.0f;
static float config_offset_y = 0.0f;
static float config_virtual_width = 1400.0f;
static float config_virtual_height = 920.0f;
static float config_draw_scale = 0.98f;

static FARPROC real_proc(const char *name) {
    char path[MAX_PATH];

    if (!real_raylib) {
        GetModuleFileNameA(NULL, path, (DWORD)sizeof(path));
        char *slash = strrchr(path, '\\');
        if (slash) slash[1] = '\0';
        strncat(path, "libraylib_real.dll", sizeof(path) - strlen(path) - 1);
        real_raylib = LoadLibraryA(path);
        if (!real_raylib) real_raylib = LoadLibraryA("libraylib_real.dll");
    }

    return real_raylib ? GetProcAddress(real_raylib, name) : NULL;
}

static void app_dir(char *out, size_t out_size) {
    GetModuleFileNameA(NULL, out, (DWORD)out_size);
    char *slash = strrchr(out, '\\');
    if (slash) slash[1] = '\0';
}

static void load_mouse_config(void) {
    char path[MAX_PATH];
    FILE *file;
    char key[64];
    float value;

    if (config_loaded) return;
    config_loaded = 1;

    app_dir(path, sizeof(path));
    strncat(path, "mouse_calibration.ini", sizeof(path) - strlen(path) - 1);
    file = fopen(path, "r");
    if (!file) return;

    while (fscanf(file, " %63[^=]=%f", key, &value) == 2) {
        if (strcmp(key, "scale_x") == 0) config_scale_x = value;
        else if (strcmp(key, "scale_y") == 0) config_scale_y = value;
        else if (strcmp(key, "offset_x") == 0) config_offset_x = value;
        else if (strcmp(key, "offset_y") == 0) config_offset_y = value;
        else if (strcmp(key, "virtual_width") == 0) config_virtual_width = value;
        else if (strcmp(key, "virtual_height") == 0) config_virtual_height = value;
        else if (strcmp(key, "draw_scale") == 0) config_draw_scale = value;
        int ch;
        while ((ch = fgetc(file)) != '\n' && ch != EOF) {}
    }
    fclose(file);
}

static HWND raylib_window(void) {
    typedef void *(*GetWindowHandleFn)(void);
    GetWindowHandleFn get_window_handle = (GetWindowHandleFn)real_proc("GetWindowHandle");
    HWND hwnd = get_window_handle ? (HWND)get_window_handle() : NULL;
    if (!hwnd) hwnd = GetForegroundWindow();
    return hwnd;
}

static float client_width(HWND hwnd) {
    RECT client;
    if (hwnd && GetClientRect(hwnd, &client)) return (float)(client.right - client.left);
    return 0.0f;
}

static float client_height(HWND hwnd) {
    RECT client;
    if (hwnd && GetClientRect(hwnd, &client)) return (float)(client.bottom - client.top);
    return 0.0f;
}

static float draw_scale(HWND hwnd) {
    float cw;
    float ch;
    float scale_x;
    float scale_y;

    load_mouse_config();
    cw = client_width(hwnd);
    ch = client_height(hwnd);
    if (cw <= 0.0f || ch <= 0.0f || config_virtual_width <= 0.0f || config_virtual_height <= 0.0f) {
        return config_draw_scale;
    }

    scale_x = cw / config_virtual_width;
    scale_y = ch / config_virtual_height;
    return (scale_x < scale_y ? scale_x : scale_y) * config_draw_scale;
}

static float draw_offset_x(HWND hwnd) {
    float cw = client_width(hwnd);
    float scale = draw_scale(hwnd);
    float drawn_width = config_virtual_width * scale;
    return cw > drawn_width ? (cw - drawn_width) * 0.5f : 0.0f;
}

static float draw_offset_y(HWND hwnd) {
    float ch = client_height(hwnd);
    float scale = draw_scale(hwnd);
    float drawn_height = config_virtual_height * scale;
    return ch > drawn_height ? (ch - drawn_height) * 0.5f : 0.0f;
}

static int tx_i(float value, HWND hwnd) {
    return (int)(value * draw_scale(hwnd) + draw_offset_x(hwnd) + 0.5f);
}

static int ty_i(float value, HWND hwnd) {
    return (int)(value * draw_scale(hwnd) + draw_offset_y(hwnd) + 0.5f);
}

static float ts_f(float value, HWND hwnd) {
    return value * draw_scale(hwnd);
}

static Rectangle tx_rec(Rectangle rec, HWND hwnd) {
    float scale = draw_scale(hwnd);
    rec.x = rec.x * scale + draw_offset_x(hwnd);
    rec.y = rec.y * scale + draw_offset_y(hwnd);
    rec.width *= scale;
    rec.height *= scale;
    return rec;
}

__declspec(dllexport) void InitWindow(int width, int height, const char *title) {
    typedef void (*Fn)(int, int, const char *);
    typedef void (*SetWindowSizeFn)(int, int);
    Fn fn = (Fn)real_proc("InitWindow");
    SetWindowSizeFn set_window_size = (SetWindowSizeFn)real_proc("SetWindowSize");
    RECT work_area;
    int target_width = width;
    int target_height = height;

    load_mouse_config();
    if (SystemParametersInfoA(SPI_GETWORKAREA, 0, &work_area, 0)) {
        int work_width = work_area.right - work_area.left;
        int work_height = work_area.bottom - work_area.top;
        int max_width = work_width - 80;
        int max_height = work_height - 80;
        float scale_x;
        float scale_y;
        float scale;

        if (max_width < 720) max_width = work_width;
        if (max_height < 480) max_height = work_height;

        scale_x = (float)max_width / (float)width;
        scale_y = (float)max_height / (float)height;
        scale = scale_x < scale_y ? scale_x : scale_y;
        if (scale > 1.0f) scale = 1.0f;

        target_width = (int)(width * scale + 0.5f);
        target_height = (int)(height * scale + 0.5f);
    }

    if (fn) fn(target_width, target_height, title);
    if (set_window_size) set_window_size(target_width, target_height);

    {
        HWND hwnd = raylib_window();
        if (hwnd && SystemParametersInfoA(SPI_GETWORKAREA, 0, &work_area, 0)) {
            int work_width = work_area.right - work_area.left;
            int work_height = work_area.bottom - work_area.top;
            int x = work_area.left + (work_width - target_width) / 2;
            int y = work_area.top + (work_height - target_height) / 2;
            if (y < work_area.top + 8) y = work_area.top + 8;
            SetWindowPos(hwnd, NULL, x, y, target_width, target_height,
                         SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }
}

__declspec(dllexport) bool WindowShouldClose(void) {
    typedef bool (*Fn)(void);
    Fn fn = (Fn)real_proc("WindowShouldClose");
    return fn ? fn() : true;
}

__declspec(dllexport) void CloseWindow(void) {
    typedef void (*Fn)(void);
    Fn fn = (Fn)real_proc("CloseWindow");
    if (fn) fn();
}

__declspec(dllexport) void BeginDrawing(void) {
    typedef void (*Fn)(void);
    Fn fn = (Fn)real_proc("BeginDrawing");
    if (fn) fn();
}

__declspec(dllexport) void EndDrawing(void) {
    typedef void (*Fn)(void);
    Fn fn = (Fn)real_proc("EndDrawing");
    if (fn) fn();
}

__declspec(dllexport) void ClearBackground(Color color) {
    typedef void (*Fn)(Color);
    Fn fn = (Fn)real_proc("ClearBackground");
    if (fn) fn(color);
}

__declspec(dllexport) void SetTargetFPS(int fps) {
    typedef void (*Fn)(int);
    Fn fn = (Fn)real_proc("SetTargetFPS");
    if (fn) fn(fps);
}

__declspec(dllexport) float GetFrameTime(void) {
    typedef float (*Fn)(void);
    Fn fn = (Fn)real_proc("GetFrameTime");
    return fn ? fn() : 0.016f;
}

__declspec(dllexport) double GetTime(void) {
    typedef double (*Fn)(void);
    Fn fn = (Fn)real_proc("GetTime");
    return fn ? fn() : 0.0;
}

__declspec(dllexport) Vector2 GetMousePosition(void) {
    typedef Vector2 (*Fn)(void);
    Fn fn = (Fn)real_proc("GetMousePosition");
    HWND hwnd = raylib_window();
    POINT point;
    Vector2 mouse = {0.0f, 0.0f};

    if (hwnd && GetCursorPos(&point) && ScreenToClient(hwnd, &point)) {
        mouse.x = (float)point.x;
        mouse.y = (float)point.y;
    } else {
        mouse = fn ? fn() : mouse;
    }

    {
        float scale = draw_scale(hwnd);
        float offset_x = draw_offset_x(hwnd);
        float offset_y = draw_offset_y(hwnd);

        if (scale > 0.0f) {
            mouse.x = ((mouse.x - offset_x) / scale) * config_scale_x + config_offset_x;
            mouse.y = ((mouse.y - offset_y) / scale) * config_scale_y + config_offset_y;
        }
    }
    return mouse;
}

__declspec(dllexport) bool IsMouseButtonPressed(int button) {
    typedef bool (*Fn)(int);
    Fn fn = (Fn)real_proc("IsMouseButtonPressed");
    return fn ? fn(button) : false;
}

__declspec(dllexport) bool IsKeyPressed(int key) {
    typedef bool (*Fn)(int);
    Fn fn = (Fn)real_proc("IsKeyPressed");
    return fn ? fn(key) : false;
}

__declspec(dllexport) int GetCharPressed(void) {
    typedef int (*Fn)(void);
    Fn fn = (Fn)real_proc("GetCharPressed");
    return fn ? fn() : 0;
}

__declspec(dllexport) bool CheckCollisionPointRec(Vector2 point, Rectangle rec) {
    return point.x >= rec.x && point.x <= rec.x + rec.width &&
           point.y >= rec.y && point.y <= rec.y + rec.height;
}

__declspec(dllexport) int MeasureText(const char *text, int fontSize) {
    typedef int (*Fn)(const char *, int);
    Fn fn = (Fn)real_proc("MeasureText");
    HWND hwnd = raylib_window();
    float scale = draw_scale(hwnd);
    return fn ? (int)(fn(text, (int)(fontSize * scale + 0.5f)) / (scale > 0.0f ? scale : 1.0f)) : 0;
}

__declspec(dllexport) const char *TextFormat(const char *text, ...) {
    static char buffers[4][1024];
    static int index = 0;
    va_list args;

    index = (index + 1) % 4;
    va_start(args, text);
    vsnprintf(buffers[index], sizeof(buffers[index]), text, args);
    va_end(args);
    return buffers[index];
}

__declspec(dllexport) Color Fade(Color color, float alpha) {
    typedef Color (*Fn)(Color, float);
    Fn fn = (Fn)real_proc("Fade");
    if (fn) return fn(color, alpha);
    color.a = (unsigned char)(255.0f * alpha);
    return color;
}

__declspec(dllexport) void DrawText(const char *text, int posX, int posY, int fontSize, Color color) {
    typedef void (*Fn)(const char *, int, int, int, Color);
    Fn fn = (Fn)real_proc("DrawText");
    HWND hwnd = raylib_window();
    if (fn) fn(text, tx_i((float)posX, hwnd), ty_i((float)posY, hwnd), (int)(ts_f((float)fontSize, hwnd) + 0.5f), color);
}

__declspec(dllexport) void DrawRectangle(int posX, int posY, int width, int height, Color color) {
    typedef void (*Fn)(int, int, int, int, Color);
    Fn fn = (Fn)real_proc("DrawRectangle");
    HWND hwnd = raylib_window();
    if (fn) fn(tx_i((float)posX, hwnd), ty_i((float)posY, hwnd),
               (int)(ts_f((float)width, hwnd) + 0.5f), (int)(ts_f((float)height, hwnd) + 0.5f), color);
}

__declspec(dllexport) void DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom) {
    typedef void (*Fn)(int, int, int, int, Color, Color);
    Fn fn = (Fn)real_proc("DrawRectangleGradientV");
    HWND hwnd = raylib_window();
    if (fn) fn(tx_i((float)posX, hwnd), ty_i((float)posY, hwnd),
               (int)(ts_f((float)width, hwnd) + 0.5f), (int)(ts_f((float)height, hwnd) + 0.5f), top, bottom);
}

__declspec(dllexport) void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color) {
    typedef void (*Fn)(Rectangle, float, int, Color);
    Fn fn = (Fn)real_proc("DrawRectangleRounded");
    HWND hwnd = raylib_window();
    if (fn) fn(tx_rec(rec, hwnd), roundness, segments, color);
}

__declspec(dllexport) void DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color) {
    typedef void (*Fn)(Rectangle, float, int, float, Color);
    Fn fn = (Fn)real_proc("DrawRectangleRoundedLinesEx");
    HWND hwnd = raylib_window();
    if (fn) fn(tx_rec(rec, hwnd), roundness, segments, ts_f(lineThick, hwnd), color);
}

__declspec(dllexport) void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color) {
    typedef void (*Fn)(int, int, int, int, Color);
    Fn fn = (Fn)real_proc("DrawLine");
    HWND hwnd = raylib_window();
    if (fn) fn(tx_i((float)startPosX, hwnd), ty_i((float)startPosY, hwnd),
               tx_i((float)endPosX, hwnd), ty_i((float)endPosY, hwnd), color);
}

__declspec(dllexport) void DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color) {
    typedef void (*Fn)(int, int, float, float, Color);
    Fn fn = (Fn)real_proc("DrawEllipse");
    HWND hwnd = raylib_window();
    if (fn) fn(tx_i((float)centerX, hwnd), ty_i((float)centerY, hwnd),
               ts_f(radiusH, hwnd), ts_f(radiusV, hwnd), color);
}

/* ---- Funcoes adicionadas para compatibilidade com a versao Master ---- */

typedef struct Texture2D {
    unsigned int id;
    int width;
    int height;
    int mipmaps;
    int format;
} Texture2D;

__declspec(dllexport) void DrawCircle(int centerX, int centerY, float radius, Color color) {
    typedef void (*Fn)(int, int, float, Color);
    Fn fn = (Fn)real_proc("DrawCircle");
    HWND hwnd = raylib_window();
    if (fn) fn(tx_i((float)centerX, hwnd), ty_i((float)centerY, hwnd),
               ts_f(radius, hwnd), color);
}

__declspec(dllexport) Texture2D LoadTexture(const char *fileName) {
    typedef Texture2D (*Fn)(const char *);
    Fn fn = (Fn)real_proc("LoadTexture");
    Texture2D t = {0};
    return fn ? fn(fileName) : t;
}

__declspec(dllexport) void UnloadTexture(Texture2D texture) {
    typedef void (*Fn)(Texture2D);
    Fn fn = (Fn)real_proc("UnloadTexture");
    if (fn) fn(texture);
}

__declspec(dllexport) bool FileExists(const char *fileName) {
    typedef bool (*Fn)(const char *);
    Fn fn = (Fn)real_proc("FileExists");
    return fn ? fn(fileName) : false;
}

__declspec(dllexport) void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest,
                                           Vector2 origin, float rotation, Color tint) {
    typedef void (*Fn)(Texture2D, Rectangle, Rectangle, Vector2, float, Color);
    Fn fn = (Fn)real_proc("DrawTexturePro");
    HWND hwnd = raylib_window();
    if (fn) fn(texture, source, tx_rec(dest, hwnd), origin, rotation, tint);
}
