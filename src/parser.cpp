#include <parser.hpp>

#include <algorithm>

#include <stdexcept>
#include <cstring>

static bool is_text(const char& _c) {
    char c = std::tolower(_c);
    return (c >= 'a' && c <= 'z') || c == '_';
}

static size_t find(const std::vector<std::string>& vec, const std::string& var) {
    return std::find(vec.begin(), vec.end(), var) - vec.begin();
}

void calc::get_instructions(std::vector<std::string>& tokens, std::vector<std::string>& vars, std::vector<Instruction>& instructions) {
    if (tokens[1] == "=") {
        if (find(vars, tokens[0]) == vars.size()) vars.push_back(tokens[0]);

        if (is_text(tokens[2][0])) {
            if (find(vars, tokens[2]) == vars.size()) throw std::runtime_error("Unknown variable '" + tokens[2] + "'.");
            instructions.push_back({InstructionType::Load, new unsigned char[1] { static_cast<unsigned char>(find(vars, tokens[2])) }});
        } else {
            float val = std::stof(tokens[2]);
                
            unsigned char* data = new unsigned char[sizeof(float)];
            std::memcpy(data, &val, sizeof(float));
                
            instructions.push_back({InstructionType::LoadConst, data });
        }
            
        if (tokens.size() == 5) {
            if (is_text(tokens[4][0])) {
                if (find(vars, tokens[4]) == vars.size()) throw std::runtime_error("Unknown variable '" + tokens[4] + "'.");
                instructions.push_back({InstructionType::Load, new unsigned char[1] { static_cast<unsigned char>(find(vars, tokens[4])) }});
            } else {
                float val = std::stof(tokens[4]);
                
                unsigned char* data = new unsigned char[sizeof(float)];
                std::memcpy(data, &val, sizeof(float));
                
                instructions.push_back({InstructionType::LoadConst, data });
            }

            Instruction i;
            i.data = nullptr;

            switch(tokens[3][0]) {
                case '+':
                    i.type = InstructionType::Add;
                    break;
                case '-':
                    i.type = InstructionType::Sub;
                    break;
                case '*':
                    i.type = InstructionType::Mul;
                    break;
                default:
                    i.type = InstructionType::Div;
                    break;
            }

            instructions.push_back(std::move(i));
        }
            
        instructions.push_back({InstructionType::Store, new unsigned char[1] { static_cast<unsigned char>(find(vars, tokens[0])) }});
    } else {
        Instruction i;

        if (tokens[0] == "print") i.type = InstructionType::Print;
        else throw std::runtime_error("Unknown command '" + tokens[0] + "'.");

        unsigned char flag = 0;
        if (tokens.size() == 3) {
            if (tokens[2] != "nl") throw std::runtime_error("Unknown flag '" + tokens[2] + "'.");
            flag = 1;
        }
        
        if (tokens[1][0] == 0) {
            i.data = new unsigned char[3];
            i.data[0] = flag;
            i.data[1] = 0;

            std::string name = tokens[1].substr(1);
            size_t var = find(vars, name);
            if (var == vars.size()) throw std::runtime_error("Unknown variable '" + name + "'.");
            i.data[2] = static_cast<unsigned char>(var);
        } else {
            i.data = new unsigned char[tokens[1].length() + 1];
            i.data[0] = flag;

            std::copy(
                tokens[1].begin(),
                tokens[1].end(),
                i.data + 1
            );
        }

        instructions.push_back(std::move(i));
    }
}