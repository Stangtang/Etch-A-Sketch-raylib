#pragma once

#include <algorithm>
#include <string>

#include "raylib.h"

// math
constexpr float _2PI = 2 * PI;

// window
constexpr unsigned int windowWidth = 1200;
constexpr unsigned int windowHeight = 850;
constexpr Color backgroundColor = WHITE;

// canvas
constexpr unsigned int canvasTopPadding = 70;
constexpr unsigned int canvasBottomPadding = 125;
constexpr unsigned int canvasLeftRightPadding = 100;
constexpr unsigned int canvasX = canvasLeftRightPadding;
constexpr unsigned int canvasY = canvasTopPadding;
constexpr unsigned int canvasWidth = windowWidth - 2 * canvasLeftRightPadding;
constexpr unsigned int canvasHeight = windowHeight - canvasTopPadding - canvasBottomPadding;
constexpr Color canvasColor = LIGHTGRAY;

// erasure shake
constexpr float erasureShakeDuration = 1.0f;
constexpr float erasureShakeCycles = 3.0f;
constexpr float initialMaxShakeDisplacement = 10.0f;

// stylus
constexpr unsigned int stylusStartingX = canvasWidth / 2.0f;
constexpr unsigned int stylusStartingY = canvasHeight / 2.0f;
constexpr float stylusRadius = 2.0f;
constexpr float stylusMovementSpeed = 180.0f;
constexpr Color stylusColor = DARKGRAY;
constexpr Color lineColor = BLACK;

// knob
constexpr float horizontalKnobStartingRotation = -PI / 2.0f;
constexpr float verticalKnobStartingRotation = -PI / 2.0f;
constexpr unsigned int knobBottomPadding = (windowHeight - canvasHeight - canvasY) / 2;
constexpr unsigned int knobLeftRightPadding = 265;
constexpr unsigned int knobY = windowHeight - knobBottomPadding;
const Vector2 leftKnobCenter{knobLeftRightPadding, knobY};
const Vector2 rightKnobCenter{windowWidth - knobLeftRightPadding, knobY};
constexpr float knobOuterRadius = 50.0f;
constexpr float knobInnerRadius = 0.85f * knobOuterRadius;
constexpr float knobRotationSpeed = 0.035f;
constexpr float knobRotationMarkerDrawnLength = knobInnerRadius * 0.75f;
constexpr float knobRotationMarkerCoveredLength = knobRotationMarkerDrawnLength * 0.30f;
static_assert(knobRotationMarkerDrawnLength > knobRotationMarkerCoveredLength, "Rotation knob must be visible!");
constexpr float knobRotationMarkerThickness = 5.0f;
constexpr Color knobInnerColor{246, 239, 213, 255};
const Color knobOuterColor = ColorBrightness(knobInnerColor, -0.25f);
constexpr Color knobRotationMarkerColor = DARKGRAY;

// control
constexpr int key_right_1 = KeyboardKey::KEY_RIGHT;
constexpr int key_right_2 = KeyboardKey::KEY_D;
constexpr int key_left_1 = KeyboardKey::KEY_LEFT;
constexpr int key_left_2 = KeyboardKey::KEY_A;
constexpr int key_down_1 = KeyboardKey::KEY_DOWN;
constexpr int key_down_2 = KeyboardKey::KEY_S;
constexpr int key_up_1 = KeyboardKey::KEY_UP;
constexpr int key_up_2 = KeyboardKey::KEY_W;
constexpr int key_clear = KeyboardKey::KEY_SPACE;
constexpr int key_save_withControlDown = KeyboardKey::KEY_S;
constexpr int key_help = KeyboardKey::KEY_H;

// screenshot
const std::string screenshotFolder = std::string(GetApplicationDirectory()) + "screenshots";
constexpr std::string screenshotName = "screenshot";
constexpr float saveNoticeDuration = 1.5f;
constexpr unsigned int saveNoticeBoxWidth = 200;
constexpr unsigned int saveNoticeBoxHeight = 75;
constexpr unsigned int saveNoticeBoxX = canvasX + (canvasWidth - saveNoticeBoxWidth) / 2;
constexpr unsigned int saveNoticeBoxY = canvasY + (canvasHeight - saveNoticeBoxHeight) / 2;
constexpr float saveNoticeBoxRoundness = 0.10f;
constexpr unsigned int saveNoticeBoxCornerSegments = 5;
const std::string saveMessage = "Screenshot Saved!";
constexpr unsigned int saveMessageFontSize = 17;
const Color saveNoticeBoxColor = ColorFromHSV(120.0f, 0.2f, 0.9f);
constexpr Color saveMessageColor = LIME;

// help
constexpr unsigned int helpMenuWidth = 190;
constexpr unsigned int helpMenuHeight = 70;
constexpr unsigned int helpMenuRightPadding = 100;
constexpr unsigned int helpMenuX = windowWidth - helpMenuRightPadding - helpMenuWidth;
constexpr unsigned int helpMenuY = canvasY;
constexpr float helpMenuRoundness = 0.10f;
constexpr unsigned int helpMenuCornerSegments = 3;
const std::string helpMenuText = "Arrows / WASD: move stylus\nSpace: shake to erase\nCtrl+S: save\nH: toggle help";
constexpr unsigned int helpMenuFontSize = 10;
const unsigned int helpMenuTextLines = std::count(helpMenuText.begin(), helpMenuText.end(), '\n') + 1;
constexpr Color helpMenuColor{246, 239, 213, 255};
constexpr Color helpMenuTextColor{72, 60, 53, 255};