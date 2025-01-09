#include "Cpu.hpp"
#include <cstdint>
#include <print>
#include <ranges>

Cpu::Cpu(std::span<uint16_t> opcodes) {
    std::ranges::copy(FONT_SET, memory.begin());
    std::ranges::copy(opcodes, memory.begin() + START_ADDR);
};

void Cpu::tick() {
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement (All cases handled)
    switch (const uint16_t opcode = memory[program_counter++]; opcode & 0xF000) {
        case 0x0000: {
            switch (opcode) {
                case 0x00E0: {
                    for (auto row: pixels) {
                        row.fill(false);
                    }
                    break;
                }
                case 0x00EE: {
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
        case 0x1000: {
            break;
        }
        case 0x2000: {
            break;
        }
        case 0x3000: {
            bool skip = registers[opcode & 0x0100 >> 8] == (opcode & 0x0011);
            break;
        }
        case 0x4000: {
            bool no_skip = registers[opcode & 0x0100 >> 8] != (opcode & 0x0011);
            break;
        }
        case 0x5000: {
            bool skip = registers[opcode & 0x0100 >> 8] != (opcode & 0x0010 >> 4);
            break;
        }
        case 0x6000:
            registers[opcode & 0x0100 >> 8] = static_cast<char>(opcode & 0x0011);
            break;
        case 0x7000: {
            // char + char = int
            registers[opcode & 0x0100 >> 8] = static_cast<char>(
                registers[opcode & 0x0100 >> 8] + static_cast<char>(opcode & 0x0011));

            break;
        }
        case 0x8000: {
            switch (opcode & 0x800F) {
                case 0x8000: {
                    break;
                }
                case 0x8001: {
                    break;
                }
                case 0x8002: {
                    break;
                }
                case 0x8003: {
                    break;
                }
                case 0x8004: {
                    break;
                }
                case 0x8005: {
                    break;
                }
                case 0x8006: {
                    break;
                }
                case 0x8007: {
                    break;
                }
                case 0x800E: {
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
        case 0x9000: {
            break;
        }
        case 0xA000: {
            break;
        }
        case 0xB000: {
            break;
        }
        case 0xC000: {
            break;
        }
        case 0xD000: {
            break;
        }
        case 0xE000: {
            break;
        }
        case 0xF000: {
            break;
        }
    }
}

void Cpu::tick_timers() {
    if (delay_timer > 0) {
        delay_timer -= 1;
    }

    if (sound_timer > 0) {
        if (stack_pointer == 1) {
            // Beep?
        }
        sound_timer -= 1;
    }
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
