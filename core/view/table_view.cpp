#include "table_view.h"
#include "imgui.h"
#include <string>

// Questo è il codice che prima era 'static RenderDataTable'
void RenderTableView(TelemetryData& currentFile) {
    ImGui::Separator();

    int num_cols = currentFile.columnNames.size();
    int num_rows = currentFile.GetTimestamps().size();

    if (num_rows == 0 || num_cols == 0) {
        ImGui::Text("Nessun dato da visualizzare.");
        return;
    }

    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                            ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollX |
                            ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable("DataTable", num_cols, flags, ImVec2(-1, -1))) {

        for (const auto& colName : currentFile.columnNames) {
            ImGui::TableSetupColumn(colName.c_str());
        }
        ImGui::TableHeadersRow();

        for (int row = 0; row < num_rows; ++row) {
            ImGui::TableNextRow();
            for (int col_idx = 0; col_idx < num_cols; ++col_idx) {
                ImGui::TableSetColumnIndex(col_idx);

                const std::string& colName = currentFile.columnNames[col_idx];

                // --- CORREZIONE ---
                // Dobbiamo accedere a .values
                double value = currentFile.columns.at(colName).values[row];

                ImGui::Text("%.4f", value);
            }
        }

        ImGui::EndTable();
    }
}