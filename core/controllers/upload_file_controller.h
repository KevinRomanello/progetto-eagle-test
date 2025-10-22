#pragma once
#include "../ImGuiFileDialog/ImGuiFileDialog.h"
#include <string>

class upload_file_controller {
public:
    void RequestOpenFile();
    void CloseFile();
    void RenderFileDialog();  // deve esistere anche nel cpp
};