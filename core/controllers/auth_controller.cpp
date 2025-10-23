#include "auth_controller.h"
#include "../core/global.h"

void auth_controller::RequestLogin() {
    global::get().AppState.showLoginPopup = true;
}

bool auth_controller::login(const std::string& username, const std::string& password) {
    auto& state = global::get();
    if (username == "admin" && password == "1234") {
        state.user.username = username;
        state.user.authenticated = true;
        state.AppState.showLoginPopup = false;
        return true;
    }
    // Qui potresti impostare un messaggio di errore
    return false;
}

void auth_controller::RequestLogout() {
    auto& state = global::get();
    state.user.authenticated = false;
    state.AppState.loadedFiles.clear();
}

