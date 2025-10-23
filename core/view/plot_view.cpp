#include "plot_view.h"
#include "imgui.h"
#include "implot/implot.h"

// Questo è il codice che prima era 'static RenderPlots'
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
            ImPlot::PlotLine(colName.c_str(), x_data.data(), y_data.data(), x_data.size());
            ImPlot::EndPlot();
        }
    }
}