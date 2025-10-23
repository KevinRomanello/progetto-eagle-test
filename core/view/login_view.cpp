#include "login_view.h"

#include <iostream>
#include <ostream>

#include "../controllers/auth_controller.h"
#include "../data/global.h"
#include "imgui.h"

void RenderLoginView() {
    auto& state = global::get();

    if (state.AppState.showLoginPopup) {
        ImGui::OpenPopup("Authenticate");
    }

    // per centrarla
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Authenticate", &state.AppState.showLoginPopup, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char username[64];
        static char password[64];

        ImGui::InputText("Username", username, 64);
        ImGui::InputText("Password", password, 64, ImGuiInputTextFlags_Password);

        if (ImGui::Button("Login")) {
            if (auth_controller::login(username, password)) {
                std::cout << "accesso effettuato" << std::endl;
            }
            username[0] = '\0';
            password[0] = '\0';
        }

        if (!state.AppState.showLoginPopup) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

}




