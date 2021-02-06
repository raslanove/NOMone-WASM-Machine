#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <WasmMachine.h>

// Reference code on how to do tail calling (threaded code) in portable C program,
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

char* compileWasmToBites(const char* watCode) {

    // Just echo for now,
    char* value = malloc(strlen(watCode)+1);
    sprintf(value, "%s", watCode);
    return value;
}
