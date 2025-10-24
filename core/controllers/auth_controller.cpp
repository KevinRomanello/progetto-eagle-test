#include "auth_controller.h"
#include "../core/data/global.h"
#include "../controllers/user_management_controller.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "../data/global.h"

void auth_controller::LoadUserDatabase() {
    auto& db = global::get().userDatabase;
    db.clear();
    std::ifstream file(global::USER_DB_FILE);

    // CASO 1: Il file non esiste (primo avvio)
    if (!file.is_open()) {
        std::cout << "File '" << global::USER_DB_FILE << "' non trovato. Creazione..." << std::endl;

        db.push_back(UserData{"admin", "1234", UserRole::ADMIN});
        db.push_back(UserData{"advanced", "1234", UserRole::ADVANCED});
        db.push_back(UserData{"user", "1234", UserRole::USER});

        // Salva subito il file di default
        user_management_controller::SaveUserDatabase();
        return;
    }

    // CASO 2: Il file esiste, lo leggiamo
    std::cout << "Caricamento database utenti da '" << global::USER_DB_FILE << "'..." << std::endl;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string segment;
        UserData user;
        try {
            std::getline(ss, segment, ','); user.username = segment;
            std::getline(ss, segment, ','); user.password = segment;
            std::getline(ss, segment, ','); user.role = static_cast<UserRole>(std::stoi(segment));
            db.push_back(user);
        } catch (const std::exception& e) {
            std::cerr << "Errore nel parsare user.dat, riga saltata: " << e.what() << std::endl;
        }
    }
    file.close();
}


void auth_controller::RequestLogin() {
    global::get().AppState.showLoginPopup = true;
}

bool auth_controller::login(const std::string& username, const std::string& password) {
    auto& state = global::get();

    // Cerca l'utente nel database in memoria
    for (const auto& user : state.userDatabase) {
        if (user.username == username && user.password == password) {
            // Trovato! Scrive i dati sull'utente corrente
            state.user.username = user.username;
            state.user.role = user.role;
            state.user.authenticated = true;

            state.AppState.showLoginPopup = false;
            return true;
        }
    }
    return false; // Non trovato
}
void auth_controller::RequestLogout() {
    auto& state = global::get();
    state.user.authenticated = false;
    state.user.username = "";
    state.user.role = UserRole::USER; // resetta il ruolo al logout
    state.AppState.loadedFiles.clear();
}

