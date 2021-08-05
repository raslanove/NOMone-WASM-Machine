
#include <NString.h>

static boolean startsWith(const char* string, const char* value) {

    int32_t index=0;
    while (value[index]) {
        if (string[index] != value[index]) return False;
        index++;
    }
    return True;
}

static char* copy(char* destination, const char* source) {

    int32_t index=0;
    char currentChar;
    do {
        currentChar = source[index];
        destination[index] = currentChar;
        index++;
    } while (currentChar);

    return destination;
}

const struct NString_Interface NString = {
    .startsWith = startsWith,
    .copy = copy
};