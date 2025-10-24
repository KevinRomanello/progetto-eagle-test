#include "global.h"
const std::string global::USER_DB_FILE = "../core/data/userData.csv";

global& global::get() {
    static global obj;
    return obj;
}