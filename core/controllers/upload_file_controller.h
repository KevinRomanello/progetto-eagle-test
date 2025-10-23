#pragma once

#include <string>

class upload_file_controller {
public:
    static void RequestOpenFile();
    static void CloseFile();
    static void LoadFile(const std::string& filePath);
};