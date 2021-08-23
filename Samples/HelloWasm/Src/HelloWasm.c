
#include <WasmMachine.h>
#include <NSystemUtils.h>
#include <NError.h>
#include <NVector.h>

void NMain() {

    char watCode[] = "(module)";
    
    struct NWM_WasmMachine* machine = NWM.createReferenceWasmMachineInHeap();
    boolean result = machine->parseWatCode(machine, watCode);
    machine->destroy(machine);
    NSystemUtils.free(machine);
    NSystemUtils.logI("Test", "%s\n", (result==1 ? "True" : "False"));

    NLOGW("dsfdsf", "abc %f", 1.23);

    NERROR("Test", "Typing a char: %c%c%c %% %f", 'A', 'b', 'c');

    // Check if any errors ended up without handling,
    struct NVector* errors = NError.popErrors(0);
    if (errors) {
        NLOGW("Unhandled errors", "Unhandled errors count: %d", NVector.size(errors));
        struct NError error;
        while (NVector.popBack(errors, &error)) {
            NLOGW("Unhandled errors", "%s", error.message);
        }
        NError.destroyAndFreeErrors(errors);
    } else {
        NLOGI("Unhandled errors", "Unhandled errors count: 0");
    }
}
