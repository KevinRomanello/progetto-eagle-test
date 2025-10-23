#include "login_view.h"

#include <iostream>
#include <ostream>

#include "../controllers/auth_controller.h"
#include "../core/global.h"
#include "imgui.h"

void RenderLoginView() {
    auto& state = global::get();

    if (!state.AppState.showLoginPopup)
        return;

    if (ImGui::Begin("Authenticate", &state.AppState.showLoginPopup, ImGuiWindowFlags_AlwaysAutoResize)) {
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


        ImGui::End();
    }

}




