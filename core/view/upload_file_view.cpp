#include "../ImGuiFileDialog/ImGuiFileDialog.h"

void DrawMenuBar()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Apri CSV..."))
        {
            ImGuiFileDialog::Instance()->OpenDialog(
                "ChooseCSV",
                "Scegli un file CSV",
                ".csv\0"
            );
        }
        ImGui::EndMenu();
    }

    // Mostra la finestra del FileDialog
    if (ImGuiFileDialog::Instance()->Display("ChooseCSV"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            //LoadCSV(filePath); // <-- Qui carichi i dati
        }
        ImGuiFileDialog::Instance()->Close();
    }
}