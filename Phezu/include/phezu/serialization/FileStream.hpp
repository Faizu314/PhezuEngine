#pragma once

#include <string>
#include <fstream>
#include "serialization/Stream.hpp"

namespace Phezu {
    
    class FileStreamWriter : public StreamWriter {
    public:
        FileStreamWriter(const std::string& filePath) : m_Filestream(filePath, std::ios::out) {}
        ~FileStreamWriter() {
            if (m_Filestream.is_open())
                m_Filestream.close();
        }
    public:
        virtual bool Write(const char* data, size_t size) override {
            m_Filestream.write(data, size);
            return true;
        }
    private:
        std::ofstream m_Filestream;
    };
    
    class FileStreamReader : public StreamReader {
    public:
        FileStreamReader(const std::string& filePath) : m_Filestream(filePath, std::ios::in) {}
        ~FileStreamReader() {
            if (m_Filestream.is_open())
                m_Filestream.close();
        }
    public:
        virtual bool Read(char* data, size_t size) override {
            m_Filestream.read(data, size);
            return true;
        }
        virtual size_t Size() override {
            size_t currentPos = m_Filestream.tellg();
            m_Filestream.seekg( 0, std::ios::end );
            size_t size = static_cast<size_t>(m_Filestream.tellg()) - currentPos;
            m_Filestream.seekg(currentPos, std::ios::beg);
            return size;
        }
    private:
        std::ifstream m_Filestream;
    };
}
