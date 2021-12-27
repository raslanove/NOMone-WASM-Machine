#include <WasmMachine.h>
#include <NSystemUtils.h>
#include <NError.h>
#include <NVector.h>

void NMain(int argc, char *argv[]) {

    char watCode[] =
        "(module\n"
        "  (type (;0;) (func))"
        "  (type (;1;) (func (result i32) (param i32 i32 i64)))"
        ")";

    struct NWM_WasmMachine* machine = NWM.createReferenceWasmMachineInHeap();
    boolean result = machine->parseWatCode(machine, watCode);
    machine->destroy(machine);
    NSystemUtils.free(machine);

    NSystemUtils.logI("Parsing result", "%s\n", (result==1 ? "True" : "False"));

    NError.logAndTerminate();
}
