#pragma once

struct NWM_WasmMachine {
    char* (*compileWasmToBites)(const char *watCode);
};

struct NWM_Interface {    // NOMone Wasm Machine.
    struct NWM_WasmMachine* (*createReferenceWasmMachine)();
};

extern const struct NWM_Interface NWM;
