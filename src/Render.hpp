#include "Cpu.hpp"

class Render {
public:
  Render(Cpu cpu);

  void run();

private:
  Cpu cpu;
};