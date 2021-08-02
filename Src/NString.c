
#include <NString.h>

#include <stdint.h>

static boolean startsWith(const char* string, const char* value) {

    int32_t index=0;
    while (value[index]) {
        if (string[index] != value[index]) return False;
        index++;
    }
    return True;
}

const struct NString_Interface NString = {
    .startsWith = startsWith
};