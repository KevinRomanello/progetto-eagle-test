#pragma once
#include <string>

struct UserState {
    std::string username;
    bool authenticated = false;
};