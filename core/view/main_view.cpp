#include "main_view.h"
#include "../core/global.h"
#include "imgui.h"
// Variabile globale temporanea per tab attivo
int currentTabIndex = 0;

// Dummy tab per il momento
struct CsvFileTab {
    std::string name;
    bool active = true;
};

CsvFileTab exampleTab = { "Example File" };

float sidebarWidth = 300.0f;

void RenderMainView() {
    ImGui::Begin("Main View", nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) { ImGui::EndMenu(); }
        if (ImGui::BeginMenu("Settings")) { ImGui::EndMenu(); }
        ImGui::EndMenuBar();
    }

    ImVec2 winSize = ImGui::GetContentRegionAvail();

    // Sidebar
    ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, winSize.y), true);
    ImGui::Text("Sidebar items...");
    ImGui::EndChild();

    ImGui::SameLine();

    // Splitter (trascinabile)
    ImGui::PushID("Splitter");
    ImGui::InvisibleButton("##splitter", ImVec2(5, winSize.y));
    if (ImGui::IsItemActive()) {
        sidebarWidth += ImGui::GetIO().MouseDelta.x;
        if (sidebarWidth < 100.0f) sidebarWidth = 100.0f; // min width
        if (sidebarWidth > winSize.x - 100.0f) sidebarWidth = winSize.x - 100.0f; // max width
    }
    ImGui::PopID();

    ImGui::SameLine();

    // Content area
    ImGui::BeginChild("Content", ImVec2(0, winSize.y), true);
    ImGui::Text("Area principale dei tab e grafici...");
    ImGui::EndChild();


    ImGui::End(); // Main View
}
