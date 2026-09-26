#include <cstdio>
#include <cstring>
#include <fstream>
#include <instruction.hpp>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

char* data;
size_t pos = 5, size = 0;

float a = 0, b = 0, r = 0;
std::vector<std::pair<std::string, float>> vars{};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "use: calc <path/to/.bcalc>";
    return 0;
  }
  
  std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
  if (!file) throw std::runtime_error("Failed to open file.");

  file.seekg(0, std::ios::end);
  size = file.tellg();
  file.seekg(0);

  data = new char[size];
  if (!file.read(data, size)) {
    delete[] data;
    throw std::runtime_error("Failed to read file.");
  }

  if (std::memcmp(data, "bcalc", 5) != 0)
    throw std::runtime_error("Invalid bcalc file.");

  size_t sep = size - 1;
  while (data[sep] != '\0') sep--;

  {
    size_t pos = sep + 1;
    while (pos < size) {
      uint8_t len = static_cast<uint8_t>(data[pos++]);
      vars.emplace_back(std::pair{std::string(data + pos, len), 0});
      pos += len;
    }
  }

  size = sep;

  bool has_left_operand = false;

  while (pos < size) {
    InstructionType type = static_cast<InstructionType>(data[pos++]);

    switch (type) {
      case InstructionType::Load:
        if (!has_left_operand) a = vars[data[pos]].second;
        else b = vars[data[pos]].second;
        
        has_left_operand = !has_left_operand;
        pos++;

        break;
      case InstructionType::LoadConst:
        if (!has_left_operand) std::memcpy(&a, data + pos, 4);
        else std::memcpy(&b, data + pos, 4);

        has_left_operand = !has_left_operand;
        pos += 4;

        break;
      case InstructionType::Store: {
          uint8_t var = static_cast<uint8_t>(data[pos++]);

          if (has_left_operand) {
            vars[var].second = a;
            has_left_operand = false;
          } else {
            vars[var].second = r;
            a = 0;
            b = 0;
            r = 0;
          }

          break;
        }
      case InstructionType::Print: {
          bool nl = data[pos++];
          uint8_t len = data[pos++];
          if (len == 0) std::cout << vars[data[pos++]].second;
          else {
            std::cout.write(data + pos, len);
            pos += len;
          }
          if (nl) std::cout << '\n';
          
          break;
        }
      case InstructionType::Add: r = a + b; break;
      case InstructionType::Sub: r = a - b; break;
      case InstructionType::Mul: r = a * b; break;
      case InstructionType::Div:
        if (b == 0) throw std::runtime_error("Division by zero.");
        r = a / b;
        break;
    }
  }
  
  std::cout << '\n';
  
  return 0;
}
