#include <jni.h>

extern "C" {
    #include <WasmMachine.h>
};

extern "C" JNIEXPORT jstring JNICALL
Java_com_nomone_wasm_1machine_WasmMachineNatives_nativeCompileWasmToBites(JNIEnv *env, jclass clazz) {

    char message[256];
    compileWasmToBites(message);
    return env->NewStringUTF(message);
}