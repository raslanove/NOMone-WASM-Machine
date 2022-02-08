#pragma once

#include <NTypes.h>

typedef void* NWM_Function;
struct NByteVector;

struct NWM_WasmMachine {
    boolean alive;
    void* data;

    void (*destroy)(struct NWM_WasmMachine *machine);
    void (*destroyAndFree)(struct NWM_WasmMachine *machine);
    boolean (*parseWatCode)(struct NWM_WasmMachine *machine, const char *watCode);
    struct NByteVector* (*getStack)(struct NWM_WasmMachine *machine, int32_t moduleIndex);
    NWM_Function (*getFunction)(struct NWM_WasmMachine* machine, int32_t moduleIndex, const char* functionName);
    void (*callFunction)(NWM_Function functionHandle);
};

struct NWM_Interface {    // NOMone Wasm Machine.
    struct NWM_WasmMachine* (*initializeReferenceMachine)(struct NWM_WasmMachine* outputMachine);
    struct NWM_WasmMachine* (*createReferenceMachine)();
};

extern const struct NWM_Interface NWM;
