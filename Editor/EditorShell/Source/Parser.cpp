#include <vector>
#include "Parser.hpp"

#include <filesystem>

namespace Phezu::Editor {

    void TokenizeInput(const std::string& input, std::string& type, std::vector<std::string>& args) {
        args.clear();

        bool state = false; // false -> space, true -> token
        int index = 0;
        for (int i = 0; i < input.size(); i++) {
            const char c = input[i];

            if (!state || i == 0) {
                if (c != ' ')
                    state = true;
                else
                    index = i;
            }
            else {
                if (c == ' ') {
                    type = input.substr(index, i - index);
                    index = i + 1;
                    break;
                }
                if (i == input.size() - 1) {
                    type = input.substr(index, i - index + 1);
                    return;
                }
            }
        }

        std::transform(type.begin(), type.end(), type.begin(), ::tolower);

        state = false; // false -> string not started, true -> string started
        for (int i = index; i < input.size(); i++) {
            const char c = input[i];

            if (!state || i == index) {
                if (c == '"') {
                    state = true;
                    index = i + 1;
                }
            }
            else {
                if (c == '"') {
                    args.push_back(input.substr(index, i - index));
                    state = false;
                }
            }
        }
    }

    void ParseInput(const std::string& input, Command& commandObj) {
        for (int i = 0; i < 4; i++) {
            if (commandObj.Arguments[i] != nullptr)
                delete commandObj.Arguments[i];
        }
        std::memset(&commandObj, 0, sizeof(Command));

        std::string commandType;
        std::vector<std::string> tokens;
        TokenizeInput(input, commandType, tokens);

        if (commandType == "exit" && tokens.empty()) {
            commandObj.Type = CommandType::Exit;
            return;
        }
        if (tokens.size() == 1) {
            if (commandType == "build") {
                commandObj.Type = CommandType::Build;
            }
            else if (commandType == "open") {
                commandObj.Type = CommandType::Open;
            }
            else {
                commandObj.Type = CommandType::Invalid;
                return;
            }

            std::filesystem::path path(tokens[0]);
            commandObj.Arguments[0] = new char[tokens[0].size() + 1];
            std::memcpy(commandObj.Arguments[0], tokens[0].data(), tokens[0].size());
            commandObj.Arguments[0][tokens[0].size()] = '\0';

            return;
        }

        commandObj.Type = CommandType::Invalid;
    }
}
