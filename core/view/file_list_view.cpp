#include "file_list_view.h"
#include "../core/data/global.h"
#include "../controllers/file_list_controller.h"
#include "imgui.h"

void RenderFileListView() {
    auto& state = global::get();

    ImGui::Text("Loaded Files:");
    ImGui::Separator();

    // crea un selectable per ogni file sostanzialmente
    for (int i = 0; i < state.AppState.loadedFiles.size(); ++i) {
        
        const std::string& fileName = state.AppState.loadedFiles[i].fileName;
        bool isSelected = (state.AppState.selectedFileIndex == i);

        if (ImGui::Selectable(fileName.c_str(), isSelected)) {
            file_list_controller::SelectFile(i);
        }
    }
}