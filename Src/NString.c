
#include <NString.h>
#include <NError.h>
#include <NVarArgs.h>
#include <NSystemUtils.h>

static struct NString* initialize(struct NString* outputString) {
    NByteVector.create(4, &outputString->string);
    NByteVector.pushBack(&outputString->string, 0);
    return outputString;
}

static void destroy(struct NString* outputString) {
    NByteVector.destroy(&(outputString->string));
}

static inline struct NString* vAppend(struct NString* outString, const char* format, va_list vaList) {

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

const struct NString_Interface NString = {
    .initialize = initialize,
    .destroy = destroy,
    .append = append,
    .set = set,
    .get = get,
    .create = create
};