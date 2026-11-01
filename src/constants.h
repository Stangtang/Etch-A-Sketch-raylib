#pragma once
#include "raylib.h"

// window constants
constexpr unsigned int windowWidth = 1100;
constexpr unsigned int windowHeight = 760;
constexpr Color backgroundColor = WHITE;

// canvas constants
constexpr unsigned int canvasTopBottomPadding = 75;
constexpr unsigned int canvasLeftRightPadding = 100;
constexpr unsigned int canvasX = canvasLeftRightPadding;
constexpr unsigned int canvasY = canvasTopBottomPadding;
constexpr unsigned int canvasWidth = windowWidth - 2 * canvasLeftRightPadding;
constexpr unsigned int canvasHeight = windowHeight - 2 * canvasTopBottomPadding;
constexpr Color canvasColor = LIGHTGRAY;

// stylus constants
constexpr unsigned int stylusStartingX = canvasWidth / 2.0f;
constexpr unsigned int stylusStartingY = canvasHeight / 2.0f;
constexpr float stylusRadius = 10.0f;          // big for testing purposes
constexpr float stylusMovementSpeed = 500.0f;  // fast for testing purposes
constexpr Color stylusColor = BLACK;

// knob constants
const Vector2 leftKnobCenter{185, 650};
const Vector2 rightKnobCenter{915, 650};
constexpr float knobOuterRadius = 50.0f;
constexpr float knobInnerRadius = 0.85f * knobOuterRadius;
constexpr Color knobInnerColor{246, 239, 213, 255};
const Color knobOuterColor = ColorBrightness(knobInnerColor, -0.25f);

// control constants
constexpr int key_right_1 = KeyboardKey::KEY_RIGHT;
constexpr int key_right_2 = KeyboardKey::KEY_D;
constexpr int key_left_1 = KeyboardKey::KEY_LEFT;
constexpr int key_left_2 = KeyboardKey::KEY_A;
constexpr int key_down_1 = KeyboardKey::KEY_DOWN;
constexpr int key_down_2 = KeyboardKey::KEY_S;
constexpr int key_up_1 = KeyboardKey::KEY_UP;
constexpr int key_up_2 = KeyboardKey::KEY_W;
constexpr int key_clear = KeyboardKey::KEY_SPACE;