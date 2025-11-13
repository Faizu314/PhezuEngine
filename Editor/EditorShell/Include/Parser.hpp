#pragma once

#include <string>
#include "Command.hpp"

namespace Phezu::Editor {

    void ParseInput(const std::string& input, Command& commandObj);
}