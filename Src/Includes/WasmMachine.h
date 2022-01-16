#pragma once

#include <NTypes.h>

struct NWM_WasmMachine {
    boolean alive;
    void* data;

    void (*destroy)(struct NWM_WasmMachine *machine);
    void (*destroyAndFree)(struct NWM_WasmMachine *machine);
    boolean (*parseWatCode)(struct NWM_WasmMachine *machine, const char *watCode);
};

struct NWM_Interface {    // NOMone Wasm Machine.
    struct NWM_WasmMachine* (*initializeReferenceMachine)(struct NWM_WasmMachine* outputMachine);
    struct NWM_WasmMachine* (*createReferenceMachine)();
};

extern const struct NWM_Interface NWM;
