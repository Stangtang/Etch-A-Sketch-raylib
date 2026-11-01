#include <cmath>

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
    if (IsKeyDown(key_right_1) || IsKeyDown(key_right_2)) {
        stylusPos.x += dPos;
        constexpr unsigned int maxPosX = canvasWidth - stylusRadius;
        if (stylusPos.x > maxPosX) {
            stylusPos.x = maxPosX;
        }
    }
    if (IsKeyDown(key_left_1) || IsKeyDown(key_left_2)) {
        stylusPos.x -= dPos;
        if (stylusPos.x < 0 + stylusRadius) {
            stylusPos.x = 0 + stylusRadius;
        }
    }
    if (IsKeyDown(key_down_1) || IsKeyDown(key_down_2)) {
        stylusPos.y += dPos;
        constexpr unsigned int maxPosY = canvasHeight - stylusRadius;
        if (stylusPos.y > maxPosY) {
            stylusPos.y = maxPosY;
        }
    }
    if (IsKeyDown(key_up_1) || IsKeyDown(key_up_2)) {
        stylusPos.y -= dPos;
        if (stylusPos.y < 0 + stylusRadius) {
            stylusPos.y = 0 + stylusRadius;
        }
    }
}

bool HasPositionChanged(const Vector2& prevPos, const Vector2& currPos) {
    return (prevPos.x != currPos.x) || (prevPos.y != currPos.y);
}

void ClearCanvas(const RenderTexture2D& canvas) {
    BeginTextureMode(canvas);
    ClearBackground(canvasColor);
    EndTextureMode();
}

void DrawCanvas(const RenderTexture2D& canvas, const Vector2& shakeOffsets, const float& erasureProgress) {
    DrawTextureRec(canvas.texture, {0, 0, static_cast<float>(canvasWidth), -static_cast<float>(canvasHeight)},
                   {canvasX + shakeOffsets.x, canvasY + shakeOffsets.y}, WHITE);
    DrawRectangle(canvasX + shakeOffsets.x, canvasY + shakeOffsets.y, canvasWidth, canvasHeight,
                  Fade(canvasColor, erasureProgress));
}

void DrawStylus(const Vector2& stylusPos, const Vector2& shakeOffsets) {
    DrawCircle(canvasX + stylusPos.x + shakeOffsets.x, canvasY + stylusPos.y + shakeOffsets.y, stylusRadius,
               stylusColor);
}

void DrawKnob(const Vector2& knobCenter, const float& rotation, const Vector2& shakeOffsets) {
    const float offsetKnobX = knobCenter.x + shakeOffsets.x;
    const float offsetKnobY = knobCenter.y + shakeOffsets.y;

    DrawCircle(offsetKnobX, offsetKnobY, knobOuterRadius, knobOuterColor);
    DrawCircle(offsetKnobX, offsetKnobY, knobInnerRadius, knobInnerColor);

    Vector2 rotationMarkerEnd{offsetKnobX + knobRotationMarkerDrawnLength * std::cos(rotation),
                              offsetKnobY + knobRotationMarkerDrawnLength * std::sin(rotation)};
    DrawLineEx({offsetKnobX, offsetKnobY}, rotationMarkerEnd, knobRotationMarkerThickness, knobRotationMarkerColor);

    DrawCircle(offsetKnobX, offsetKnobY, knobRotationMarkerCoveredLength, knobInnerColor);

    DrawCircleV(rotationMarkerEnd, knobRotationMarkerThickness / 2, knobRotationMarkerColor);
    Vector2 rotationMarkerStart{offsetKnobX + knobRotationMarkerCoveredLength * std::cos(rotation),
                                offsetKnobY + knobRotationMarkerCoveredLength * std::sin(rotation)};
    DrawCircleV(rotationMarkerStart, knobRotationMarkerThickness / 2, knobRotationMarkerColor);
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
    ClearCanvas(canvas);

    Vector2 knobRotations{0.0f, 0.0f};

    float erasureShakeTimer = 0.0f;
    float erasureProgress = 0.0f;
    Vector2 shakeOffsets{0.0f, 0.0f};

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();

        if (erasureShakeTimer <= 0.0f) {
            const Vector2 prevStylusPos = stylusPos;
            UpdateStylusPosition(stylusPos, dt);
            if (HasPositionChanged(prevStylusPos, stylusPos)) {
                BeginTextureMode(canvas);
                DrawLineEx(prevStylusPos, stylusPos, 2 * stylusRadius, lineColor);
                DrawCircleV(prevStylusPos, stylusRadius, lineColor);
                EndTextureMode();

                knobRotations.x += (stylusPos.x - prevStylusPos.x) * knobRotationSpeed;
                knobRotations.y += (stylusPos.y - prevStylusPos.y) * knobRotationSpeed;
            }
        }

        if (IsKeyPressed(key_clear) && erasureShakeTimer <= 0.0f) {
            erasureShakeTimer = erasureShakeDuration;
        }
        if (erasureShakeTimer > 0.0f) {
            erasureShakeTimer -= dt;
            if (erasureShakeTimer <= 0.0f) {
                erasureShakeTimer = 0.0f;
                erasureProgress = 0.0f;
                shakeOffsets = {0.0f, 0.0f};
                ClearCanvas(canvas);
            }
        }

        if (erasureShakeTimer > 0.0f) {
            erasureProgress = 1.0f - erasureShakeTimer / erasureShakeDuration;
            const float angle = erasureProgress * 2.0f * PI * erasureShakeCycles;
            shakeOffsets.x = std::cos(angle) * initialMaxShakeDisplacement * (1.0f - erasureProgress);
            shakeOffsets.y = std::sin(angle) * initialMaxShakeDisplacement * (1.0f - erasureProgress);
        }

        BeginDrawing();

        ClearBackground(backgroundColor);

        DrawCanvas(canvas, shakeOffsets, erasureProgress);

        DrawStylus(stylusPos, shakeOffsets);

        DrawKnob(leftKnobCenter, knobRotations.x, shakeOffsets);
        DrawKnob(rightKnobCenter, knobRotations.y, shakeOffsets);

        EndDrawing();
    }

    UnloadRenderTexture(canvas);

    CloseWindow();
    return 0;
}
