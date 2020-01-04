#pragma once

#include <string>
#include <random>
#include <array>

const unsigned int KEY_COUNT = 16;
const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTER_COUNT = 16;
const unsigned int STACK_SIZE = 16;
const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int FONT_SET_SIZE = 80;

class Chip8 {
public:
    explicit Chip8(bool altOp);

    void cycle();

    void loadRom(const std::string &filepath);

    uint8_t *keys();

    [[nodiscard]] const uint32_t *video() const;

    [[nodiscard]] bool drawFlag() const;

    void disableDrawFlag();

    [[nodiscard]] bool soundFlag() const;

    void disableSoundFlag();

private:
    void clearScreen();

    void decodeFuncTable0();

    void decodeFuncTable8();

    void decodeFuncTableE();

    void decodeFuncTableF();

    void opcodeUnknown();

    void opcode00E0();

    void opcode00EE();

    void opcode1NNN();

    void opcode2NNN();

    void opcode3XKK();

    void opcode4XKK();

    void opcode5XY0();

    void opcode6XKK();

    void opcode7XKK();

    void opcode8XY0();

    void opcode8XY1();

    void opcode8XY2();

    void opcode8XY3();

    void opcode8XY4();

    void opcode8XY5();

    void opcode8XY6();

    void opcode8XY7();

    void opcode8XYE();

    void opcode9XY0();

    void opcodeANNN();

    void opcodeBNNN();

    void opcodeCXKK();

    void opcodeDXYN();

    void opcodeEX9E();

    void opcodeEXA1();

    void opcodeFX07();

    void opcodeFX0A();

    void opcodeFX15();

    void opcodeFX18();

    void opcodeFX1E();

    void opcodeFX29();

    void opcodeFX33();

    void opcodeFX55();

    void opcodeFX65();

    uint8_t memory_[MEMORY_SIZE];
    uint8_t registers_[REGISTER_COUNT];
    uint16_t opcode_;
    uint16_t index_;
    uint16_t pc_;

    uint16_t stack_[STACK_SIZE];
    uint16_t sp_;

    uint32_t video_[VIDEO_WIDTH * VIDEO_HEIGHT]; // uint32_t used to work with SDL well

    uint8_t delayTimer_;
    uint8_t soundTimer_;

    uint8_t keys_[KEY_COUNT];

    std::array<uint8_t, FONT_SET_SIZE> fontSet_;

    bool drawFlag_;
    bool soundFlag_;

    const bool altOp_{}; // bool for specifying the behaviour of FX55 and FX65 opcodes

    std::default_random_engine randGen_;
    std::uniform_int_distribution<uint8_t> randByte_;

    // Set up function pointer tables, initialising them with a default function
    using chip8Func = void (Chip8::*)();

    chip8Func funcTable_[0xF + 1]{&Chip8::opcodeUnknown};
    chip8Func funcTable0_[0xE + 1]{&Chip8::opcodeUnknown};
    chip8Func funcTable8_[0xE + 1]{&Chip8::opcodeUnknown};
    chip8Func funcTableE_[0xE + 1]{&Chip8::opcodeUnknown};
    chip8Func funcTableF_[0x65 + 1]{&Chip8::opcodeUnknown};
};