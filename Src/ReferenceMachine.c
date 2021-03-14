#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WasmMachine.h>
#include <ReferenceMachine.h>

static void NWM_destroyReferenceMachine(struct NWM_WasmMachine *machine) {
    machine->alive = False;
}

static char* compileWasmToBites(struct NWM_WasmMachine *machine, const char* watCode) {

    // Just echo for now,
    char* value = malloc(strlen(watCode)+1);
    sprintf(value, "%s", watCode);
    return value;
}

struct NWM_WasmMachine *NWM_createReferenceWasmMachine(struct NWM_WasmMachine *outputMachine) {

    memset(outputMachine, 0, sizeof(struct NWM_WasmMachine));

    outputMachine->alive = True;
    outputMachine->destroy = NWM_destroyReferenceMachine;
    outputMachine->compileWasmToBites = compileWasmToBites;

    return outputMachine;
}

struct NWM_WasmMachine *NWM_createReferenceWasmMachineInHeap() {
    struct NWM_WasmMachine* machine = malloc(sizeof(struct NWM_WasmMachine));
    return NWM_createReferenceWasmMachine(machine);
}