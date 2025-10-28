#pragma once
#include <string>

enum UserRole {
    USER,
    ADVANCED,
    ADMIN
};

// user caricato dal file db
struct UserData {
    std::string username;
    std::string password;
    UserRole role = UserRole::USER;
};

struct UserState {
    std::string username;
    bool authenticated = false;
    UserRole role = UserRole::USER;
};