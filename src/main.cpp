#include "raylib.h"
#include "resource_dir.h"

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    constexpr int windowWidth = 1100;
    constexpr int windowHeight = 760;
    InitWindow(windowWidth, windowHeight, "Etch-A-Sketch");
    SetTargetFPS(60);
    SearchAndSetResourceDir("resources");

    Image windowIcon = LoadImage("etch_a_sketch_icon.png");
    ImageFormat(&windowIcon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);  // safe to unload as GPU now handles the image

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(WHITE);
        DrawText("hello, world", 200, 200, 50, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
