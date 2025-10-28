#include "content_view.h"
#include "../core/data/global.h"
#include "imgui.h"
#include "table_view.h"
#include "plot_view.h"


void RenderContentView() {
    auto& state = global::get();

    // controlla se un file è selezionato
    if (state.AppState.selectedFileIndex != -1) {

        TelemetryData& currentFile = state.AppState.loadedFiles[state.AppState.selectedFileIndex];

        // Disegna la TabBar
        if (ImGui::BeginTabBar("FileContentTabs")) {

            // Tab 1: Dati
            if (ImGui::BeginTabItem("Data Table")) {
                RenderTableView(currentFile);
                ImGui::EndTabItem();
            }

            // Tab 2: Grafici
            if (ImGui::BeginTabItem("Plots")) {
                RenderPlotView(currentFile);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

    } else {
        // nessun file selezionato
        // istruzioni all'avvio del programma
        ImGui::Text("No file selected.");
        ImGui::Text("Log in from the menu 'Login...'");
        ImGui::Text("Load a file from the menu 'File > Load File...'");
    }
}