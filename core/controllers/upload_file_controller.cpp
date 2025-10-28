#include "upload_file_controller.h"
#include "../data/global.h"
#include "../utilities/csv_parser.h"
#include <iostream>
#include "../utilities/csv_parser.h"

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
        return; // nessun file valido selezionato
    }

    // rimozione del file
    state.AppState.loadedFiles.erase(state.AppState.loadedFiles.begin() + index);

    // se è vuoto mette a -1 sennò va al primo file
    if (state.AppState.loadedFiles.empty())
        state.AppState.selectedFileIndex = -1;
    else
        state.AppState.selectedFileIndex = 0;
}

void upload_file_controller::LoadFile(const std::string& filePath) {
    auto& state = global::get();
    try {

        // prendo il nome del file
        std::string nome = GetFileNameFromPath(filePath);

        // controlla che non sia già caricato, in caso return senza caricarlo.
        for (int i = 0; i < state.AppState.loadedFiles.size(); i++) {
            if ( nome == state.AppState.loadedFiles[i].fileName) {
                std::cerr << "File già caricato" << std::endl;
                return;
            }
        }

        // tenta di parsare il file
        TelemetryData newFile = CSVParser::Parse(filePath);

        // aggiungo il file parsato alla lista
        state.AppState.loadedFiles.push_back(newFile);

        // lo seleziono in modo da visualizzarlo
        state.AppState.selectedFileIndex = state.AppState.loadedFiles.size() - 1;

    } catch (const std::runtime_error& e) {
        std::cerr << "Errore caricamento file: " << e.what() << std::endl;
    }
}
