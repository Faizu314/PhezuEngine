// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <string>
#include "Command.hpp"

namespace Phezu::Editor {

    void ParseInput(const std::string& input, Command& commandObj);
}