
#include <WasmMachine.h>
#include <ReferenceMachine.h>

const struct NWM_Interface NWM = {
    .initializeReferenceMachine = NWM_initializeReferenceMachine,
    .createReferenceMachine = NWM_createReferenceMachine
};

///////////////////////////////////////
// Code experiments and extras
///////////////////////////////////////

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