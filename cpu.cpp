#include "cpu.hpp"
#include <stdexcept>

void cpu::handle_opcode(const short opcode) {
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0:
                    break;
                case 0x00EE:
                    break;
                default:
                    break;
            }
            break;
        case 0x1000:
            break;
        case 0x2000:
            break;
        case 0x3000:
            break;
        case 0x4000:
            break;
        case 0x5000:
            break;
        case 0x6000:
            break;
        case 0x7000:
            break;
        case 0x8000:
            switch (opcode & 0x800F) {
                case 0x8000:
                    break;
                case 0x8001:
                    break;
                case 0x8002:
                    break;
                case 0x8003:
                    break;
                case 0x8004:
                    break;
                case 0x8005:
                    break;
                case 0x8006:
                    break;
                case 0x8007:
                    break;
                case 0x800E:
                    break;
                default:
                    break;
            }
            break;
        case 0x9000:
            break;
        case 0xA000:
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
        default:
            throw std::runtime_error("Unknown opcode");
    }
}
