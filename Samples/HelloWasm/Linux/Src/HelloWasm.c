
#include <WasmMachine.h>
#include <NSystemUtils.h>

void NMain(void) {

    char watCode[] = "(module)";
    
    struct NWM_WasmMachine* machine = NWM.createReferenceWasmMachineInHeap();
    boolean result = machine->parseWatCode(machine, watCode);
    machine->destroy(machine);
    NSystemUtils.free(machine);
    
    NSystemUtils.log("%d\n", result);
}
