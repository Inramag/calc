#pragma once

#include <cstdint>

enum InstructionType : std::uint8_t {
    Load, LoadConst, LoadRes, Store, Print,

    Add, Sub,
    Mul, Div
};

struct Instruction {
    InstructionType type;
    unsigned char* data;
};