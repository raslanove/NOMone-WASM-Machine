#include "Includes/NativeInterface.h"

extern "C" {
    #include <WasmMachine.h>
};

JNI_CALL_PREFIX jstring JNICALL JNI_FUNCTION_NAME_PREFIX(nativeCompileWasmToBites)(JNI_FUNCTION_SIGNATURE_PREFIX) {

    char message[256];
    compileWasmToBites(message);
    return env->NewStringUTF(message);
}