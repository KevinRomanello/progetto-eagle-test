#include "login_view.h"
#include "../controllers/auth_controller.h"
#include "../core/global.h"
#include "imgui.h"

void RenderLoginUI() {
    auto& state = global::get();
    static char username[64];
    static char password[64];

    ImGui::InputText("Username", username, 64);
    ImGui::InputText("Password", password, 64, ImGuiInputTextFlags_Password);

    if (ImGui::Button("Login")) {
        auth_controller::login(username, password);
    }
}