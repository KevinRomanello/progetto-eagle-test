#include "global.h"

global& global::get() {
    static global obj;
    return obj;
}