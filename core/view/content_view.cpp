#include "content_view.h"

#include <iostream>

#include "../core/data/global.h"
#include "imgui.h"
#include "table_view.h" // Include la sub-view
#include "plot_view.h"  // Include la sub-view

// Questa view viene chiamata DENTRO il BeginChild("Content"...) 
// della main_view
void RenderContentView() {
    auto& state = global::get();

    // Controlla se un file è selezionato
    if (state.AppState.selectedFileIndex != -1) {

        TelemetryData& currentFile = state.AppState.loadedFiles[state.AppState.selectedFileIndex];

        // Disegna la TabBar
        if (ImGui::BeginTabBar("FileContentTabs")) {

            // Tab 1: Dati
            if (ImGui::BeginTabItem("Data Table")) {
                RenderTableView(currentFile); // Chiama la sub-view
                ImGui::EndTabItem();
            }

            // Tab 2: Grafici
            if (ImGui::BeginTabItem("Plots")) {
                RenderPlotView(currentFile); // Chiama la sub-view
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

    } else {
        // Nessun file selezionato
        ImGui::Text("Nessun file selezionato.");
        ImGui::Text("Esegui l'accesso dal menu 'Profile > Login...'");
        ImGui::Text("Carica un file dal menu 'File > Carica File...'");
    }
}