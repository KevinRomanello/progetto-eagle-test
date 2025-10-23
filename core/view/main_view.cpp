#include "main_view.h"

#include <iostream>
#include <ostream>

#include "../core/global.h"
#include "imgui.h"
#include "login_view.h"
#include "upload_file_view.h"
#include "../controllers/upload_file_controller.h"
#include "../controllers/auth_controller.h"

// Variabile globale temporanea per tab attivo
int currentTabIndex = 0;

// Dummy tab per il momento
struct CsvFileTab {
    std::string name;
    bool active = true;
};

CsvFileTab exampleTab = { "Example File" };

upload_file_controller uploadController;
auth_controller authController;

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
                upload_file_controller::RequestOpenFile();
            }
            if (ImGui::MenuItem("Chiudi File")) {
                upload_file_controller::CloseFile();
            }
            ImGui::EndMenu();
        }

        // PROFILE
        if (ImGui::BeginMenu("Profile")) {
            if (!state.user.authenticated && ImGui::MenuItem("Login")) {
                auth_controller::RequestLogin();
            }
            if (state.user.authenticated && ImGui::MenuItem("Logout")) {
                auth_controller::RequestLogout();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    RenderLoginView();      // non i controlli vengono delegati alla funzione chiamata
    RenderUploadFileView();

    ImVec2 winSize = ImGui::GetContentRegionAvail();

    // Sidebar
    ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, winSize.y), true);
    ImGui::Text("Sidebar items...");
    ImGui::EndChild();

    ImGui::SameLine();

    // Splitter (trascinabile)
    ImGui::PushID("Splitter");
    ImGui::InvisibleButton("##splitter", ImVec2(5, winSize.y));

    // Quando il mouse è sopra lo splitter, cambia il cursore
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW); // doppia freccia orizzontale

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
