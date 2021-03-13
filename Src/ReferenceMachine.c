#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WasmMachine.h>
#include <ReferenceMachine.h>

static char* compileWasmToBites(const char* watCode) {

    // Just echo for now,
    char* value = malloc(strlen(watCode)+1);
    sprintf(value, "%s", watCode);
    return value;
}

struct NWM_WasmMachine *NWM_createReferenceWasmMachine() {

    struct NWM_WasmMachine* machine = malloc(sizeof(struct NWM_WasmMachine));
    memset(machine, 0, sizeof(struct NWM_WasmMachine));

    machine->compileWasmToBites = compileWasmToBites;

    return machine;
}