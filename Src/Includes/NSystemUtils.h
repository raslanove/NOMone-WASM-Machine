
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

#define NTCOLOR(color) NTerminalColor.color

struct NSystemUtils_Interface {
    void* (*malloc)(int32_t size);
    void (*free)(void* address);
    void* (*memset)(void* address, int value, int32_t length); // Returns address.
    void* (*memcpy)(void* dest, const void* src, int32_t length); // Returns destination.
    void (*logI)(const char* tag, const char* format, ...);
    void (*logW)(const char* tag, const char* format, ...);
    void (*logE)(const char* tag, const char* format, ...);
    void (*getTime)(int64_t* outTimeSeconds, int64_t* outTimeNanos);
    boolean (*isNaN)(double value);
    boolean (*isInf)(double value);
};

extern const struct NSystemUtils_Interface NSystemUtils;

struct NTerminalColor {
    // Thanks to this answer: https://stackoverflow.com/a/51944613/1942069

    // Color end string, color reset,
    const char* const RESET;

    // Regular Colors. Normal color; no bold; background color etc;
    const char* const BLACK;
    const char* const RED;
    const char* const GREEN;
    const char* const YELLOW;
    const char* const BLUE;
    const char* const MAGENTA;
    const char* const CYAN;
    const char* const WHITE;

    // Bold;
    const char* const BLACK_BOLD;
    const char* const RED_BOLD;
    const char* const GREEN_BOLD;
    const char* const YELLOW_BOLD;
    const char* const BLUE_BOLD;
    const char* const MAGENTA_BOLD;
    const char* const CYAN_BOLD;
    const char* const WHITE_BOLD;

    // Underline;
    const char* const BLACK_UNDERLINED;
    const char* const RED_UNDERLINED;
    const char* const GREEN_UNDERLINED;
    const char* const YELLOW_UNDERLINED;
    const char* const BLUE_UNDERLINED;
    const char* const MAGENTA_UNDERLINED;
    const char* const CYAN_UNDERLINED;
    const char* const WHITE_UNDERLINED;

    // Background;
    const char* const BLACK_BACKGROUND;
    const char* const RED_BACKGROUND;
    const char* const GREEN_BACKGROUND;
    const char* const YELLOW_BACKGROUND;
    const char* const BLUE_BACKGROUND;
    const char* const MAGENTA_BACKGROUND;
    const char* const CYAN_BACKGROUND;
    const char* const WHITE_BACKGROUND;

    // High Intensity;
    const char* const BLACK_BRIGHT;
    const char* const RED_BRIGHT;
    const char* const GREEN_BRIGHT;
    const char* const YELLOW_BRIGHT;
    const char* const BLUE_BRIGHT;
    const char* const MAGENTA_BRIGHT;
    const char* const CYAN_BRIGHT;
    const char* const WHITE_BRIGHT;

    // Bold High Intensity;
    const char* const BLACK_BOLD_BRIGHT;
    const char* const RED_BOLD_BRIGHT;
    const char* const GREEN_BOLD_BRIGHT;
    const char* const YELLOW_BOLD_BRIGHT;
    const char* const BLUE_BOLD_BRIGHT;
    const char* const MAGENTA_BOLD_BRIGHT;
    const char* const CYAN_BOLD_BRIGHT;
    const char* const WHITE_BOLD_BRIGHT;

    // High Intensity backgrounds;
    const char* const BLACK_BACKGROUND_BRIGHT;
    const char* const RED_BACKGROUND_BRIGHT;
    const char* const GREEN_BACKGROUND_BRIGHT;
    const char* const YELLOW_BACKGROUND_BRIGHT;
    const char* const BLUE_BACKGROUND_BRIGHT;
    const char* const MAGENTA_BACKGROUND_BRIGHT;
    const char* const CYAN_BACKGROUND_BRIGHT;
    const char* const WHITE_BACKGROUND_BRIGHT;

    // Symantec colors,
    const char* const STREAM_DEFAULT;
    const char* const ERROR;
    const char* const WARNING;
    const char* const HIGHLIGHT;
    const char* const DANGER;
};
extern const struct NTerminalColor NTerminalColor;