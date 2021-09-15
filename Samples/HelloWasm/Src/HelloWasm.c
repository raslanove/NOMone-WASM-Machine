#include <WasmMachine.h>
#include <NSystemUtils.h>
#include <NError.h>
#include <NVector.h>

void NMain() {

    char watCode[] =
        "(module\n"
        "  (type (;0;) (func))"
        "  (type (;1;) (func (param i32 i32 i32)))"
        ")";

    struct NWM_WasmMachine* machine = NWM.createReferenceWasmMachineInHeap();
    boolean result = machine->parseWatCode(machine, watCode);
    machine->destroy(machine);
    NSystemUtils.free(machine);

    NSystemUtils.logI("Parsing result", "%s\n", (result==1 ? "True" : "False"));

    NError.logAndTerminate();
}
