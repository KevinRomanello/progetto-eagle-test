#include "add_user_view.h"

#include <iostream>
#include <ostream>

#include "../core/data/global.h"
#include "../controllers/user_management_controller.h"
#include "imgui.h"

void RenderAddUserView() {
    auto& state = global::get();
    
    if (state.AppState.showAddUserPopup) {
        ImGui::OpenPopup("Add New User");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Add New User", &state.AppState.showAddUserPopup, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char username[64] = "";
        static char password[64] = "";
        static int selectedRoleIndex = 0; // 0=USER, 1=ADVANCED

        const char* roles[] = { "USER", "ADVANCED" };
        
        ImGui::InputText("Username", username, 64);
        ImGui::InputText("Password", password, 64);
        ImGui::Combo("Role", &selectedRoleIndex, roles, IM_ARRAYSIZE(roles));

        if (ImGui::Button("Add")) {
            UserRole role = (selectedRoleIndex == 0) ? UserRole::USER : UserRole::ADVANCED;
            if (user_management_controller::AddUser(username, password, role)) {
                username[0] = '\0';
                password[0] = '\0';
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            user_management_controller::RequestCloseAddUserPopup();
            username[0] = '\0';
            password[0] = '\0';
        }

        ImGui::EndPopup();
    }
}