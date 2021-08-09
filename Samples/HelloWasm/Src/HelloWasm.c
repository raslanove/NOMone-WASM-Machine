
#include <WasmMachine.h>
#include <NSystemUtils.h>

#include <NString.h>

void NMain() {

    char watCode[] = "(module)";
    
    struct NWM_WasmMachine* machine = NWM.createReferenceWasmMachineInHeap();
    boolean result = machine->parseWatCode(machine, watCode);
    machine->destroy(machine);
    NSystemUtils.free(machine);
    NSystemUtils.logE("Test", "%s\n", (result==1 ? "True" : "False"));
}
