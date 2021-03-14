
#include <stdio.h>
#include <stdlib.h>

#include <WasmMachine.h>

void main(void) {

    char watCode[] = "besm Allah AlRa7maan AlRa7eem :)";
    char *biteCode;
    
    struct NWM_WasmMachine* machine = NWM.createReferenceWasmMachine();
    biteCode = machine->compileWasmToBites(watCode);
    free(machine);
    
    printf("%s\n", biteCode);
    free(biteCode);
}
