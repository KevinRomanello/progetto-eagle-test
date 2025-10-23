#include "upload_file_view.h"
#include "../controllers/upload_file_controller.h"
#include "../data/global.h"
#include "../thirdparty/ImGuiFileDialog/ImGuiFileDialog.h"


void RenderUploadFileView() {
    auto& state = global::get();

    if (state.AppState.showUploadDialog) {
        ImGuiFileDialog::Instance()->OpenDialog(
            "ChooseCSV",
            "Scegli un file CSV",
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


    if (ImGuiFileDialog::Instance()->Display("ChooseCSV")) {

        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            upload_file_controller::LoadFile(filePath);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}