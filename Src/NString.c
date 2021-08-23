
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
            case '%': {
                NByteVector.pushBack(outVector, '%');
                continue;
            }
            case 's': {
                const char* sourceString = va_arg(vaList, const char*);

                int32_t stringIndex = 0;
                char currentChar;
                while (currentChar = sourceString[stringIndex++]) {
                    NByteVector.pushBack(outVector, currentChar);
                }
                continue;
            }
            case 'c': {
                char sourceChar = (char) va_arg(vaList, int); // ‘char’ is promoted to ‘int’ when passed through ‘...’.
                NByteVector.pushBack(outVector, sourceChar);
                continue;
            }
            case 'd': {
                int32_t sourceInteger = (int32_t) va_arg(vaList, int32_t);

                if (sourceInteger < 0) {
                    NByteVector.pushBack(outVector, '-');
                    sourceInteger = 0-sourceInteger;
                }

                char digits[10];
                int32_t digitsCount=0;
                do {
                    digits[digitsCount++] = 48 + (sourceInteger%10);
                    sourceInteger /= 10;
                } while (sourceInteger);

                // Push digits in reverse order,
                while (digitsCount--) NByteVector.pushBack(outVector, digits[digitsCount]);

                continue;
            }
            case 'l': {

                // Check if a 'd' follows,
                currentChar = format[index++];
                if (currentChar != 'd') {
                    NERROR("NString.vAppend", "Expected \"%sd%s\" after \"%s%%l%s\" in format string: %s%s", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), format);
                    goto exit;
                }

                int64_t sourceInteger = (int64_t) va_arg(vaList, int64_t);

                if (sourceInteger < 0) {
                    NByteVector.pushBack(outVector, '-');
                    sourceInteger = 0-sourceInteger;
                }

                char digits[19];
                int32_t digitsCount=0;
                do {
                    digits[digitsCount++] = 48 + (sourceInteger%10);
                    sourceInteger /= 10;
                } while (sourceInteger);

                // Push digits in reverse order,
                while (digitsCount--) NByteVector.pushBack(outVector, digits[digitsCount]);

                continue;
            }

            default:
                NERROR("NString.vAppend", "Unexpected sequence: \"%s%%%c%s\" in format string: %s%s", NTCOLOR(HIGHLIGHT), currentChar, NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), format);
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

static struct NString* replace(const char* textToBeSearched, const char* textToBeRemoved, const char* textToBeInserted) {

    // Create a new string for the result,
    struct NString* newString = NString.create("");

    int32_t searchIndex=0;
    char currentChar;
    while (currentChar = textToBeSearched[searchIndex]) {

        // Attempt matching,
        int32_t matchIndex=0;
        while (textToBeRemoved[matchIndex] && textToBeSearched[searchIndex+matchIndex] &&
               textToBeRemoved[matchIndex] == textToBeSearched[searchIndex+matchIndex]) {
            matchIndex++;
        }

        // Check if a match occurred,
        if (!textToBeRemoved[matchIndex]) {
            NString.append(newString, "%s", textToBeInserted);
            searchIndex += matchIndex;
        } else {
            NString.append(newString, "%c", currentChar);
            searchIndex++;
        }
    }

    return newString;
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
    .replace = replace,
    .length = length
};
