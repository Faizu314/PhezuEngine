// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

namespace Phezu::Editor {

    enum class CommandType {
        None = 0,
        Invalid,
        Exit,
        Open,
        Build
    };

    struct Command {
        CommandType Type;
        char* Arguments[4];
    };
}