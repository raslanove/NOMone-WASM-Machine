
#include <WasmMachine.h>
#include <NSystemUtils.h>
#include <NError.h>

void NMain() {

    char watCode[] = "(module)";
    
    struct NWM_WasmMachine* machine = NWM.createReferenceWasmMachineInHeap();
    boolean result = machine->parseWatCode(machine, watCode);
    machine->destroy(machine);
    NSystemUtils.free(machine);
    NSystemUtils.logE("Test", "%s\n", (result==1 ? "True" : "False"));

    NLOGE("Errors", "Unhandled errors count: %d", NError.popDestroyAndFreeErrors(0));
}
