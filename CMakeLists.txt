cmake_minimum_required(VERSION 3.21)
project(Project1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(IMGUI_FILE_DIALOG_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ImGuiFileDialog/ImGuiFileDialog.cpp
        core/utilities/csv_parser.cpp
        core/utilities/csv_parser.h
        core/data/telemetry_data.h
        core/data/user.h
        core/data/AppState.h
        core/controllers/file_list_controller.h
        core/controllers/file_list_controller.cpp
        core/view/file_list_view.cpp
        core/view/file_list_view.h
        core/view/table_view.h
        core/view/table_view.cpp
        core/view/content_view.h
        core/view/content_view.cpp
        core/controllers/user_management_controller.h
        core/controllers/user_management_controller.cpp
        core/view/add_user_view.h
        "core/view/add_user_view.cpp"
        core/view/add_user_view.cpp
        core/view/user_management_view.h
        core/view/user_management_view.cpp
        core/data/Fonts.h
)


set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread -g -rdynamic -std=c++1z -DGLEW_STATIC -DNDEBUG -w")

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/implot
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ImGuiFileDialog
)

FILE(GLOB imgui_srcs ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/*)
add_library(
    ${PROJECT_NAME}
    SHARED

    # Sources
    ${CMAKE_CURRENT_SOURCE_DIR}/core/app.cpp

    # GUI
    ${imgui_srcs}
        ${IMGUI_FILE_DIALOG_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_opengl2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/misc/cpp/imgui_stdlib.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/implot/implot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/implot/implot_items.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/implot/implot_demo.cpp
        core/main.cpp
        core/data/global.cpp
        core/data/global.h
        core/controllers/upload_file_controller.cpp
        core/controllers/upload_file_controller.h
        core/controllers/auth_controller.cpp
        core/controllers/auth_controller.h
        core/model/user.cpp
        core/model/user.h
        core/model/telemetry_data.cpp
        core/model/telemetry_data.h
        core/view/login_view.cpp
        core/view/login_view.h
        core/view/plot_view.cpp
        core/view/plot_view.h
        core/view/main_view.cpp
        core/view/main_view.h
        core/view/upload_file_view.h
        core/view/upload_file_view.cpp
)

target_link_libraries(${PROJECT_NAME}
    glfw
    OpenGL::GL
    GLEW::GLEW
    dl
)
if(NOT APPLE)
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_executable(project_1 core/main.cpp
        )
target_link_libraries(project_1 ${PROJECT_NAME})
