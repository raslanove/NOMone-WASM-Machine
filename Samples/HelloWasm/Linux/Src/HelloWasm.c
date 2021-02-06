#include <WasmMachine.h>

#include <stdio.h>
#include <stdlib.h>

void main(void) {

    char watCode[] = "besm Allah AlRa7maan AlRa7eem :)";
    char *biteCode = compileWasmToBites(watCode);

    printf("%s\n", biteCode);
    free(biteCode);
}
