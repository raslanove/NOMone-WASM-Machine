#include <stdint.h>
#include <string.h>
#include <stdio.h>

int jump(int a, int b) {

    static const int array[] = {(intptr_t) &&returnPosition - (intptr_t) &&startPosition};

    startPosition:
    returnPosition:

    if (a < b) {
        a *= 2;
        goto *(void *) ((intptr_t) && startPosition + array[0]);
    }

    return a;
}

void compileWasmToBites(char *message) {

    // Test the jumping,
    int jumpResult = jump(3, 25);
    sprintf(message, "besm Allah: %d", jumpResult);
}
