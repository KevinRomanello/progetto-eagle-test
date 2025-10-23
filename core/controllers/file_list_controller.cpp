#include "file_list_controller.h"
#include "../core/data/global.h"

void file_list_controller::SelectFile(int index) {
    auto& state = global::get();

    // Controlla la validità dell'indice prima di modificare il Model
    if (index >= 0 && index < state.AppState.loadedFiles.size()) {
        state.AppState.selectedFileIndex = index;
    }
}