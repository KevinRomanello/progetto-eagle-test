#pragma once
#include <string>
#include <vector>

class global {
public:
    enum class Role { Guest, User, Maintainer, Admin };

    struct User {
        std::string username;
        Role role = Role::Guest;
        bool authenticated = false;
    } user;

    struct TelemetryData {
        std::vector<float> time;
        std::vector<float> speed;
        std::vector<float> throttle;
        std::vector<float> brake;
    } data;

    static global& get();
private:
    global() = default;
};