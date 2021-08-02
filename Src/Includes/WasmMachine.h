#pragma once

#include <NTypes.h>

struct NWM_WasmMachine {
    boolean alive;
    void (*destroy)(struct NWM_WasmMachine *machine);
    boolean (*parseWatCode)(struct NWM_WasmMachine *machine, const char *watCode);
};

struct NWM_Interface {    // NOMone Wasm Machine.
    struct NWM_WasmMachine* (*createReferenceWasmMachine)(struct NWM_WasmMachine* outputMachine);
    struct NWM_WasmMachine* (*createReferenceWasmMachineInHeap)();
};

extern const struct NWM_Interface NWM;
