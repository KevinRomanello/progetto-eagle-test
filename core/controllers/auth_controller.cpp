#include "auth_controller.h"
#include "../core/global.h"

bool auth_controller::login(const std::string& username, const std::string& password) {
    auto& state = global::get();
    if (username == "admin" && password == "1234") {
        state.user.username = username;
        state.user.authenticated = true;
        return true;
    }
    return false;
}

void auth_controller::logout() {
    global::get().user.authenticated = false;
}