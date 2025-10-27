#include "file_list_view.h"
#include "../core/data/global.h"
#include "../controllers/file_list_controller.h"
#include "imgui.h"

void RenderFileListView() {
    auto& state = global::get();

    ImGui::Text("Loaded Files:");
    ImGui::Separator();

    // Itera sui file caricati nel Model
    for (int i = 0; i < state.AppState.loadedFiles.size(); ++i) {
        
        const std::string& fileName = state.AppState.loadedFiles[i].fileName;
        bool isSelected = (state.AppState.selectedFileIndex == i);

        // Disegna l'elemento
        if (ImGui::Selectable(fileName.c_str(), isSelected)) {
            
            // La View non modifica il Model, chiama il Controller
            file_list_controller::SelectFile(i);
        }
    }
}