#pragma once
#include <string>

enum UserRole {
    USER,
    ADVANCED,
    ADMIN
};

struct UserState {
    std::string username;
    bool authenticated = false;

    //    Di default, chiunque (anche se non loggato) è un 'USER'
    //    con authenticated = false.
    UserRole role = UserRole::USER;
};