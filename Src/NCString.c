
#include <NCString.h>
#include <NError.h>
#include <NSystemUtils.h>

static int32_t strlen(const char* string) {
    int32_t length=0;
    while (string[length]) length++;
    return length;
}

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

static int32_t parseInteger(const char* string) {
    int32_t integerLength = strlen(string);
    if (integerLength > 11) {
        NERROR("NCString.parseInteger()", "Integer length can't exceed 10 digits and a sign. Found: %s%s", NTCOLOR(HIGHLIGHT), string);
        return 0;
    }

    // Parse,
    // Sign,
    int32_t offset=0;
    int32_t currentDigitMagnitude = 1;
    if (string[0] == '-') {
        offset = 1;
        integerLength--;
        currentDigitMagnitude = -1;
    }

    // Value,
    int32_t value=0;
    int32_t lastValue=0;
    char currentChar;
    while (integerLength--) {
        currentChar = string[offset + integerLength] - 48;
        if ((currentChar<0) || (currentChar>9)) {
            NERROR("NCString.parseInteger()", "Only digits from 0 to 9 are allowed. Found: %s%s", NTCOLOR(HIGHLIGHT), string);
            return 0;
        }
        value += currentDigitMagnitude * (int32_t) currentChar;
        currentDigitMagnitude *= 10;

        // Check overflow,
        if (offset) {
            if (lastValue < value) {
                NERROR("NCString.parseInteger()", "Value too small to fit in a 32 bit integer: %s%s", NTCOLOR(HIGHLIGHT), string);
                return 0;
            }
        } else {
            if (lastValue > value) {
                NERROR("NCString.parseInteger()", "Value too large to fit in a 32 bit integer: %s%s", NTCOLOR(HIGHLIGHT), string);
                return 0;
            }
        }
        lastValue = value;
    }

    return value;
}

const struct NCString_Interface NCString = {
    .length = strlen,
    .startsWith = startsWith,
    .equals = equals,
    .copy = copy,
    .parseInteger = parseInteger
};