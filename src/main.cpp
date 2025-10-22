#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "Cpu.hpp"
#include "Render.hpp"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    throw std::runtime_error{"invalid argument count"};
  }

  std::filesystem::path name{argv[1]};

  auto length = std::filesystem::file_size(name);
  if (length == 0) {
    throw std::runtime_error("error reading file");
  }

  std::vector<uint8_t> buffer(length);
  std::ifstream file{name, std::ios_base::binary};
  file.read(reinterpret_cast<char *>(buffer.data()), length);
  file.close();

  Render renderer{Cpu{std::span{buffer}}};
  renderer.run();

  return 0;
}
