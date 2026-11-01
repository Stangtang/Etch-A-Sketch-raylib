#include "constants.h"
#include "raylib.h"
#include "resource_dir.h"

void SetWindowIcon() {
    Image windowIcon = LoadImage("etch_a_sketch_icon.png");
    constexpr Rectangle cropArea = {49.0f, 40.0f, 373.0f, 273.0f};
    ImageCrop(&windowIcon, cropArea);
    ImageFormat(&windowIcon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);  // safe to unload as GPU now handles the image
}

void UpdateStylusPosition(Vector2& stylusPos, const float& dt) {
    const int dPos = stylusMovementSpeed * dt;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        stylusPos.x += dPos;
        constexpr unsigned int maxPosX = canvasWidth - stylusRadius;
        if (stylusPos.x > maxPosX) {
            stylusPos.x = maxPosX;
        }
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        stylusPos.x -= dPos;
        if (stylusPos.x < 0 + stylusRadius) {
            stylusPos.x = 0 + stylusRadius;
        }
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        stylusPos.y += dPos;
        constexpr unsigned int maxPosY = canvasHeight - stylusRadius;
        if (stylusPos.y > maxPosY) {
            stylusPos.y = maxPosY;
        }
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        stylusPos.y -= dPos;
        if (stylusPos.y < 0 + stylusRadius) {
            stylusPos.y = 0 + stylusRadius;
        }
    }
}

bool HasPositionChanged(Vector2 prevPos, Vector2 currPos) {
    return (prevPos.x != currPos.x) || (prevPos.y != currPos.y);
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(windowWidth, windowHeight, "Etch-A-Sketch");
    SetTargetFPS(60);
    SearchAndSetResourceDir("resources");

    SetWindowIcon();

    Vector2 stylusPos{// stylus position relative to top-left corner of canvas
                      stylusStartingX, stylusStartingY};

    RenderTexture2D canvas = LoadRenderTexture(canvasWidth, canvasHeight);
    BeginTextureMode(canvas);
    ClearBackground(canvasColor);
    EndTextureMode();

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();

        const Vector2 prevStylusPos = stylusPos;
        UpdateStylusPosition(stylusPos, dt);

        if (HasPositionChanged(prevStylusPos, stylusPos)) {
            BeginTextureMode(canvas);
            DrawLineEx(prevStylusPos, stylusPos, stylusRadius, stylusColor);
            EndTextureMode();
        }

        BeginDrawing();

        ClearBackground(backgroundColor);

        // draw canvas
        DrawTextureRec(canvas.texture, {0, 0, static_cast<float>(canvasWidth), -static_cast<float>(canvasHeight)},
                       {static_cast<float>(canvasX), static_cast<float>(canvasY)}, WHITE);

        // draw stylus
        DrawCircle(canvasX + stylusPos.x, canvasY + stylusPos.y, stylusRadius, stylusColor);

        EndDrawing();
    }

    UnloadRenderTexture(canvas);

    CloseWindow();
    return 0;
}
