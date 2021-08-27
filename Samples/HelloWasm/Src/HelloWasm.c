#include <WasmMachine.h>
#include <NSystemUtils.h>
#include <NError.h>
#include <NVector.h>

void NMain() {

    char watCode[] = "(module";

    struct NWM_WasmMachine* machine = NWM.createReferenceWasmMachineInHeap();
    boolean result = machine->parseWatCode(machine, watCode);
    machine->destroy(machine);
    NSystemUtils.free(machine);

    NSystemUtils.logI("Parsing result", "%s\n", (result==1 ? "True" : "False"));

    NError.logAndTerminate();
}
