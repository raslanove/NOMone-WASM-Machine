
///////////////////////////////////////////////////////
// Created by Omar El Sayyed on 5th of August 2021.
///////////////////////////////////////////////////////

#pragma once

#include <NTypes.h>

struct NCString_Interface {
    int32_t (*length)(const char* string);
    boolean (*startsWith)(const char* string, const char* value);
    boolean (*equals)(const char* string, const char* value);
    char* (*copy)(char* destination, const char* source); // Returns destination.
    int32_t (*parseInteger)(const char* string);
};

extern const struct NCString_Interface NCString;
