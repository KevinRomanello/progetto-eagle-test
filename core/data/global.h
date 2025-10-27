#pragma once

#include "telemetry_data.h"
#include "user.h"
#include "AppState.h"
#include "Fonts.h"
#include <string>

class global {
public:
    AppState AppState;
    UserState user;
    Fonts fonts;

    // file db temporaneo (scelta brutta senza db)
    static const std::string USER_DB_FILE;

    // L'elenco di tutti gli utenti registrati (caricato da file)
    std::vector<UserData> userDatabase;

    static global& get();

private:
    global(){}
    global(const global&) = delete;
    void operator=(const global&) = delete;
};