
#include <NCString.h>

static boolean startsWith(const char* string, const char* value) {

    int32_t index=0;
    while (string[index] && value[index]) {
        if (string[index] != value[index]) return False;
        index++;
    }

    // Check if loop ended because string ended while value didn't,
    if (value[index]) return False;

    // Oh well, they must be equals then :D
    return True;
}

static boolean equals(const char* string, const char* value) {

    int32_t index=0;
    while (string[index] && value[index]) {
        if (string[index] != value[index]) return False;
        index++;
    }

    // Check if the loop ended while either of the two strings didn't reach the end yet,
    if (string[index] || value[index]) return False;

    // Yep, they are equal :)
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

const struct NCString_Interface NCString = {
    .startsWith = startsWith,
    .equals = equals,
    .copy = copy
};