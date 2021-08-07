
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 4th of August 2021.
//////////////////////////////////////////////////////

#pragma once

#include <NTypes.h>

#ifndef VERBOSE
#define VERBOSE 1
#endif
inline void dummyFunction() {}
#define NLOGI(tag, format, ...) ((VERBOSE) ? NSystemUtils.logI(tag, format, ##__VA_ARGS__) : dummyFunction())
#define NLOGW(tag, format, ...) ((VERBOSE) ? NSystemUtils.logW(tag, format, ##__VA_ARGS__) : dummyFunction())
#define NLOGE(tag, format, ...) ((VERBOSE) ? NSystemUtils.logE(tag, format, ##__VA_ARGS__) : dummyFunction())

struct NSystemUtils_Interface {
    void* (*malloc)(int32_t size);
    void (*free)(void* address);
    void* (*memset)(void* address, int value, int32_t length); // Returns address.
    void* (*memcpy)(void* dest, const void* src, int32_t length); // Returns destination.
    void (*logI)(const char* tag, const char* format, ...);
    void (*logW)(const char* tag, const char* format, ...);
    void (*logE)(const char* tag, const char* format, ...);
    void (*getTime)(int64_t* outTimeSeconds, int64_t* outTimeNanos);
};

extern const struct NSystemUtils_Interface NSystemUtils;