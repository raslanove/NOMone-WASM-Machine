
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 2nd of August 2021.
//////////////////////////////////////////////////////

#pragma once

#include <NTime.h>
#include <NTypes.h>

#define NERROR_MAX_MESSAGE_LENGTH 256
#define NERROR(tag, errorMessageFormat, ...) NError.pushAndPrintError(tag, errorMessageFormat, ##__VA_ARGS__)

struct NError {
    char message[NERROR_MAX_MESSAGE_LENGTH];
    struct NTime time;
};

struct NError_Interface {
    void (*terminate)();  // Call once at the program end.
    int32_t (*observeErrors)();  // Returns current position in error stack.
    struct NError* (*pushError)(const char* errorMessageFormat, ...);
    struct NError* (*pushAndPrintError)(const char* tag, const char* errorMessageFormat, ...);
    struct NVector* (*popErrors)(int32_t stackPosition);  // 0 if no error occurred since then, a vector of NErrors otherwise.
    int32_t (*popDestroyAndFreeErrors)(int32_t stackPosition);
};

extern const struct NError_Interface NError;
