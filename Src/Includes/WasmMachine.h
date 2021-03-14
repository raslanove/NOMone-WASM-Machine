#pragma once

#ifndef BOOL
#define BOOL
typedef enum { False=0, True=1 } boolean;
#endif

struct NWM_WasmMachine {
    boolean alive;
    void (*destroy)(struct NWM_WasmMachine *machine);
    char* (*compileWasmToBites)(struct NWM_WasmMachine *machine, const char *watCode);
};

struct NWM_Interface {    // NOMone Wasm Machine.
    struct NWM_WasmMachine* (*createReferenceWasmMachine)(struct NWM_WasmMachine* outputMachine);
    struct NWM_WasmMachine* (*createReferenceWasmMachineInHeap)();
};

extern const struct NWM_Interface NWM;
