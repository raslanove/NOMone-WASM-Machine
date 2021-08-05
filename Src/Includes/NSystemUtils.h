
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 4th of August 2021.
//////////////////////////////////////////////////////

#pragma once

#include <NTypes.h>

#define NLOG(text) NSystemUtils.log(text)

struct NSystemUtils_Interface {
    void* (*malloc)(int32_t size);
    void (*free)(void* address);
    void* (*memset)(void* address, int value, int32_t length); // Returns address.
    void* (*memcpy)(void* dest, const void* src, int32_t length); // Returns destination.
    void (*log)(const char* format, ...);
    void (*getTime)(int64_t* outTimeSeconds, int64_t* outTimeNanos);
};

extern const struct NSystemUtils_Interface NSystemUtils;