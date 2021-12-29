#pragma once

struct NWM_WasmMachine;

struct NWM_WasmMachine *NWM_initializeReferenceMachine(struct NWM_WasmMachine *outputMachine);
struct NWM_WasmMachine *NWM_createReferenceMachine();