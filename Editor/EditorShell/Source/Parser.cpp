#include <vector>
#include "Parser.hpp"

#include <filesystem>

namespace Phezu::Editor {

    void TokenizeInput(const std::string& input, std::vector<std::string>& output) {
        output.clear();

        int tokenStartIndex = 0;
        for (int i = 0; i < input.size(); i++) {
            if (input[i] != ' ') {
                if (i > tokenStartIndex) {
                    output.push_back(input.substr(tokenStartIndex, i - tokenStartIndex));
                }

                continue;
            }

            if (i > tokenStartIndex) {
                output.push_back(input.substr(tokenStartIndex, i - tokenStartIndex));
            }

            tokenStartIndex = i;
        }


    }

    void ParseInput(const std::string& input, Command& commandObj) {
        for (int i = 0; i < 4; i++) {
            if (commandObj.Arguments[i] != nullptr)
                delete commandObj.Arguments[i];
        }
        std::memset(&commandObj, 0, sizeof(Command));

        std::vector<std::string> tokens;
        TokenizeInput(input, tokens);

        std::string commandType = tokens[0];
        tokens.erase(tokens.begin());

        if (commandType == "Exit" && tokens.empty()) {
            commandObj.Type = CommandType::Exit;
            return;
        }
        if (commandType == "Build" && tokens.empty()) {
            commandObj.Type = CommandType::Build;
            return;
        }
        if (commandType == "Open" && tokens.size() == 1) {
            commandObj.Type = CommandType::Open;
            std::filesystem::path path(tokens[0]);
            commandObj.Arguments[0] = new char[tokens[0].size()];
            std::memcpy(commandObj.Arguments[0], tokens[0].data(), tokens[0].size());

            return;
        }

        commandObj.Type = CommandType::Invalid;
    }
}
