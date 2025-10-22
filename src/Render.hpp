#include <raylib-cpp.hpp>

#include "Cpu.hpp"

class Render {
public:
  Render(Cpu cpu);

  void run();

private:
  Cpu cpu;

  const raylib::Color bg{0x220000FF};
  const raylib::Color fg{0xAA9999FF};
  const int tick_count = 8;
  bool paused = false;
};