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

void calc::get_instructions(const std::vector<std::vector<std::string>>& tokens, std::vector<std::string>& vars, std::vector<Instruction>& instructions) {
    for (const std::vector<std::string>& line : tokens) {
        if (line[1] == "=") {
            if (find(vars, line[0]) == vars.size()) vars.push_back(line[0]);

            if (is_text(line[2][0])) {
                if (find(vars, line[2]) == vars.size()) throw std::runtime_error("Unknown variable '" + line[2] + "'.");
                instructions.push_back({InstructionType::Load, new unsigned char[1] { static_cast<unsigned char>(find(vars, line[2])) }});
            } else {
                float val = std::stof(line[2]);
                
                unsigned char* data = new unsigned char[sizeof(float)];
                std::memcpy(data, &val, sizeof(float));
                
                instructions.push_back({InstructionType::LoadConst, data });
            }
            
            if (line.size() == 5) {
                if (is_text(line[4][0])) {
                    if (find(vars, line[4]) == vars.size()) throw std::runtime_error("Unknown variable '" + line[4] + "'.");
                    instructions.push_back({InstructionType::Load, new unsigned char[1] { static_cast<unsigned char>(find(vars, line[4])) }});
                } else {
                    float val = std::stof(line[4]);
                
                    unsigned char* data = new unsigned char[sizeof(float)];
                    std::memcpy(data, &val, sizeof(float));
                
                    instructions.push_back({InstructionType::LoadConst, data });
                }

                Instruction i;
                i.data = nullptr;

                switch(line[3][0]) {
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
            
            instructions.push_back({InstructionType::Store, new unsigned char[1] { static_cast<unsigned char>(find(vars, line[0])) }});
        } else {
            Instruction i;

            if (line[0] == "print") i.type = InstructionType::Print;
            else throw std::runtime_error("Unknown command '" + line[0] + "'.");

            unsigned char flag = 0;
            if (line.size() == 3) {
                if (line[2] != "nl") throw std::runtime_error("Unknown flag '" + line[2] + "'.");
                flag = 1;
            }
        
            if (line[1][0] == 0) {
                unsigned char* data = new unsigned char[3];
                data[0] = flag;
                data[1] = 0;

                std::string name = line[1].substr(1);
                size_t var = find(vars, name);
                if (var == vars.size()) throw std::runtime_error("Unknown variable '" + name + "'.");
                data[2] = static_cast<unsigned char>(var);
                
                i.data = data;
            } else {
                unsigned char* data = new unsigned char[line[1].length() + 1];
                data[0] = flag;

                std::copy(
                    line[1].begin(),
                    line[1].end(),
                    data + 1
                );

                i.data = data;
            }

            instructions.push_back(std::move(i));
        }
    }
}