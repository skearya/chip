#include "raylib-cpp.hpp"
#include <print>
#include <functional>
#include <iostream>

#include "Cpu.hpp"

int main() {
    // Cpu cpu;

    constexpr int window_width = 800;
    constexpr int window_height = 400;

    raylib::Window window(window_width, window_height, "raylib-cpp [core] example - basic window");

    window.SetTargetFPS(60);

    while (!window.ShouldClose()) {
        window.BeginDrawing();

        window.ClearBackground(raylib::Color::Black());
        raylib::DrawText("Congrats! You created your first window!", 800, 400, 20, raylib::Color::Black());

        constexpr int rows = 32;
        constexpr int cols = 64;

        constexpr bool pixels[rows][cols] = {true};

        constexpr float pixel_width = static_cast<float>(window_width) / static_cast<float>(cols);
        constexpr float pixel_height = static_cast<float>(window_height) / static_cast<float>(rows);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (pixels[i][j]) {
                    raylib::Rectangle(static_cast<float>(j) * pixel_width,
                                      static_cast<float>(i) * pixel_height,
                                      pixel_width, pixel_height).Draw(
                        pixels[i][j] ? raylib::Color::White() : raylib::Color::Black());
                }
            }
        }

        window.EndDrawing();
    }

    return 0;
}
