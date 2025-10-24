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
    std::string password; // Dobbiamo memorizzare la password
    UserRole role = UserRole::USER;
};

struct UserState {
    std::string username;
    bool authenticated = false;
    //    Di default, chiunque (anche se non loggato) è un 'USER'
    //    con authenticated = false.
    UserRole role = UserRole::USER;
};