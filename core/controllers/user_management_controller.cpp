#include "user_management_controller.h"
#include "../core/data/global.h"
#include <iostream>
#include <fstream>

// --- FUNZIONE DI SALVATAGGIO ---
void user_management_controller::SaveUserDatabase() {
    auto& db = global::get().userDatabase;
    std::ofstream file(global::USER_DB_FILE); 

    if (!file.is_open()) {
        std::cerr << "ERRORE CRITICO: Impossibile salvare " << global::USER_DB_FILE << std::endl;
        return;
    }
    for (const auto& user : db) {
        file << user.username << ","
             << user.password << ","
             << static_cast<int>(user.role)
             << "\n";
    }
    file.close();
    std::cout << "Database utenti salvato." << std::endl;
}

// --- Funzioni di gestione popup ---
void user_management_controller::RequestShowPopup() {
    global::get().AppState.showUserManagementPopup = true;
    global::get().AppState.selectedUserIndexInDb = -1; 
}
void user_management_controller::RequestClosePopup() {
    global::get().AppState.showUserManagementPopup = false;
}
void user_management_controller::RequestShowAddUserPopup() {
    global::get().AppState.showAddUserPopup = true;
}
void user_management_controller::RequestCloseAddUserPopup() {
    global::get().AppState.showAddUserPopup = false;
}

// --- Funzioni di Azione ---
void user_management_controller::SelectUser(int db_index) {
    global::get().AppState.selectedUserIndexInDb = db_index;
}

void user_management_controller::DeleteSelectedUser() {
    auto& state = global::get();
    int index = state.AppState.selectedUserIndexInDb;

    if (index < 0 || index >= state.userDatabase.size()) return;
    if (state.userDatabase[index].role == UserRole::ADMIN) {
        std::cerr << "ERRORE: Impossibile eliminare l'amministratore!" << std::endl;
        return;
    }

    state.userDatabase.erase(state.userDatabase.begin() + index);
    state.AppState.selectedUserIndexInDb = -1; 

    SaveUserDatabase(); // Salva le modifiche
}

bool user_management_controller::AddUser(const std::string& user, const std::string& pass, UserRole role) {
    if (user.empty() || pass.empty()) return false;
    
    auto& db = global::get().userDatabase;
    for (const auto& u : db) {
        if (u.username == user) {
            std::cerr << "Username già esistente" << std::endl;
            return false;
        }
    }

    db.push_back(UserData{user, pass, role});
    SaveUserDatabase(); // Salva le modifiche
    RequestCloseAddUserPopup();
    return true;
}