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