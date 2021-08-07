
///////////////////////////////////////////////////////
// Created by Omar El Sayyed on 5th of August 2021.
///////////////////////////////////////////////////////

#pragma once

#include <NTypes.h>

struct NCString_Interface {
    boolean (*startsWith)(const char* string, const char* value);  // True if successful.
    char* (*copy)(char* destination, const char* source); // Returns destination.
};

extern const struct NCString_Interface NCString;
