#pragma once

#include <fstream>

#include <string>
#include <vector>

namespace calc {
    std::vector<std::vector<std::string>> get_tokens(std::ifstream& input);
}