#include <jni.h>

#include <string.h>
#include <stdio.h>

int jump(int a, int b) {

    static const int array[] = {(int) &&returnPosition - (int) &&startPosition};

    startPosition:
    returnPosition:

    if (a < b) {
        a *= 2;
        goto *(void *) ((int) && startPosition + array[0]);
    }

    return a;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_nomone_wasm_1machine_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {

    // Test the jumping,
    int jumpResult = jump(3, 25);
    char message[256];
    sprintf(message, "besm Allah: %d", jumpResult);
    return env->NewStringUTF(message);
}
