
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 26th of July 2021.
//////////////////////////////////////////////////////

#pragma once

#include <NByteVector.h>

struct NString {
    struct NByteVector string;
};

struct NString_Interface {
    struct NString* (*initialize)(struct NString* outputString);
    void (*destroy)(struct NString* outputString);
    struct NString* (*create)(const char* format, ...);
};

extern const struct NString_Interface NString;
