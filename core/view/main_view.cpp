#include "main_view.h"

#include <iostream>
#include <ostream>

#include "../data/global.h"
#include "imgui.h"
#include "login_view.h"
#include "upload_file_view.h"
#include "../controllers/upload_file_controller.h"
#include "../controllers/auth_controller.h"
#include "../controllers/user_management_controller.h"
#include "file_list_view.h"
#include "user_management_view.h"
#include "content_view.h"

upload_file_controller uploadController;
auth_controller authController;

// valore della sidebar dove ci sono i file aperti
float sidebarWidth = 300.0f;

void RenderMainView() {

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    // imposta dim e posizione della finestra
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    // flags della finestra
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;           // Vogliamo il menu bar
    window_flags |= ImGuiWindowFlags_NoResize;          // Niente ridimensionamento
    window_flags |= ImGuiWindowFlags_NoMove;            // Niente spostamento
    window_flags |= ImGuiWindowFlags_NoCollapse;        // Niente collasso
    window_flags |= ImGuiWindowFlags_NoSavedSettings;   // NON salvare la posizione in imgui.ini
    window_flags |= ImGuiWindowFlags_NoTitleBar;        // Rimuove la barra del titolo (opzionale)
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus; // Non venire avanti se cliccata

    ImGui::Begin("Main View##Root", nullptr, window_flags);

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    auto& state = global::get();

    ImGui::PushFont(state.fonts.Default);

    // menu bar
    if (ImGui::BeginMenuBar()) {
        // file
        if (state.user.authenticated) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Load File")) {
                    upload_file_controller::RequestOpenFile();
                }
                if (ImGui::MenuItem("Close File")) {
                    upload_file_controller::CloseFile();
                }
                ImGui::EndMenu();
            }
        }

        // menu visibile solo all'admin
        if (state.user.role == UserRole::ADMIN) {
            if (ImGui::BeginMenu("Users")) {
                if (ImGui::MenuItem("Manage Users...")) {
                    user_management_controller::RequestShowPopup();
                }
                ImGui::EndMenu();
            }
        }

        // login logout
        if (state.user.authenticated) {
            if (ImGui::MenuItem("Logout")) {
                auth_controller::RequestLogout();
            }
        }else {
            if (ImGui::MenuItem("Login")) {
                auth_controller::RequestLogin();
            }
        }


        ImGui::EndMenuBar();
    }

    // call alle funzioni per il rendering delle views
    RenderLoginView();
    RenderUploadFileView();
    RenderUserManagementView();

    // prende la grandezza della finestra
    ImVec2 winSize = ImGui::GetContentRegionAvail();

    // costruzione della sidebar
    ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, winSize.y), true);
    RenderFileListView();
    ImGui::EndChild();

    ImGui::SameLine();

    // costruzione dello splitter delle 2 colonne della mainview
    ImGui::PushID("Splitter");
    ImGui::InvisibleButton("##splitter", ImVec2(5, winSize.y));
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    if (ImGui::IsItemActive()) {
        sidebarWidth += ImGui::GetIO().MouseDelta.x;
        if (sidebarWidth < 100.0f) sidebarWidth = 100.0f;
        if (sidebarWidth > winSize.x - 100.0f) sidebarWidth = winSize.x - 100.0f;
    }
    ImGui::PopID();

    ImGui::SameLine();

    // area per i dati dei file caricati
    ImGui::BeginChild("Content", ImVec2(0, winSize.y), true);

    // chiama la funzione per renderizzare i dati
    RenderContentView();

    ImGui::EndChild();

    ImGui::PopFont();

    ImGui::End();
}