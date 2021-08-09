
#include <NString.h>
#include <NError.h>
#include <NSystemUtils.h>

static struct NString* initialize(struct NString* outputString) {
    NByteVector.create(4, &outputString->string);
    NByteVector.pushBack(&outputString->string, 0);
    return outputString;
}

static void destroy(struct NString* string) {
    NByteVector.destroy(&(string->string));
}

static void destroyAndFree(struct NString* string) {
    NByteVector.destroy(&(string->string));
    NSystemUtils.free(string);
}

static struct NString* vAppend(struct NString* outString, const char* format, va_list vaList) {

    // Pop the termination zero,
    struct NByteVector *outVector = &outString->string;
    char tempChar;
    NByteVector.popBack(outVector, &tempChar);

    // Parse the format string,
    int32_t index=0;
    char currentChar;
    while (currentChar = format[index++]) {

        // Regular characters,
        if (currentChar!='%') {
            NByteVector.pushBack(outVector, currentChar);
            continue;
        }

        // Found a '%',
        currentChar = format[index++];
        switch(currentChar) {
            case 's': {
                const char* sourceString = va_arg(vaList, const char*);

                int32_t stringIndex = 0;
                char currentChar;
                while (currentChar = sourceString[stringIndex++]) {
                    NByteVector.pushBack(outVector, currentChar);
                }
            }
            continue;
            default:
                // TODO: correct ...xxx
                NError.pushAndPrintError("NString.vAppend", "Unexpected sequence");
                //NError.pushAndPrintError("NString.vAppend", "Unexpected sequence at index %d", index);
                goto exit;
        }
    }

    // Add the termination zero,
    exit:
    NByteVector.pushBack(outVector, 0);

    return outString;
}

static struct NString* append(struct NString* outString, const char* format, ...) {
    va_list vaList;
    va_start(vaList, format);
    vAppend(outString, format, vaList);
    va_end(vaList);
    return outString;
}

static struct NString* set(struct NString* outString, const char* format, ...) {
    NByteVector.clear(&(outString->string));
    NByteVector.pushBack(&outString->string, 0);

    va_list vaList;
    va_start(vaList, format);
    vAppend(outString, format, vaList);
    va_end(vaList);
    return outString;
}

static const char* get(struct NString* string) {
    return string->string.objects;
}

static struct NString* create(const char* format, ...) {
    struct NString* outString = NSystemUtils.malloc(sizeof(struct NString));
    initialize(outString);

    va_list vaList;
    va_start(vaList, format);
    vAppend(outString, format, vaList);
    va_end(vaList);
    return outString;
}

static int32_t length(struct NString* string) {
    return string->string.size - 1;
}

const struct NString_Interface NString = {
    .initialize = initialize,
    .destroy = destroy,
    .destroyAndFree = destroyAndFree,
    .vAppend = vAppend,
    .append = append,
    .set = set,
    .get = get,
    .create = create,
    .length = length
};