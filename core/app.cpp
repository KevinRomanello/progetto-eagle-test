#include "app.h"
#include <string>
#include <dispatch/dispatch.h>

#include "implot/implot.h"



struct user_auth {
    std::string pass =  "pass";
    std::string user = "admin";
    bool authenticated = false;
};

user_auth Localuser;

void reload() {
    ImGui::Begin("Reload");

    static char username[64] = "";
    static char password[128] = "";

    ImGui::InputText("Username", username, 64);
    ImGui::InputText("Password", password, 128, ImGuiInputTextFlags_Password);

    if (Localuser.authenticated) {
        ImGui::Text("You are logged in.");
    }

    if (ImGui::Button("Accedi")) {
        if (Localuser.pass == password && Localuser.user == username) {
            Localuser.authenticated = true;
        }
    }

    ImGui::End();
}

void app_render() {

  //ImGui::ShowDemoWindow();
  //ImPlot::ShowDemoWindow();
  reload();
}

