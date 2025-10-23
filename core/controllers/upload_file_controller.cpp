#include "upload_file_controller.h"
#include "../data/global.h"
#include "../utilities/csv_parser.h"
#include <iostream>

void upload_file_controller::RequestOpenFile() {
    auto& state = global::get();
    if (!state.user.authenticated)
        return;

    state.AppState.showUploadDialog = true;
}

void upload_file_controller::CloseFile() {
    auto& state = global::get();

    int index = state.AppState.selectedFileIndex;
    if (index < 0 || index >= state.AppState.loadedFiles.size()) {
        return; // Nessun file valido selezionato
    }

    // Rimuovi il file dal vettore
    state.AppState.loadedFiles.erase(state.AppState.loadedFiles.begin() + index);

    // Resetta l'indice. Se c'erano file dopo, si "spostano"
    // quindi -1 è la scelta più sicura.
    state.AppState.selectedFileIndex = -1;
}

void upload_file_controller::LoadFile(const std::string& filePath) {
    auto& state = global::get();

    try {
        // 1. Tenta di parsare il file con il nuovo parser
        TelemetryData newFile = CSVParser::Parse(filePath);

        // 2. Aggiungi l'oggetto TelemetryData al Model
        state.AppState.loadedFiles.push_back(newFile);

        // 3. Seleziona automaticamente il file appena caricato
        state.AppState.selectedFileIndex = state.AppState.loadedFiles.size() - 1;

    } catch (const std::runtime_error& e) {
        std::cerr << "Errore caricamento file: " << e.what() << std::endl;
        // TODO creare un popup per la visualizzazione degli errori
    }
}
