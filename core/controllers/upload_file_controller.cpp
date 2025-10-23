#include "upload_file_controller.h"
#include "../core/global.h"
#include "../thirdparty/ImGuiFileDialog/ImGuiFileDialog.h"
#include <iostream>

static bool showFileDialog = false;

void upload_file_controller::RequestOpenFile() {
    auto& state = global::get();
    if (!state.user.authenticated)
        return;

    showFileDialog = true; // flag interno come per showLoginPopup
}

void upload_file_controller::CloseFile() {
    auto& state = global::get();
    state.AppState.loadedFiles.clear(); // reset dei file caricati
}

void LoadCSV(const std::string& filePath) {
    auto& state = global::get();
    std::cout << "Caricando file CSV: " << filePath << std::endl;

    // Esempio di memorizzazione dei file caricati, simile a AppState.loadedFiles
    state.AppState.loadedFiles.push_back(filePath);
}

void upload_file_controller::RenderFileDialog() {
    if (showFileDialog) {
        ImGuiFileDialog::Instance()->OpenDialog(
            "ChooseCSV",
            "Scegli un file CSV",
            ".csv\0"
        );
        showFileDialog = false;
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseCSV")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            LoadCSV(filePath);
        }
        ImGuiFileDialog::Instance()->Close();
    }
}

