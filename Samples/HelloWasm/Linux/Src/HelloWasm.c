
#include <stdio.h>
#include <stdlib.h>

#include <WasmMachine.h>

void main(void) {

    char watCode[] = "(module)";

    struct NWM_WasmMachine* machine = NWM.createReferenceWasmMachineInHeap();
    boolean result = machine->parseWatCode(machine, watCode);
    machine->destroy(machine);
    free(machine);
    
    printf("%d\n", result);
}
