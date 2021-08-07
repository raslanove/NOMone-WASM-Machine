
//////////////////////////////////////////////////////
// Created by Omar El Sayyed on 4th of August 2021.
//////////////////////////////////////////////////////

#include <NSystemUtils.h>

#include <stdlib.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <vector>
#include <android/log.h>

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

#define LOG_IMPLEMENTATION(logLevel, logTag) \
	static std::vector<char> message; \
	va_list arguments; \
	va_start(arguments, format); \
	int textLength = vsnprintf(&message[0], message.size(), format, arguments) + 1; \
	va_end(arguments); \
	if (textLength > message.size()) { \
		message.resize(textLength); \
		va_start(arguments, format); \
		vsnprintf(&message[0], textLength, format, arguments); \
		va_end(arguments); \
	} \
	__android_log_print(logLevel, logTag, "%s", &message[0]);

void logI(const char *logTag, const char *format, ...) { LOG_IMPLEMENTATION(ANDROID_LOG_INFO, logTag); }
void logW(const char *logTag, const char *format, ...) { LOG_IMPLEMENTATION(ANDROID_LOG_WARN, logTag); }
void logE(const char *logTag, const char *format, ...) { LOG_IMPLEMENTATION(ANDROID_LOG_ERROR, logTag); }

static void getTime(int64_t* outTimeSeconds, int64_t* outTimeNanos) {
    *outTimeSeconds = ++currentTime;
    *outTimeNanos = 0;
}

const struct NSystemUtils_Interface NSystemUtils = {
    .malloc = nMalloc,
    .free = nFree,
    .memset = nMemset,
    .memcpy = nMemcpy,
    .logI = logI,
	.logW = logW,
	.logE = logE,
    .getTime = getTime
};
