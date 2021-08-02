#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <ReferenceMachine.h>

#include <WasmMachine.h>
#include <ByteVector.h>
#include <NString.h>

struct ParsingStructure {
    struct NWM_WasmMachine *machine;
    const char* watCode;
};

static boolean parseModule(struct ParsingStructure* parsingStructure, int32_t watCodeByteIndex);

static void NWM_destroyReferenceMachine(struct NWM_WasmMachine *machine) {
    machine->alive = False;
}

static boolean parseWatCode(struct NWM_WasmMachine *machine, const char* watCode) {

    // Start parsing (recursive descent),
    struct ParsingStructure parsingStructure;
    parsingStructure.machine = machine;
    parsingStructure.watCode = watCode;

    boolean success = parseModule(&parsingStructure, 0);

    return success;
}

static boolean parseModule(struct ParsingStructure* parsingStructure, int32_t watCodeByteIndex) {

    // Parse,
    if (!NString.startsWith(&(parsingStructure->watCode[watCodeByteIndex]), "(module")) return False;

    printf("yay!");
    return True;
}

struct NWM_WasmMachine *NWM_createReferenceWasmMachine(struct NWM_WasmMachine *outputMachine) {

    memset(outputMachine, 0, sizeof(struct NWM_WasmMachine));

    outputMachine->alive = True;
    outputMachine->destroy = NWM_destroyReferenceMachine;
    outputMachine->parseWatCode = parseWatCode;

    return outputMachine;
}

struct NWM_WasmMachine *NWM_createReferenceWasmMachineInHeap() {
    struct NWM_WasmMachine* machine = malloc(sizeof(struct NWM_WasmMachine));
    return NWM_createReferenceWasmMachine(machine);
}
