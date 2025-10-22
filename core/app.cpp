#include "app.h"
#include <string>
#include <dispatch/dispatch.h>
#include "view/login_view.h"
#include "view/main_view.h"

#include "implot/implot.h"

struct user_auth {
    std::string pass =  "pass";
    std::string user = "admin";
    bool authenticated = false;
};

user_auth Localuser;

void reload() {
    ImGui::Begin("Authenticate");

    RenderLoginView();


    ImGui::End();
}



void app_render() {
    //ImGui::ShowDemoWindow();
    //ImPlot::ShowDemoWindow();
    //reload();
    RenderMainView();
}

