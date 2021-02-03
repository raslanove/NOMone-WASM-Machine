#include <jni.h>

extern "C" {
    #include <wasm_machine.h>
};

extern "C" JNIEXPORT jstring JNICALL
Java_com_nomone_wasm_1machine_MainActivity_compileWasmToBites(JNIEnv *env, jobject thiz) {

    char message[256];
    compileWasmToBites(message);
    return env->NewStringUTF(message);
}
