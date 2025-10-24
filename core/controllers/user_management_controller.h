#pragma once
#include <string>
#include "../core/data/user.h"

class user_management_controller {
public:
    // Gestione popup
    static void RequestShowPopup();
    static void RequestClosePopup();
    static void RequestShowAddUserPopup();
    static void RequestCloseAddUserPopup();

    // Azioni
    static void SelectUser(int db_index);
    static void DeleteSelectedUser();
    static bool AddUser(const std::string& user, const std::string& pass, UserRole role);
    
    // Funzione di salvataggio
    static void SaveUserDatabase();
};