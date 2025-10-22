#include <print>
#include <raylib-cpp.hpp>

#include "Cpu.hpp"
#include "Keyboard.hpp"
#include "Render.hpp"
#include "raylib.h"

const char *frag = R"(
// Modified version of https://www.shadertoy.com/view/DlfSz8

#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

#define CURVATURE 5.1
#define BLUR .012
#define CA_AMT 1.008

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);

    vec2 crtUV = fragTexCoord * 2. - 1.;
    vec2 offset = crtUV.yx / CURVATURE;
    crtUV += crtUV * offset * offset;
    crtUV = crtUV * .5 + .5;
    
    vec2 edge = smoothstep(0., BLUR, crtUV) * (1. - smoothstep(1. - BLUR, 1., crtUV));

    texelColor.rgb = vec3(
        texture(texture0, (crtUV - .5) * CA_AMT + .5).r,
        texture(texture0, crtUV).g,
        texture(texture0, (crtUV - .5) / CA_AMT + .5).b
    ) * edge.x * edge.y;

    float scan = .75 * 0.5 * abs(sin(fragTexCoord.y));
    texelColor.rgb = mix(texelColor.rgb, vec3(0.0), scan);

    // final color is the color from the texture 
    //    times the tint color (colDiffuse)
    //    times the fragment color (interpolated vertex color)
    finalColor = vec4(texelColor.rgb, 1.0) * colDiffuse * fragColor;
}
)";

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

  raylib::Window window{window_width, window_height, "chip"};

  raylib::Shader shader = raylib::ShaderUnmanaged::LoadFromMemory(0, frag);

  raylib::RenderTexture2D target{window_width, window_height};

  window.SetTargetFPS(60);

  while (!window.ShouldClose()) {
    if (raylib::Keyboard::IsKeyPressed(KEY_P)) {
      paused = !paused;
    }

    if (!paused) {
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

      for (int i = 0; i < tick_count; ++i) {
        cpu.tick();
        cpu.tick_timers();
      }
    }

    target.BeginMode();

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

    target.EndMode();

    window.BeginDrawing();

    shader.BeginMode();

    target.GetTexture().Draw(
        raylib::Rectangle{0, 0, (float)target.GetTexture().GetWidth(),
                          (float)-target.GetTexture().GetHeight()},
        Vector2{0, 0}, raylib::Color::White());

    shader.EndMode();

    window.EndDrawing();
  }
}