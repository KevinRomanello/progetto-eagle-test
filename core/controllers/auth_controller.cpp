#include "auth_controller.h"
#include <iostream>

#include "../data/global.h"

void auth_controller::RequestLogin() {
    global::get().AppState.showLoginPopup = true;
}

bool auth_controller::login(const std::string& username, const std::string& password) {
    auto& state = global::get();

    // --- Database utenti hard-coded ---
    // chiaramente è temporaneo ma è facilmente upgradabile con l'aggiunta di un db
    UserRole roleToSet;
    bool loginSuccess = false;

    if (username == "admin" && password == "1234") {
        roleToSet = UserRole::ADMIN;
        loginSuccess = true;
    }
    else if (username == "advanced" && password == "1234") {
        roleToSet = UserRole::ADVANCED;
        loginSuccess = true;
    }
    else if (username == "user" && password == "1234") {
        roleToSet = UserRole::USER;
        loginSuccess = true;
    }
    // --- fine database ---

    if (loginSuccess) {
        state.user.username = username;
        state.user.authenticated = true;
        state.user.role = roleToSet; // assegna il ruolo
        state.AppState.showLoginPopup = false;
        return true;
    }

    return false;
}
void auth_controller::RequestLogout() {
    auto& state = global::get();
    state.user.authenticated = false;
    state.user.username = "";
    state.user.role = UserRole::USER; // resetta il ruolo al logout
    state.AppState.loadedFiles.clear();
}

