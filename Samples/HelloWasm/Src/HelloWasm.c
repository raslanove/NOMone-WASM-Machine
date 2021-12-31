#include <WasmMachine.h>
#include <NSystemUtils.h>
#include <NError.h>
#include <NVector.h>

void NMain(int argc, char *argv[]) {

    char watCode[] =
            "(module\n"
            "  (type (;0;) (func))\n"
            "  (type (;1;) (func (param i32 i32 i64)))\n"
            "  (type (;10;) (func (param i32 i32 i64) (result i32)))\n"
            "  (func $__wasm_call_ctors (type 0))\n"
            "  (func $initialize (type 4) (param i32))\n"
            "  (func $strlen (type 6) (param i32) (result i32) (local i32 i32 i32))\n"
            ")";

    struct NWM_WasmMachine* machine = NWM.createReferenceMachine();
    boolean result = machine->parseWatCode(machine, watCode);
    machine->destroyAndFree(machine);

    NSystemUtils.logI("Parsing result", "%s\n", (result==1 ? "True" : "False"));

    NError.logAndTerminate();
}
