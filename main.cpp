#include <iostream>

#include <lexer.hpp>

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

    auto lines = calc::get_tokens(inp);

    return 0;
}