#include <cstdint>
#include <print>
#include <ranges>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <format>

#include "Cpu.hpp"

// Isolates a hex digit (4 bits) based on a right-to-left index
// Ex: digit<3>(0xFAAA) -> 0x0F
template<uint8_t pos>
uint8_t digit(const uint16_t opcode) {
    return opcode >> pos * 4 & 0x000F;
}

Cpu::Cpu(std::span<uint8_t> opcodes) {
    std::ranges::copy(FONT_SET, memory.begin());
    std::ranges::copy(opcodes, memory.begin() + START_ADDR);
};

void Cpu::tick() {
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement (All cases handled)
    switch (const uint16_t opcode = fetch_opcode(); opcode & 0xF000) {
        case 0x0000: {
            switch (opcode) {
                case 0x0000: {
                    return;
                }
                case 0x00E0: {
                    for (std::array<bool, 64> &row: pixels) {
                        row.fill(false);
                    }
                    break;
                }
                case 0x00EE: {
                    program_counter = pop();
                    break;
                }
                default: {
                    throw std::runtime_error(std::format("Invalid opcode: 0x{:04X}", opcode));
                }
            }
            break;
        }
        case 0x1000: {
            program_counter = opcode & 0x0FFF;
            break;
        }
        case 0x2000: {
            push(program_counter);
            program_counter = opcode & 0x0FFF;
            break;
        }
        case 0x3000: {
            if (registers[digit<2>(opcode)] == (opcode & 0x00FF)) {
                program_counter += 2;
            }
            break;
        }
        case 0x4000: {
            if (registers[digit<2>(opcode)] != (opcode & 0x00FF)) {
                program_counter += 2;
            }
            break;
        }
        case 0x5000: {
            if (registers[digit<2>(opcode)] == registers[digit<1>(opcode)]) {
                program_counter += 2;
            }
            break;
        }
        case 0x6000:
            registers[digit<2>(opcode)] = opcode & 0x00FF;
            break;
        case 0x7000: {
            registers[digit<2>(opcode)] += opcode & 0x00FF;
            break;
        }
        case 0x8000: {
            switch (opcode & 0x800F) {
                case 0x8000: {
                    registers[digit<2>(opcode)] = registers[digit<1>(opcode)];
                    break;
                }
                case 0x8001: {
                    registers[digit<2>(opcode)] |= registers[digit<1>(opcode)];
                    registers[0xF] = 0;
                    break;
                }
                case 0x8002: {
                    registers[digit<2>(opcode)] &= registers[digit<1>(opcode)];
                    registers[0xF] = 0;
                    break;
                }
                case 0x8003: {
                    registers[digit<2>(opcode)] ^= registers[digit<1>(opcode)];
                    registers[0xF] = 0;
                    break;
                }
                case 0x8004: {
                    const uint16_t result = registers[digit<2>(opcode)] + registers[digit<1>(opcode)];
                    registers[digit<2>(opcode)] = result;
                    registers[0xF] = result > 0xFF;
                    break;
                }
                case 0x8005: {
                    const bool safe = registers[digit<2>(opcode)] >= registers[digit<1>(opcode)];
                    registers[digit<2>(opcode)] -= registers[digit<1>(opcode)];
                    registers[0xF] = safe;
                    break;
                }
                case 0x8006: {
                    // TODO: Make configurable
                    registers[digit<2>(opcode)] = registers[digit<1>(opcode)];

                    const uint8_t lsb = registers[digit<2>(opcode)] & 0x01;
                    registers[digit<2>(opcode)] >>= 1;
                    registers[0xF] = lsb;
                    break;
                }
                case 0x8007: {
                    const bool safe = registers[digit<1>(opcode)] >= registers[digit<2>(opcode)];
                    registers[digit<2>(opcode)] = registers[digit<1>(opcode)] - registers[digit<2>(opcode)];
                    registers[0xF] = safe;
                    break;
                }
                case 0x800E: {
                    // TODO: Make configurable
                    registers[digit<2>(opcode)] = registers[digit<1>(opcode)];

                    const uint8_t msb = (registers[digit<2>(opcode)] & 0x80) >> 7;
                    registers[digit<2>(opcode)] <<= 1;
                    registers[0xF] = msb;
                    break;
                }
                default: {
                    throw std::runtime_error(std::format("Invalid opcode: 0x{:04X}", opcode));
                }
            }
            break;
        }
        case 0x9000: {
            if (registers[digit<2>(opcode)] != registers[digit<1>(opcode)]) {
                program_counter += 2;
            }
            break;
        }
        case 0xA000: {
            i_register = opcode & 0x0FFF;
            break;
        }
        case 0xB000: {
            program_counter = registers[0] + opcode & 0x0FFF;
            break;
        }
        case 0xC000: {
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution distrib(0, 255);
            registers[digit<2>(opcode)] = distrib(gen) & opcode & 0x00FF;
            break;
        }
        case 0xD000: {
            const uint8_t starting_x = registers[digit<2>(opcode)] % 64;
            const uint8_t starting_y = registers[digit<1>(opcode)] % 32;
            const uint8_t rows = digit<0>(opcode);

            bool flipped = false;

            for (int i = 0; i < rows; i++) {
                const uint8_t y = starting_y + i;
                if (y >= 32) break;

                const uint8_t row = memory[i_register + i];

                for (int j = 0; j < 8; j++) {
                    const uint8_t x = starting_x + j;
                    if (x >= 64) break;

                    const bool on = (row >> (7 - j) & 0b1) == 0b1;

                    if (pixels[y][x] && on) {
                        flipped = true;
                    }

                    pixels[y][x] ^= on;
                }
            }

            registers[0xF] = flipped;
            break;
        }
        case 0xE000: {
            switch (opcode & 0xE0FF) {
                case 0xE09E: {
                    if (keys[registers[digit<2>(opcode)]]) {
                        program_counter += 2;
                    }
                    break;
                }
                case 0xE0A1: {
                    if (!keys[registers[digit<2>(opcode)]]) {
                        program_counter += 2;
                    }
                    break;
                }
                default: {
                    throw std::runtime_error(std::format("Invalid opcode: 0x{:04X}", opcode));
                }
            }
            break;
        }
        case 0xF000: {
            switch (opcode & 0xF0FF) {
                case 0xF007: {
                    registers[digit<2>(opcode)] = delay_timer;
                    break;
                }
                case 0xF00A: {
                    for (int i = 0; i < keys.size(); i++) {
                        if (keys[i]) {
                            registers[digit<2>(opcode)] = i;
                            return;
                        }
                    }

                    program_counter -= 2;
                    break;
                }
                case 0xF015: {
                    delay_timer = registers[digit<2>(opcode)];
                    break;
                }
                case 0xF018: {
                    sound_timer = registers[digit<2>(opcode)];
                    break;
                }
                case 0xF01E: {
                    i_register += registers[digit<2>(opcode)];
                    break;
                }
                case 0xF029: {
                    i_register = (digit<2>(opcode) & 0x000F) * 5;
                    break;
                }
                case 0xF033: {
                    const uint8_t num = registers[digit<2>(opcode)];

                    const uint8_t hundredths = num / 100;
                    const uint8_t tenths = (num - hundredths * 100) / 10;
                    const uint8_t ones = num - hundredths * 100 - tenths * 10;

                    memory[i_register] = hundredths;
                    memory[i_register + 1] = tenths;
                    memory[i_register + 2] = ones;
                    break;
                }
                case 0xF055: {
                    for (int i = 0; i <= digit<2>(opcode); i++) {
                        memory[i_register++] = registers[i];
                    }
                    break;
                }
                case 0xF065: {
                    for (int i = 0; i <= digit<2>(opcode); i++) {
                        registers[i] = memory[i_register++];
                    }
                    break;
                }
                default: {
                    throw std::runtime_error(std::format("Invalid opcode: 0x{:04X}", opcode));
                }
            }
            break;
        }
    }
}

void Cpu::tick_timers() {
    if (delay_timer > 0) {
        delay_timer -= 1;
    }

    if (sound_timer > 0) {
        if (sound_timer == 1) {
            // Beep?
        }
        sound_timer -= 1;
    }
}

uint16_t Cpu::fetch_opcode() {
    const uint8_t one = memory[program_counter++];
    const uint8_t two = memory[program_counter++];

    return one << 8 | two;
}

void Cpu::push(const uint16_t address) {
    if (stack_pointer == stack.size()) {
        throw std::runtime_error("Stack overflowed");
    }

    stack[stack_pointer++] = address;
}

uint16_t Cpu::pop() {
    if (stack_pointer == 0) {
        throw std::runtime_error("Tried pop with 0 elements");
    }

    return stack[--stack_pointer];
}
