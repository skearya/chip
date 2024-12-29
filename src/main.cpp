#include "raylib-cpp.hpp"

int main() {
    raylib::Window window(800, 450, "raylib-cpp [core] example - basic window");

    window.SetTargetFPS(60);

    while (!window.ShouldClose()) {
        window.BeginDrawing();

        window.ClearBackground(raylib::Color::RayWhite());
        raylib::DrawText("Congrats! You created your first window!", 190, 200, 20, raylib::Color::Black());

        window.EndDrawing();
    }

    return 0;
}
