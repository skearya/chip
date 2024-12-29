#pragma once

#include <array>
#include <stack>

class cpu {
    static void handle_opcode(short opcode);

private:
    std::array<char, 4096> memory = {};
    std::stack<short, std::array<short, 12> > stack = {};
    std::array<char, 16> registers = {};
    short address_register = 0;
    short index_register = 0;
};
