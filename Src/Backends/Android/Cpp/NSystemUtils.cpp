
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 4th of August 2021.
//////////////////////////////////////////////////////

#include <NSystemUtils.h>
#include <NString.h>
#include <NError.h>

#include <stdlib.h>
#include <memory.h>
#include <stdarg.h>
#include <android/log.h>
#include <math.h>

static int64_t currentTime=0;

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

#define LOG_IMPLEMENTATION(logLevel, color) \
    struct NString* formattedString = NString.create(""); \
    va_list vaList; \
    va_start(vaList, format); \
    int32_t errorsStart = NError.observeErrors(); \
    NString.vAppend(formattedString, format, vaList); \
    int32_t errorsCount = NError.observeErrors() - errorsStart; \
    va_end(vaList); \
    if (!errorsCount) { \
        struct NString* coloredString = NString.replace(NString.get(formattedString), NTCOLOR(STREAM_DEFAULT), color); \
        __android_log_print(logLevel, logTag ? logTag : "", "%s%s%s", color, NString.get(coloredString), NTCOLOR(RESET)); \
        NString.destroyAndFree(coloredString); \
    } \
    NString.destroyAndFree(formattedString)

void logI(const char *logTag, const char *format, ...) { LOG_IMPLEMENTATION(ANDROID_LOG_INFO , NTCOLOR(RESET)); }
void logW(const char *logTag, const char *format, ...) { LOG_IMPLEMENTATION(ANDROID_LOG_WARN , NTCOLOR(WARNING)); }
void logE(const char *logTag, const char *format, ...) { LOG_IMPLEMENTATION(ANDROID_LOG_ERROR, NTCOLOR(ERROR)); }

static void getTime(int64_t* outTimeSeconds, int64_t* outTimeNanos) {
    *outTimeSeconds = ++currentTime;
    *outTimeNanos = 0;
}

static boolean isNaN(double value) { return (isnan(value) ? True : False); }
static boolean isInf(double value) { return (isinf(value) ? True : False); }

const struct NSystemUtils_Interface NSystemUtils = {
    .malloc = nMalloc,
    .free = nFree,
    .memset = nMemset,
    .memcpy = nMemcpy,
    .logI = logI,
    .logW = logW,
    .logE = logE,
    .getTime = getTime,
    .isNaN = isNaN,
    .isInf = isInf
};

const struct NTerminalColor NTerminalColor = {
    .RESET = "\033[0m",

    .BLACK   = "\033[0;30m",
    .RED     = "\033[0;31m",
    .GREEN   = "\033[0;32m",
    .YELLOW  = "\033[0;33m",
    .BLUE    = "\033[0;34m",
    .MAGENTA = "\033[0;35m",
    .CYAN    = "\033[0;36m",
    .WHITE   = "\033[0;37m",

    .BLACK_BOLD   = "\033[1;30m",
    .RED_BOLD     = "\033[1;31m",
    .GREEN_BOLD   = "\033[1;32m",
    .YELLOW_BOLD  = "\033[1;33m",
    .BLUE_BOLD    = "\033[1;34m",
    .MAGENTA_BOLD = "\033[1;35m",
    .CYAN_BOLD    = "\033[1;36m",
    .WHITE_BOLD   = "\033[1;37m",

    .BLACK_UNDERLINED   = "\033[4;30m",
    .RED_UNDERLINED     = "\033[4;31m",
    .GREEN_UNDERLINED   = "\033[4;32m",
    .YELLOW_UNDERLINED  = "\033[4;33m",
    .BLUE_UNDERLINED    = "\033[4;34m",
    .MAGENTA_UNDERLINED = "\033[4;35m",
    .CYAN_UNDERLINED    = "\033[4;36m",
    .WHITE_UNDERLINED   = "\033[4;37m",

    .BLACK_BACKGROUND   = "\033[40m",
    .RED_BACKGROUND     = "\033[41m",
    .GREEN_BACKGROUND   = "\033[42m",
    .YELLOW_BACKGROUND  = "\033[43m",
    .BLUE_BACKGROUND    = "\033[44m",
    .MAGENTA_BACKGROUND = "\033[45m",
    .CYAN_BACKGROUND    = "\033[46m",
    .WHITE_BACKGROUND   = "\033[47m",

    .BLACK_BRIGHT   = "\033[0;90m",
    .RED_BRIGHT     = "\033[0;91m",
    .GREEN_BRIGHT   = "\033[0;92m",
    .YELLOW_BRIGHT  = "\033[0;93m",
    .BLUE_BRIGHT    = "\033[0;94m",
    .MAGENTA_BRIGHT = "\033[0;95m",
    .CYAN_BRIGHT    = "\033[0;96m",
    .WHITE_BRIGHT   = "\033[0;97m",

    .BLACK_BOLD_BRIGHT   = "\033[1;90m",
    .RED_BOLD_BRIGHT     = "\033[1;91m",
    .GREEN_BOLD_BRIGHT   = "\033[1;92m",
    .YELLOW_BOLD_BRIGHT  = "\033[1;93m",
    .BLUE_BOLD_BRIGHT    = "\033[1;94m",
    .MAGENTA_BOLD_BRIGHT = "\033[1;95m",
    .CYAN_BOLD_BRIGHT    = "\033[1;96m",
    .WHITE_BOLD_BRIGHT   = "\033[1;97m",

    .BLACK_BACKGROUND_BRIGHT   = "\033[0;100m",
    .RED_BACKGROUND_BRIGHT     = "\033[0;101m",
    .GREEN_BACKGROUND_BRIGHT   = "\033[0;102m",
    .YELLOW_BACKGROUND_BRIGHT  = "\033[0;103m",
    .BLUE_BACKGROUND_BRIGHT    = "\033[0;104m",
    .MAGENTA_BACKGROUND_BRIGHT = "\033[0;105m",
    .CYAN_BACKGROUND_BRIGHT    = "\033[0;106m",
    .WHITE_BACKGROUND_BRIGHT   = "\033[0;107m",

    .STREAM_DEFAULT = "\033NOMoneSD",
    .ERROR = "\033[0;96m",      // CYAN_BRIGHT
    .WARNING = "\033[1;94m",    // BLUE_BOLD_BRIGHT
    .HIGHLIGHT = "\033[0;34m",  // BLUE
    .DANGER = "\033[1;96m"      // CYAN_BOLD_BRIGHT
};