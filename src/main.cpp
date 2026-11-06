#include <chrono>
#include <cmath>
#include <exception>
#include <future>

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

void UpdateStylusPosition(Vector2& stylusPos, const float& dt, const bool& isControlKeyDown) {
    const float dPos = stylusMovementSpeed * dt;
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
    if (!isControlKeyDown && (IsKeyDown(key_down_1) || IsKeyDown(key_down_2))) {
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
    DrawCircle(canvasX + stylusPos.x + shakeOffsets.x, canvasY + stylusPos.y + shakeOffsets.y, stylusRadius, stylusColor);
}

void DrawKnob(const Vector2& knobCenter, const float& rotation, const Vector2& shakeOffsets) {
    const float offsetKnobX = knobCenter.x + shakeOffsets.x;
    const float offsetKnobY = knobCenter.y + shakeOffsets.y;

    DrawCircle(offsetKnobX, offsetKnobY, knobOuterRadius, knobOuterColor);
    DrawCircle(offsetKnobX, offsetKnobY, knobInnerRadius, knobInnerColor);

    Vector2 rotationMarkerEndPos{offsetKnobX + knobRotationMarkerDrawnLength * std::cos(rotation),
                                 offsetKnobY + knobRotationMarkerDrawnLength * std::sin(rotation)};
    DrawLineEx({offsetKnobX, offsetKnobY}, rotationMarkerEndPos, knobRotationMarkerThickness, knobRotationMarkerColor);

    DrawCircle(offsetKnobX, offsetKnobY, knobRotationMarkerCoveredLength, knobInnerColor);

    DrawCircleV(rotationMarkerEndPos, knobRotationMarkerThickness / 2, knobRotationMarkerColor);
    Vector2 rotationMarkerStartPos{offsetKnobX + knobRotationMarkerCoveredLength * std::cos(rotation),
                                   offsetKnobY + knobRotationMarkerCoveredLength * std::sin(rotation)};
    DrawCircleV(rotationMarkerStartPos, knobRotationMarkerThickness / 2, knobRotationMarkerColor);
}

void DrawKnobs(const Vector2& knobRotations, const Vector2& shakeOffsets) {
    DrawKnob(leftKnobCenter, knobRotations.x, shakeOffsets);
    DrawKnob(rightKnobCenter, knobRotations.y, shakeOffsets);
}

void DrawSaveNotice(const unsigned int& saveMessageX, const unsigned int& saveMessageY, const float& saveNoticeProgress) {
    const float easedColorAlpha = 1.0f - std::pow(saveNoticeProgress, 3);
    DrawRectangleRounded({saveNoticeBoxX, saveNoticeBoxY, saveNoticeBoxWidth, saveNoticeBoxHeight}, saveNoticeBoxRoundness,
                         saveNoticeBoxCornerSegments, Fade(saveNoticeBoxColor, easedColorAlpha));
    DrawText(saveMessage.c_str(), saveMessageX, saveMessageY, saveMessageFontSize, Fade(saveMessageColor, easedColorAlpha));
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(windowWidth, windowHeight, "Etch-A-Sketch");
    SetTargetFPS(60);
    SearchAndSetResourceDir("resources");

    SetWindowIcon();

    Vector2 stylusPos{stylusStartingX, stylusStartingY};  // stylus position relative to top-left corner of canvas

    RenderTexture2D canvas = LoadRenderTexture(canvasWidth, canvasHeight);
    ClearCanvas(canvas);

    Vector2 knobRotations{horizontalKnobStartingRotation, verticalKnobStartingRotation};

    float erasureShakeTimer = 0.0f;
    float erasureProgress = 0.0f;
    Vector2 shakeOffsets{0.0f, 0.0f};

    if (!DirectoryExists(screenshotFolder.c_str())) {
        MakeDirectory(screenshotFolder.c_str());
    }
    unsigned int screenshotNumber = 1;
    std::future<bool> screenshotSave;
    float saveNoticeTimer = 0.0f;
    float saveNoticeProgress = 0.0f;
    // calculate save message constants after window is loaded
    const unsigned int saveMessageWidth = MeasureText(saveMessage.c_str(), saveMessageFontSize);
    const unsigned int saveMessageX = canvasX + canvasWidth / 2 - saveMessageWidth / 2;
    constexpr unsigned int saveMessageY = canvasY + canvasHeight / 2 - saveMessageFontSize / 2;

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();
        const bool isControlKeyDown = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);

        if (screenshotSave.valid() && screenshotSave.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            if (screenshotSave.get()) {
                saveNoticeTimer = saveNoticeDuration;
            } else {
                TraceLog(LOG_WARNING, "Screenshot could not be saved.");
            }
        }

        if (erasureShakeTimer <= 0.0f) {
            const Vector2 prevStylusPos = stylusPos;
            UpdateStylusPosition(stylusPos, dt, isControlKeyDown);
            if (HasPositionChanged(prevStylusPos, stylusPos)) {
                BeginTextureMode(canvas);
                DrawLineEx(prevStylusPos, stylusPos, 2 * stylusRadius, lineColor);
                DrawCircleV(prevStylusPos, stylusRadius, lineColor);
                EndTextureMode();

                knobRotations.x += (stylusPos.x - prevStylusPos.x) * knobRotationSpeed;
                knobRotations.y += (stylusPos.y - prevStylusPos.y) * knobRotationSpeed;
                knobRotations.x = std::fmod(knobRotations.x, _2PI);
                knobRotations.y = std::fmod(knobRotations.y, _2PI);
            }
        }

        if (IsKeyPressed(key_clear) && erasureShakeTimer <= 0.0f) {
            erasureShakeTimer = erasureShakeDuration;
        }
        if (erasureShakeTimer > 0.0f) {
            erasureProgress = 1.0f - erasureShakeTimer / erasureShakeDuration;
            const float angle = erasureProgress * _2PI * erasureShakeCycles;
            shakeOffsets.x = std::cos(angle) * initialMaxShakeDisplacement * (1.0f - erasureProgress);
            shakeOffsets.y = std::sin(angle) * initialMaxShakeDisplacement * (1.0f - erasureProgress);

            erasureShakeTimer -= dt;
            if (erasureShakeTimer <= 0.0f) {
                erasureShakeTimer = 0.0f;
                erasureProgress = 0.0f;
                shakeOffsets = {0.0f, 0.0f};
                ClearCanvas(canvas);
            }
        }

        if (isControlKeyDown && IsKeyPressed(key_save_withControlDown) && !screenshotSave.valid()) {
            const std::string screenshotPath =
                screenshotFolder + '/' + screenshotName + std::to_string(screenshotNumber++) + ".png";
            Image screenshot = LoadImageFromScreen();
            if (screenshot.data == nullptr) {
                TraceLog(LOG_WARNING, "Screenshot could not be captured.");
            } else {
                try {
                    screenshotSave = std::async(std::launch::async, [screenshot, screenshotPath]() {
                        const bool saved = ExportImage(screenshot, screenshotPath.c_str());
                        UnloadImage(screenshot);
                        return saved;
                    });
                } catch (const std::exception& error) {
                    UnloadImage(screenshot);
                    TraceLog(LOG_WARNING, "Could not start screenshot save: %s", error.what());
                }
            }
        }
        if (saveNoticeTimer > 0.0f) {
            saveNoticeProgress = 1.0f - saveNoticeTimer / saveNoticeDuration;

            saveNoticeTimer -= dt;
            if (saveNoticeTimer <= 0.0f) {
                saveNoticeTimer = 0.0f;
                saveNoticeProgress = 0.0f;
            }
        }

        BeginDrawing();

        ClearBackground(backgroundColor);

        DrawCanvas(canvas, shakeOffsets, erasureProgress);

        DrawStylus(stylusPos, shakeOffsets);

        DrawKnobs(knobRotations, shakeOffsets);

        if (saveNoticeTimer > 0.0f) {
            DrawSaveNotice(saveMessageX, saveMessageY, saveNoticeProgress);
        }

        EndDrawing();
    }

    // finish last write before exiting
    if (screenshotSave.valid() && !screenshotSave.get()) {
        TraceLog(LOG_WARNING, "Screenshot could not be saved.");
    }

    UnloadRenderTexture(canvas);

    CloseWindow();

    return 0;
}
