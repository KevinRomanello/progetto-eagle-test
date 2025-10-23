#pragma once
#include "telemetry_data.h"
#include <string>

struct AppState {
    bool showLoginPopup = false;
    bool showUploadDialog = false;

    std::vector<TelemetryData> loadedFiles; // Usa la nostra nuova struct
    int selectedFileIndex = -1;
};
