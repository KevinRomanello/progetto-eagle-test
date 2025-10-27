#include "plot_view.h"
#include "imgui.h"
#include "implot/implot.h"
#include "../core/data/global.h"

ImFont* gFontDefault = nullptr;

static const std::vector<ImVec4> plotColors = {
    ImVec4(0.0f, 0.7f, 1.0f, 1.0f), // Blu
    ImVec4(1.0f, 0.3f, 0.3f, 1.0f), // Rosso
    ImVec4(0.1f, 1.0f, 0.1f, 1.0f), // Verde
    ImVec4(1.0f, 0.7f, 0.0f, 1.0f), // Arancione
    ImVec4(0.7f, 0.3f, 1.0f, 1.0f), // Viola
    ImVec4(1.0f, 1.0f, 0.0f, 1.0f) // Giallo
};

// vettore per poter ridimensionare i bvari plot
static std::map<std::string, float> plotHeights;
static const float defaultPlotHeight = 250.0f; // Altezza di partenza
static const float minPlotHeight = 100.0f; // Altezza minima

// NUOVA VARIABILE STATICA PER L'ALTEZZA DEL MASTER PLOT
static float masterPlotHeight = 350.0f;

// Mappa statica per memorizzare la visibilità delle serie nel Master Plot
static std::map<std::string, bool> seriesVisibility;

void RenderPlotView(TelemetryData &currentFile) {
    ImGui::Separator();

    std::vector<double> &x_data = currentFile.GetTimestamps();
    if (x_data.empty()) {
        ImGui::Text("Nessun dato da plottare.");
        return;
    }

    const char *x_label = currentFile.columnNames[0].c_str();

    auto &state = global::get();
    auto &user = state.user;

    // 1. Disegna il Master Plot
    if (user.role == UserRole::ADVANCED || user.role == UserRole::ADMIN) {
        // --- LOOP 2: MASTER PLOT ---

        ImGui::PushFont(state.fonts.TitoloGrande);

        if (ImPlot::BeginPlot("MASTER PLOT", ImVec2(-1, masterPlotHeight))) {

            ImGui::PushFont(state.fonts.Default);
            // Imposta l'asse Y per essere bloccato tra 0 e 1 (range normalizzato)
            ImPlot::SetupAxes(x_label, "Normalized Value", ImPlotAxisFlags_None,
                              ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
            ImPlot::SetupAxisLimits(ImAxis_Y1, -0.1, 1.1, ImGuiCond_Always);

            // Vettore temporaneo per i dati normalizzati
            std::vector<double> normalized_data;

            for (int i = 1; i < currentFile.columnNames.size(); ++i) {
                const std::string &colName = currentFile.columnNames[i];

                DataColumn &column = currentFile.GetColumn(colName);
                double min = column.min;
                double max = column.max;
                double range = max - min;

                // Evita divisione per zero se tutti i valori sono uguali
                if (range == 0) range = 1.0;

                // 4. Calcola i dati normalizzati al volo
                normalized_data.clear();
                normalized_data.reserve(column.values.size());

                for (double val: column.values) {
                    // Formula di normalizzazione Min-Max
                    normalized_data.push_back((val - min) / range);
                }

                // 5. Applica colore e plotta
                int colorIndex = (i - 1) % plotColors.size();
                ImPlot::PushStyleColor(ImPlotCol_Line, plotColors[colorIndex]);

                // ImPlot::PlotLine(colName.c_str(), ...) aggiungerà automaticamente la serie
                ImPlot::PlotLine(colName.c_str(), x_data.data(), normalized_data.data(), x_data.size());

                ImPlot::PopStyleColor();
            }
            ImGui::PopFont();
            ImPlot::EndPlot();
        }

        ImGui::PopFont();

        // 1. Usiamo un ID stringa univoco per garantire che il bottone sia isolato
        // e tracciato correttamente da ImGui.
        ImGui::PushID("MasterPlotSplitter");

        // 2. Il bottone invisibile deve avere la stessa larghezza del plot
        // e un'altezza di circa 5 pixel.
        ImGui::InvisibleButton("##splitter", ImVec2(-1, 5.0f));

        // 3. Controlliamo se ci stiamo interagendo
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);

        // 4. L'errore è spesso qui: dobbiamo usare IsItemActive() per catturare il drag
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            // Aggiorniamo l'altezza del Master Plot
            masterPlotHeight += ImGui::GetIO().MouseDelta.y;

            // Applichiamo il limite minimo
            if (masterPlotHeight < minPlotHeight)
                masterPlotHeight = minPlotHeight;
        }

        ImGui::PopID(); // Rilasciamo l'ID
    }

    // --- LOOP 2: PLOT INDIVIDUALI ---
    for (int i = 1; i < currentFile.columnNames.size(); ++i) {
        const std::string &colName = currentFile.columnNames[i];
        std::vector<double> &y_data = currentFile.GetColumnData(colName);

        if (y_data.size() != x_data.size()) continue;

        // Se non abbiamo ancora un'altezza per questo grafico,
        // inizializzala a quella di default.
        if (plotHeights.find(colName) == plotHeights.end()) {
            plotHeights[colName] = defaultPlotHeight;
        }

        // Recupera un *riferimento* all'altezza di questo grafico
        float &currentHeight = plotHeights[colName];

        ImGui::PushFont(state.fonts.TitoloGrande);

        if (ImPlot::BeginPlot(colName.c_str(), ImVec2(-1, currentHeight))) {

            ImGui::PushFont(state.fonts.Default);

            ImPlot::SetupAxes(x_label, colName.c_str());

            // facciamo il i mod size(enum) sostanzialmente per continuare a usare in loop i colori
            int colorIndex = (i - 1) % plotColors.size();

            ImPlot::PushStyleColor(ImPlotCol_Line, plotColors[colorIndex]);

            ImPlot::PlotLine(colName.c_str(), x_data.data(), y_data.data(), x_data.size());

            ImPlot::PopStyleColor(); // rimuove il colore per non sporcare i grafici successivi

            ImGui::PopFont();

            ImPlot::EndPlot();
        }

        ImGui::PopFont();

        // --- SPLITTER ORIZZONTALE ---
        // Usiamo l'indice 'i' per il PushID.
        ImGui::PushID(i);

        ImGui::InvisibleButton("##splitter", ImVec2(-1, 5.0f));

        // Cambiamo il cursore quando ci passiamo sopra
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS); // Freccia Su/Giu

        // Quando l'utente clicca e trascina...
        if (ImGui::IsItemActive()) {
            // ...aggiorniamo l'altezza del grafico
            currentHeight += ImGui::GetIO().MouseDelta.y;

            // Applichiamo un limite minimo
            if (currentHeight < minPlotHeight)
                currentHeight = minPlotHeight;
        }

        ImGui::PopID(); // Rilasciamo l'ID
    }
}
