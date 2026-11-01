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

void DrawCanvas(const RenderTexture2D& canvas, const float& shakeOffset) {
    DrawTextureRec(canvas.texture, {0, 0, static_cast<float>(canvasWidth), -static_cast<float>(canvasHeight)},
                   {static_cast<float>(canvasX) + shakeOffset, static_cast<float>(canvasY) + shakeOffset}, WHITE);
}

void DrawStylus(const Vector2& stylusPos, const float& shakeOffset) {
    DrawCircle(canvasX + stylusPos.x + shakeOffset, canvasY + stylusPos.y + shakeOffset, stylusRadius, stylusColor);
}

void DrawKnob(const Vector2& knobCenter, const float& rotation, const float& shakeOffset) {
    // DrawCircleV(knobCenter, knobOuterRadius, knobOuterColor);
    DrawCircle(knobCenter.x + shakeOffset, knobCenter.y + shakeOffset, knobOuterRadius, knobOuterColor);
    // DrawCircleV(knobCenter, knobInnerRadius, knobInnerColor);
    DrawCircle(knobCenter.x + shakeOffset, knobCenter.y + shakeOffset, knobInnerRadius, knobInnerColor);

    Vector2 rotationMarkerEnd{knobCenter.x + knobRotationMarkerDrawnLength * std::cos(rotation) + shakeOffset,
                              knobCenter.y + knobRotationMarkerDrawnLength * std::sin(rotation) + shakeOffset};
    DrawLineEx({knobCenter.x + shakeOffset, knobCenter.y + shakeOffset}, rotationMarkerEnd, knobRotationMarkerThickness,
               knobRotationMarkerColor);

    // DrawCircleV(knobCenter, knobRotationMarkerCoveredLength, knobInnerColor);
    DrawCircle(knobCenter.x + shakeOffset, knobCenter.y + shakeOffset, knobRotationMarkerCoveredLength, knobInnerColor);

    DrawCircleV(rotationMarkerEnd, knobRotationMarkerThickness / 2, knobRotationMarkerColor);
    Vector2 rotationMarkerStart{knobCenter.x + knobRotationMarkerCoveredLength * std::cos(rotation) + shakeOffset,
                                knobCenter.y + knobRotationMarkerCoveredLength * std::sin(rotation) + shakeOffset};
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
    float erasureShakeTimer = 0.0f;

    KnobRotations knobRotations{0.0f, 0.0f};

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

                knobRotations.xRotation += (stylusPos.x - prevStylusPos.x) * knobRotationSpeed;
                knobRotations.yRotation += (stylusPos.y - prevStylusPos.y) * knobRotationSpeed;
            }
        }

        if (IsKeyPressed(key_clear) && erasureShakeTimer <= 0.0f) {
            erasureShakeTimer = erasureShakeDuration;
        }
        if (erasureShakeTimer > 0.0f) {
            erasureShakeTimer -= dt;
            if (erasureShakeTimer <= 0.0f) {
                erasureShakeTimer = 0.0f;
                ClearCanvas(canvas);
            }
        }

        float shakeOffset = 0.0f;
        if (erasureShakeTimer > 0.0f) {
            const float erasureProgress = 1.0f - erasureShakeTimer / erasureShakeDuration;
            shakeOffset = std::sin(erasureProgress * 2.0f * PI * erasureShakeCycles) * initialMaxShakeDisplacement *
                          (1.0f - erasureProgress);
        }

        BeginDrawing();

        ClearBackground(backgroundColor);

        DrawCanvas(canvas, shakeOffset);

        DrawStylus(stylusPos, shakeOffset);

        DrawKnob(leftKnobCenter, knobRotations.xRotation, shakeOffset);
        DrawKnob(rightKnobCenter, knobRotations.yRotation, shakeOffset);

        EndDrawing();
    }

    UnloadRenderTexture(canvas);

    CloseWindow();
    return 0;
}
