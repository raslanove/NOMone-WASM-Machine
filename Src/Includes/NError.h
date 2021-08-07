
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 2nd of August 2021.
//////////////////////////////////////////////////////

#pragma once

#include <NTime.h>
#include <NTypes.h>

#define NERROR(text) NError.pushAndPrintError(text)

struct NError {
    char message[256];
    struct NTime time;
};

struct NError_Interface {
    void (*terminate)();  // Call once at the program end.
    int32_t (*observeErrors)();  // Returns current position in error stack.
    struct NError* (*pushError)(const char* errorMessage);
    struct NError* (*pushAndPrintError)(const char* tag, const char* errorMessage);
    struct NVector* (*popErrors)(int32_t stackPosition);  // 0 if no error occurred since then, a vector of NErrors otherwise.
};

extern const struct NError_Interface NError;