#pragma once

// window constants
constexpr unsigned int windowWidth = 1100;
constexpr unsigned int windowHeight = 760;

// canvas constants
constexpr unsigned int canvasTopBottomPadding = 75;
constexpr unsigned int canvasLeftRightPadding = 100;
constexpr unsigned int canvasX = canvasLeftRightPadding;
constexpr unsigned int canvasY = canvasTopBottomPadding;
constexpr unsigned int canvasWidth = windowWidth - 2 * canvasLeftRightPadding;
constexpr unsigned int canvasHeight = windowHeight - 2 * canvasTopBottomPadding;