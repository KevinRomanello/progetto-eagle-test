#include "plot_view.h"
#include "imgui.h"
#include "implot/implot.h"

static const std::vector<ImVec4> plotColors = {
    ImVec4(0.0f, 0.7f, 1.0f, 1.0f),  // Blu
    ImVec4(1.0f, 0.3f, 0.3f, 1.0f),  // Rosso
    ImVec4(0.1f, 1.0f, 0.1f, 1.0f),  // Verde
    ImVec4(1.0f, 0.7f, 0.0f, 1.0f),  // Arancione
    ImVec4(0.7f, 0.3f, 1.0f, 1.0f),  // Viola
    ImVec4(1.0f, 1.0f, 0.0f, 1.0f)   // Giallo
};

void RenderPlotView(TelemetryData& currentFile) {
    ImGui::Text("File: %s", currentFile.fileName.c_str());
    ImGui::Separator();

    std::vector<double>& x_data = currentFile.GetTimestamps();
    if (x_data.empty()) {
        ImGui::Text("Nessun dato da plottare.");
        return;
    }

    const char* x_label = currentFile.columnNames[0].c_str();

    for (int i = 1; i < currentFile.columnNames.size(); ++i) {
        const std::string& colName = currentFile.columnNames[i];
        std::vector<double>& y_data = currentFile.GetColumnData(colName);

        if (y_data.size() != x_data.size()) continue;

        if (ImPlot::BeginPlot(colName.c_str(), ImVec2(-1, 250))) {
            ImPlot::SetupAxes(x_label, colName.c_str());

            // facciamo il i mod size(enum) sostanzialmente per continuare a usare in loop i colori
            int colorIndex = (i - 1) % plotColors.size();

            ImPlot::PushStyleColor(ImPlotCol_Line, plotColors[colorIndex]);

            ImPlot::PlotLine(colName.c_str(), x_data.data(), y_data.data(), x_data.size());

            ImPlot::PopStyleColor();  // rimuove il colore per non sporcare i grafici successivi

            ImPlot::EndPlot();
        }
    }
}