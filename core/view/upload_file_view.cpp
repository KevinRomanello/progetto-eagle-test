#include "upload_file_view.h"
#include "../controllers/upload_file_controller.h"
#include "../data/global.h"
#include "../thirdparty/ImGuiFileDialog/ImGuiFileDialog.h"

// utilizzo ImGuiFileDialog che è uan finestra per sciegliere i file già fatta
void RenderUploadFileView() {
    auto& state = global::get();

    if (state.AppState.showUploadDialog) {
        ImGuiFileDialog::Instance()->OpenDialog(
            "ChooseCSV",
            "Choose CSV File",
            ".csv\0"
        );
        state.AppState.showUploadDialog = false;
    }

    if (!state.user.authenticated) {
        if (ImGuiFileDialog::Instance()->IsOpened("ChooseCSV")) {
            ImGuiFileDialog::Instance()->Close();
        }
        return;
    }

    ImVec2 fixedDialogSize = ImVec2(800, 400);
    ImGuiWindowFlags dialogFlags = ImGuiWindowFlags_NoResize;

    if (ImGuiFileDialog::Instance()->Display("ChooseCSV", dialogFlags, fixedDialogSize)) {

        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            upload_file_controller::LoadFile(filePath);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}