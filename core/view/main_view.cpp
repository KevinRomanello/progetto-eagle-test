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

// Variabile globale temporanea per tab attivo
int currentTabIndex = 0;

upload_file_controller uploadController;
auth_controller authController;

float sidebarWidth = 300.0f;

void RenderMainView() {
    // 1. Ottieni il "viewport" principale (l'intera finestra dell'OS)
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    // 2. Imposta la posizione e la dimensione della *prossima* finestra
    //    per riempire l'area di lavoro del viewport.
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    // 3. Imposta i flag per renderla una "root window" non interattiva
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;           // Vogliamo il menu bar
    window_flags |= ImGuiWindowFlags_NoResize;          // Niente ridimensionamento
    window_flags |= ImGuiWindowFlags_NoMove;            // Niente spostamento
    window_flags |= ImGuiWindowFlags_NoCollapse;        // Niente collasso
    window_flags |= ImGuiWindowFlags_NoSavedSettings;   // NON salvare la posizione in imgui.ini
    window_flags |= ImGuiWindowFlags_NoTitleBar;        // Rimuove la barra del titolo (opzionale)
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus; // Non venire avanti se cliccata

    // 4. Inizia la finestra. Nota "Main View##Root".
    //    Il "##Root" è un ID nascosto. Passiamo 'nullptr' per 'p_open'.
    ImGui::Begin("Main View##Root", nullptr, window_flags);

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    auto& state = global::get();

    ImGui::PushFont(state.fonts.Default);

    // --- 2. Menu Bar ---
    if (ImGui::BeginMenuBar()) {
        // FILE
        if (state.user.authenticated) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Load File...")) {
                    upload_file_controller::RequestOpenFile();
                }
                if (ImGui::MenuItem("Load File")) {
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

        // PROFILE
        if (state.user.authenticated) {
            if (ImGui::BeginMenu("Logout")) {
                auth_controller::RequestLogout();
                ImGui::EndMenu();
            }
        }else {
            if (ImGui::BeginMenu("Login")) {
                auth_controller::RequestLogin();
                ImGui::EndMenu();
            }
        }


        ImGui::EndMenuBar();
    }

    // --- 3. Rendering Popup (gestiscono da soli la visibilità) ---
    RenderLoginView();
    RenderUploadFileView();
    RenderUserManagementView();

    // --- 4. DEFINIZIONE AREA DI LAVORO ---
    ImVec2 winSize = ImGui::GetContentRegionAvail();

    // --- 5. Sidebar (Contenitore) ---
    ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, winSize.y), true);
    RenderFileListView();
    ImGui::EndChild();

    ImGui::SameLine();

    // --- 6. Splitter (La "barra per modificare") ---
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

    // --- 7. Content Area (Contenitore) ---
    ImGui::BeginChild("Content", ImVec2(0, winSize.y), true);

    RenderContentView();    // Delega il render alla view

    ImGui::EndChild();

    ImGui::PopFont();

    ImGui::End();
}