#pragma once
#include <string>

class auth_controller {
public:
    // Carica il DB da file all'avvio
    static void LoadUserDatabase();
    static bool login(const std::string& username, const std::string& password);
    static void RequestLogin();
    static void RequestLogout();
};