
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 4th of August 2021.
//////////////////////////////////////////////////////

#include <NSystemUtils.h>

#include <stdlib.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

static int64_t time=0;

static void* nMalloc(int32_t size) {
    return malloc(size);
}

static void nFree(void* address) {
    free(address);
}

static void* nMemset(void* address, int value, int32_t length) {
    return memset(address, value, length);
}

static void* nMemcpy(void* dest, const void* src, int32_t length) {
    return memcpy(dest, src, length);
}

static void nLog(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

static void getTime(int64_t* outTimeSeconds, int64_t* outTimeNanos) {
    *outTimeSeconds = ++time;
    *outTimeNanos = 0;
}

const struct NSystemUtils_Interface NSystemUtils = {
    .malloc = nMalloc,
    .free = nFree,
    .memset = nMemset,
    .memcpy = nMemcpy,
    .log = nLog,
    .getTime = getTime
};