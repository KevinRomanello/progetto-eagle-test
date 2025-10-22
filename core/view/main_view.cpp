#include "main_view.h"
#include "../core/global.h"
#include "imgui.h"

void RenderMainView() {
    ImGui::Begin("Main View");
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open DLL...")) { /* open file dialog */ }
            if (ImGui::MenuItem("Exit")) { /* quit app */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) { /* options */ ImGui::EndMenu(); }
        ImGui::EndMainMenuBar();
    }

    int currentTab = 0;

    ImGui::BeginChild("Sidebar", ImVec2(200, 0), true);
    if (ImGui::Selectable("Home", currentTab == 0)) currentTab = 0;
    if (ImGui::Selectable("Loaded DLLs", currentTab == 1)) currentTab = 1;
    if (ImGui::Selectable("Logs", currentTab == 2)) currentTab = 2;
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("Content", ImVec2(0, 0), true);
    switch (currentTab) {/*
        case 0: DrawHomeTab(); break;
        case 1: DrawDllManagerTab(); break;
        case 2: DrawLogsTab(); break;*/
    }
    ImGui::EndChild();

    if (ImGui::Button("Settings"))
        ImGui::OpenPopup("SettingsPopup");
    if (ImGui::BeginPopupModal("SettingsPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Configuration options here");
        if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}
