
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 26th of July 2021.
//////////////////////////////////////////////////////

#pragma once

#include <NTypes.h>

struct NString_Interface {
    boolean (*startsWith)(const char* string, const char* value);  // True if successful.
    char* (*copy)(char* destination, const char* source); // Returns destination.
};

extern const struct NString_Interface NString;