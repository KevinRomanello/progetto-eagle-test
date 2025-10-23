#pragma once
#include <string>

class auth_controller {
public:
    static bool login(const std::string& username, const std::string& password);
    static void RequestLogin();
    static void RequestLogout();
};