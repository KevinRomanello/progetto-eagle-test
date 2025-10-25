#include "user_management_view.h"
#include "../core/data/global.h"
#include "../controllers/user_management_controller.h"
#include "add_user_view.h"
#include "imgui.h"
#include <vector>
#include <iostream> // Per il tuo cout

void RenderUserManagementView() {
    auto& state = global::get();

    if (state.AppState.showUserManagementPopup) {
        ImGui::OpenPopup("Manage Users");
    }

    // --- QUESTA È LA SOLUZIONE ---
    // Diciamo a ImGui di forzare questa posizione e dimensione
    // AD OGNI FRAME (Always), ignorando il file .ini
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always);

    // Flag per non salvare MAI PIÙ la posizione di questa finestra
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::BeginPopupModal("Manage Users", &state.AppState.showUserManagementPopup, flags)) {

        // --- Pannello Sinistro (ORA LA LISTA) ---
        // Imposta la larghezza a 0 per farla auto-adattare, ma con uno splitter
        // Mettiamo una larghezza fissa per la lista, es. 330
        ImGui::BeginChild("UserListPane", ImVec2(330, -40), true);
        ImGui::Text("Users (excluding Admin)");
        ImGui::Separator();
        for (int i = 0; i < state.userDatabase.size(); ++i) {
            const auto& user = state.userDatabase[i];
            if (user.role == UserRole::ADMIN) continue;

            bool isSelected = (state.AppState.selectedUserIndexInDb == i);
            if (ImGui::Selectable(user.username.c_str(), isSelected)) {
                user_management_controller::SelectUser(i);
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // --- Pannello Destro (ORA I DETTAGLI) ---
        // (Usa ImVec2(0, ...) per fargli prendere lo spazio rimanente)
        ImGui::BeginChild("DetailsPane", ImVec2(0, -40), true);
        ImGui::Text("User Details");
        ImGui::Separator();
        if (state.AppState.selectedUserIndexInDb != -1) {
            UserData& selectedUser = state.userDatabase[state.AppState.selectedUserIndexInDb];
            const char* roles[] = { "USER", "ADVANCED", "ADMIN" };
            ImGui::Text("Username: %s", selectedUser.username.c_str());
            ImGui::Text("Password: %s", std::string(selectedUser.password.length(), '*').c_str());
            ImGui::Text("Role:     %s", roles[selectedUser.role]);
        } else {
            ImGui::Text("Select a user from the list.");
        }
        ImGui::EndChild();

        // Pulsanti in fondo
        if (ImGui::Button("Add User...")) {
            user_management_controller::RequestShowAddUserPopup();
        }
        ImGui::SameLine();
        bool userSelected = state.AppState.selectedUserIndexInDb != -1;
        ImGui::BeginDisabled(!userSelected);
        if (ImGui::Button("Delete Selected")) {
            user_management_controller::DeleteSelectedUser();
        }
        ImGui::EndDisabled();

        // Disegna il popup "Aggiungi" (così appare sopra)
        RenderAddUserView();

        ImGui::EndPopup();
    }
}