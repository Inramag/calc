#include <iostream>

#include <lexer.hpp>
#include <parser.hpp>

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: calcc <input.calc> [output.bcalc]\n";
        return 1;
    }

    std::string input = argv[1];
    std::string output;

    if (argc == 2) {
        output = input;
        output.replace(output.size() - 5, 5, ".bcalc");
    } else {
        output = argv[2];
    }

    std::ifstream inp(input);
    if (!inp) {
        std::cerr << "Unable to open input file '" + input + "'.";
        return 1;
    }

    std::vector<std::vector<std::string>> tokens = calc::get_tokens(inp);

    std::vector<std::string> vars;
    std::vector<Instruction> instructions;

    calc::get_instructions(tokens, vars, instructions);

    std::ofstream file(output, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open output file '" + output + "'.\n";
        return 1;
    }

    file.write("bcalc", 5);

    for (const std::string& var : vars) {
        file.put(static_cast<char>(var.size()));
        file.write(var.data(), var.size());
    }

    file.put('\0');

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

    return 0;
}