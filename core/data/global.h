#pragma once

#include "telemetry_data.h"
#include "user.h"
#include "AppState.h"
#include <vector>

class global {
public:
    AppState AppState;
    UserState user;

    static global& get();

private:
    global() {}
    global(const global&) = delete;
    void operator=(const global&) = delete;
};