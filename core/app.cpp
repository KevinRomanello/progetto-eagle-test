#include "app.h"
#include <string>
#include <dispatch/dispatch.h>
#include "view/login_view.h"
#include "view/main_view.h"

#include "implot/implot.h"

void app_render() {
    //ImGui::ShowDemoWindow();
    //ImPlot::ShowDemoWindow();
    RenderMainView();
}

