// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

namespace Phezu {
    
    class StreamWriter {
    public:
        virtual bool Write(const char* data, size_t size) = 0;
    };
    
    class StreamReader {
    public:
        virtual bool Read(char* destination, size_t size) = 0;
        virtual size_t Size() = 0;
    };
}
