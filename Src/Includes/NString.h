
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 26th of July 2021.
//////////////////////////////////////////////////////

#pragma once

#include <NByteVector.h>
#include <NVarArgs.h>

struct NString {
    struct NByteVector string;
};

struct NString_Interface {
    struct NString* (*initialize)(struct NString* outputString);
    void (*destroy)(struct NString* string);
    void (*destroyAndFree)(struct NString* string);

    struct NString* (*vAppend)(struct NString* outString, const char* format, va_list vaList);
    struct NString* (*append)(struct NString* outString, const char* format, ...);
    struct NString* (*set)(struct NString* outString, const char* format, ...);
    const char* (*get)(struct NString* string);
    struct NString* (*create)(const char* format, ...);
    struct NString* (*replace)(const char* textToBeSearched, const char* textToBeRemoved, const char* textToBeInserted);
    int32_t (*length)(struct NString* string);
};

extern const struct NString_Interface NString;
