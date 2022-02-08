#include <WasmMachine.h>
#include <NSystemUtils.h>
#include <NError.h>
#include <NVector.h>
#include <NByteVector.h>
#include <NCString.h>

void NMain(int argc, char *argv[]) {

    // Understanding the generated code.
    // =================================
    // __wasm_call_ctors:
    // ==================
    //   An empty function, never seen it have any content. We can remove it safely.
    //
    // __dso_handle:
    // =============
    //   - A DSO (Dynamic Shared Object) is an object file that is meant to be used simultaneously (or shared) by multiple applications (.o files?) while they are executing.
    //     See: https://www3.risc.jku.at/education/courses/ws2003/intropar/origin-new/MproCplrDbx_TG/sgi_html/ch03.html
    //   - The __dso_handle has something to do with calling terminating functions in DSOs (see: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#dso-dtor-runtime-api ).
    //   - We don't use the __dso_handle, so you can just ignore it.
    //

    char watCode[] =
            "(module\n"
            "  (type (;0;) (func))\n"
            "  (type (;1;) (func (param i32 i32) (result i32)))\n"
            "  (func $__wasm_call_ctors (type 0))\n"
            "  (func $foo (type 1) (param i32 i32) (result i32)\n"
            "    local.get 0\n"
            "    local.get 1\n"
            "    i32.add)\n"
            "  (table (;0;) 1 1 funcref)\n"
            "  (memory (;0;) 2)\n"
            "  (global (;0;) (mut i32) (i32.const 66560))    ;; Probably the stack pointer (or base pointer). It's used for local structs.\n"
            "  (global (;1;) i32 (i32.const 1024))           ;; __dso_handle\n"
            "  (global (;2;) i32 (i32.const 1024))           ;; __data_end\n"
            "  (global (;3;) i32 (i32.const 1024))           ;; __global_base\n"
            "  (global (;4;) i32 (i32.const 66560))          ;; __heap_base (stack size(64K) + __data_end (and sometimes +8))\n"
            "  (global (;5;) i32 (i32.const 0))              ;; __memory_base\n"
            "  (global (;6;) i32 (i32.const 1))              ;; __table_base\n"
            "  (export \"memory\" (memory 0))\n"
            "  (export \"__wasm_call_ctors\" (func $__wasm_call_ctors))\n"
            "  (export \"foo\" (func $foo))\n"
            "  (export \"__dso_handle\" (global 1))\n"
            "  (export \"__data_end\" (global 2))\n"
            "  (export \"__global_base\" (global 3))\n"
            "  (export \"__heap_base\" (global 4))\n"
            "  (export \"__memory_base\" (global 5))\n"
            "  (export \"__table_base\" (global 6)))";

    struct NWM_WasmMachine* machine = NWM.createReferenceMachine();
    boolean result = machine->parseWatCode(machine, watCode);

    if (result) {
        NLOGI("Parsing result", "True\n");
    } else {
        NLOGE("Parsing result", "False. Required match length: %s%d%s\n", NTCOLOR(HIGHLIGHT), NCString.length(watCode), NTCOLOR(STREAM_DEFAULT));
    }

    // Call an arbitrary function,
    NWM_Function function = machine->getFunction(machine, 0, "$foo");
    if (!function) NLOGE("HelloWasm.NMain()", "Function not found.");

    struct NByteVector* machineStack = machine->getStack(machine, 0);
    NByteVector.pushBack32Bit(machineStack, 5);
    NByteVector.pushBack32Bit(machineStack, 7);
    machine->callFunction(function);

    // Clean up,
    machine->destroyAndFree(machine);
    NError.logAndTerminate();
}
