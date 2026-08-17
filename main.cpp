#include <iostream>
#include <filesystem>
#include <fstream>

#include <lexer.hpp>
#include <parser.hpp>

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: calcc <input.calc> [output.bcalc]\n";
        return 1;
    }

    std::filesystem::path input = argv[1], output;

    if (argc == 2) {
        output = input;
        output.replace_extension(".bcalc");
    } else {
        output = argv[2];
    }

    std::ifstream inp(input);
    if (!inp) {
        std::cerr << "Unable to open input file '" + input.string() + "'.\n";
        return 1;
    }

    std::ofstream file(output, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open output file '" + output.string() + "'.\n";
        return 1;
    }

    file.write("bcalc", 5);

    std::vector<std::string> vars;
    std::vector<Instruction> instructions;

    std::string line;
    while (std::getline(inp, line)) {
        std::vector<std::string> tokens = calc::get_tokens(line);
        if (tokens.empty()) continue;
        calc::get_instructions(tokens, vars, instructions);

        for (Instruction& i : instructions) {
            file.put(static_cast<char>(i.type));

            switch (i.type) {
                case InstructionType::Load:
                case InstructionType::Store:
                    file.write(reinterpret_cast<const char*>(i.data), 1);
                    break;

                case InstructionType::LoadConst:
                    file.write(reinterpret_cast<const char*>(i.data), sizeof(float));
                    break;

                case InstructionType::Print: {
                    file.write(reinterpret_cast<const char*>(i.data), i.data[1] == 0 ? 3 : i.data[1] + 2);
                    break;
                }

                case InstructionType::Add:
                case InstructionType::Sub:
                case InstructionType::Mul:
                case InstructionType::Div:
                    break;
            }

            delete[] i.data;
            i.data = nullptr;
        }

        instructions.clear();
    }

    file.put('\0');

    for (const std::string& var : vars) {
        file.put(static_cast<char>(var.size()));
        file.write(var.data(), var.size());
    }

    return 0;
}