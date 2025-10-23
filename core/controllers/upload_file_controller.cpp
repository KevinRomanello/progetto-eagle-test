#include "upload_file_controller.h"
#include "../core/global.h"
#include <iostream>

void upload_file_controller::RequestOpenFile() {
    auto& state = global::get();
    if (!state.user.authenticated)
        return;

    state.AppState.showUploadDialog = true;
}

void upload_file_controller::CloseFile() {
    auto& state = global::get();
    state.AppState.loadedFiles.clear();
}

void upload_file_controller::LoadFile(const std::string& filePath) {
    auto& state = global::get();
    std::cout << "Caricando file CSV: " << filePath << std::endl;

    state.AppState.loadedFiles.push_back(filePath);
}
