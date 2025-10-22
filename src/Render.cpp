#include <raylib-cpp.hpp>

#include "Cpu.hpp"
#include "Render.hpp"

Render::Render(Cpu cpu) : cpu{cpu} {}

void Render::run() {
  constexpr int window_width = 800;
  constexpr int window_height = 400;

  constexpr int rows = 32;
  constexpr int cols = 64;

  constexpr float pixel_width =
      static_cast<float>(window_width) / static_cast<float>(cols);
  constexpr float pixel_height =
      static_cast<float>(window_height) / static_cast<float>(rows);

  raylib::Color bg{0x111216FF};
  raylib::Color fg{0xf58ee0FF};

  raylib::Window window{window_width, window_height, "chip"};

  window.SetTargetFPS(60);

  while (!window.ShouldClose()) {
    cpu.keys[0x1] = raylib::Keyboard::IsKeyDown(KEY_ONE);
    cpu.keys[0x2] = raylib::Keyboard::IsKeyDown(KEY_TWO);
    cpu.keys[0x3] = raylib::Keyboard::IsKeyDown(KEY_THREE);
    cpu.keys[0xC] = raylib::Keyboard::IsKeyDown(KEY_FOUR);
    cpu.keys[0x4] = raylib::Keyboard::IsKeyDown(KEY_Q);
    cpu.keys[0x5] = raylib::Keyboard::IsKeyDown(KEY_W);
    cpu.keys[0x6] = raylib::Keyboard::IsKeyDown(KEY_E);
    cpu.keys[0xD] = raylib::Keyboard::IsKeyDown(KEY_R);
    cpu.keys[0x7] = raylib::Keyboard::IsKeyDown(KEY_A);
    cpu.keys[0x8] = raylib::Keyboard::IsKeyDown(KEY_S);
    cpu.keys[0x9] = raylib::Keyboard::IsKeyDown(KEY_D);
    cpu.keys[0xE] = raylib::Keyboard::IsKeyDown(KEY_F);
    cpu.keys[0xA] = raylib::Keyboard::IsKeyDown(KEY_Z);
    cpu.keys[0x0] = raylib::Keyboard::IsKeyDown(KEY_X);
    cpu.keys[0xB] = raylib::Keyboard::IsKeyDown(KEY_C);
    cpu.keys[0xF] = raylib::Keyboard::IsKeyDown(KEY_V);

    for (int i = 0; i < 8; ++i) {
      cpu.tick();
      cpu.tick_timers();
    }

    window.BeginDrawing();

    window.ClearBackground(bg);

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (cpu.pixels[i][j]) {
          raylib::Rectangle{static_cast<float>(j) * pixel_width,
                            static_cast<float>(i) * pixel_height, pixel_width,
                            pixel_height}
              .Draw(fg);
        }
      }
    }

    window.EndDrawing();
  }
}