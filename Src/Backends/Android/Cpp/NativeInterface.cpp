#include "Includes/NativeInterface.h"

extern "C" {
    #include <WasmMachine.h>
};

JNI_CALL_PREFIX jstring JNICALL JNI_FUNCTION_NAME_PREFIX(nativeCompileWasmToBites)(JNI_FUNCTION_SIGNATURE_PREFIX, jstring watCode) {

    const char* nativeString = env->GetStringUTFChars(watCode, 0);
    char* biteCode = compileWasmToBites(nativeString);
    env->ReleaseStringUTFChars(watCode, nativeString);
    jstring returnString = env->NewStringUTF(biteCode);
    delete biteCode;

    return returnString;
}