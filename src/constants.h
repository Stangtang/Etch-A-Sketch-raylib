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
constexpr float stylusRadius = 2.0f;
constexpr Color stylusColor = BLACK;