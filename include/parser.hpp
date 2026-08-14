#pragma once

#include <instruction.hpp>

#include <string>
#include <vector>

namespace calc {
    void get_instructions(const std::vector<std::vector<std::string>>& tokens, std::vector<std::string>& vars, std::vector<Instruction>& instructions);
}