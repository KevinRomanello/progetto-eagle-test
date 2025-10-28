#pragma once
#include "telemetry_data.h"
#include <string>

struct AppState {
    bool showLoginPopup = false;
    bool showUploadDialog = false;
    bool showUserManagementPopup = false;
    bool showAddUserPopup = false;

    std::vector<TelemetryData> loadedFiles;
    int selectedFileIndex = -1;

    // Indice dell'utente selezionato nel popup di gestione
    int selectedUserIndexInDb = -1;
};
