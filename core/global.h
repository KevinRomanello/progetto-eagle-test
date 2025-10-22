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

    struct TelemetryFile {
        std::string name;       // nome file (es. acceleration.csv)
        std::string type;       // es. "acceleration" o "skidpad"

        // Colonne CSV
        std::vector<float> time;
        std::vector<float> accX;
        std::vector<float> accY;
        std::vector<float> accZ;

        bool parsed = false;
    } TelemetryFIle;

    struct AppState {
        bool showLoginPopup = false;  // flag per mostrare finestra di log in
        std::vector<TelemetryFile> loadedFiles; //lista di file caricati
    } AppState;



    static global& get();
private:
    global() = default;
};