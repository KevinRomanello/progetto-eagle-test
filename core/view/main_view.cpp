#include "main_view.h"

#include <iostream>
#include <ostream>

#include "../core/global.h"
#include "imgui.h"
#include "login_view.h"

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

    auto& state = global::get();

    // Menu bar in alto
    if (ImGui::BeginMenuBar()) {
        // FILE
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Carica File...")) {
                // TODO: apri file dialog e aggiungi a openFiles
                //openFiles.push_back("NuovoFile.csv");
            }
            if (/*!openFiles.empty() && */ImGui::MenuItem("Chiudi File")) {
                // Chiudi l'ultimo file aperto (esempio)
                //openFiles.pop_back();
            }
            ImGui::EndMenu();
        }

        // PROFILE
        if (ImGui::BeginMenu("Profile")) {
            if (!state.user.authenticated && ImGui::MenuItem("Login")) {
                state.AppState.showLoginPopup = true;
            }
            if (state.user.authenticated && ImGui::MenuItem("Logout")) {
                state.user.authenticated = false;
                state.AppState.loadedFiles.clear(); // chiudi file al logout
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (state.AppState.showLoginPopup) {
        RenderLoginView();
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
