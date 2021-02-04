#include <WasmMachine.h>

#include <stdio.h>

void main(void) {

    char message[256];
    compileWasmToBites(message);

    printf("%s\n", message);    
}
