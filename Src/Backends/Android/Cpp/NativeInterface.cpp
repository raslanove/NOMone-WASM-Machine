#include "Includes/NativeInterface.h"

extern "C" {
    #include <WasmMachine.h>
    #include <Vector.h>
};

static boolean initialized=False;
static struct NVector machines;

static void initialize() {
    NVector.create(1, sizeof(NWM_WasmMachine), &machines);
    initialized = True;
}

static struct NWM_WasmMachine* getMachine(int32_t index) {
    return (NWM_WasmMachine *) NVector.get(&machines, index);
}

JNI_CALL_PREFIX jstring JNICALL JNI_FUNCTION_NAME_PREFIX(nativeCompileWasmToBites)(JNI_FUNCTION_SIGNATURE_PREFIX, int32_t machineIndex, jstring watCode) {

    NWM_WasmMachine *machine = getMachine(machineIndex);

    const char* nativeString = env->GetStringUTFChars(watCode, 0);
    char* biteCode = machine->compileWasmToBites(machine, nativeString);
    env->ReleaseStringUTFChars(watCode, nativeString);
    jstring returnString = env->NewStringUTF(biteCode);
    delete biteCode;

    return returnString;
}

JNI_CALL_PREFIX int JNICALL JNI_FUNCTION_NAME_PREFIX(nativeCreateReferenceMachine)(JNI_FUNCTION_SIGNATURE_PREFIX) {

    // Create a vector if non-existing,
    if (!initialized) initialize();

    // Find the first dead machine (if any),
    int32_t machinesCount = NVector.size(&machines);
    int32_t newMachineIndex=0;
    for (; newMachineIndex<machinesCount; newMachineIndex++) {
        if (!getMachine(newMachineIndex)->alive) break;
    }

    // Push a new machine if no dead ones found,
    if (newMachineIndex==machinesCount) {
        NWM_WasmMachine newMachine;
        NVector.pushBack(&machines, &newMachine);
    }

    // Create the new machine,
    NWM.createReferenceWasmMachine(getMachine(newMachineIndex));

    return newMachineIndex;
}

JNI_CALL_PREFIX void JNICALL JNI_FUNCTION_NAME_PREFIX(nativeDestroyMachine)(JNI_FUNCTION_SIGNATURE_PREFIX, int32_t machineIndex) {
    NWM_WasmMachine *machine = getMachine(machineIndex);
    machine->destroy(machine);
}