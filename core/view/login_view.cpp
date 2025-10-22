#include "login_view.h"
#include "../controllers/auth_controller.h"
#include "../core/global.h"
#include "imgui.h"

void OnCloseSettings(global *state) {
    state->showLoginPopup = false;
}

void RenderLoginView() {
    auto& state = global::get();

    if (!state.showLoginPopup)
        return;

    if (ImGui::Begin("Authenticate", &state.showLoginPopup, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char username[64];
        static char password[64];

        ImGui::InputText("Username", username, 64);
        ImGui::InputText("Password", password, 64, ImGuiInputTextFlags_Password);

        if (ImGui::Button("Login")) {
            if (auth_controller::login(username, password)) {
                state.showLoginPopup = false;
            }
            username[0] = '\0';
            password[0] = '\0';
        }


        ImGui::End();
    }

}




