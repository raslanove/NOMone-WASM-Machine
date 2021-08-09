#include "Includes/NativeInterface.h"

#include <NSystem.h>

static const char packageName[] = NATIVES_CLASS;
static JavaVM *currentJvm=0;
static jclass interfaceClass;

extern "C" void NMain();

class JniState {
    bool shouldDetachThread;
public:
    JNIEnv *env;
    jclass interfaceClass;

    JniState();
    ~JniState();
};

JniState::JniState() {

    this->interfaceClass = ::interfaceClass;
    shouldDetachThread = false;
    if (currentJvm->GetEnv((void **)&env, JNI_VERSION_1_6) == JNI_EDETACHED) {
        currentJvm->AttachCurrentThread(&env, nullptr);
        shouldDetachThread = true;
    }
}

JniState::~JniState() {
    if (shouldDetachThread) currentJvm->DetachCurrentThread();
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {

    currentJvm = jvm;

    JNIEnv *env;
    currentJvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    interfaceClass = env->FindClass(packageName);
    interfaceClass = (jclass) env->NewGlobalRef(interfaceClass);

    //env->RegisterNatives(clazz, sMethods, 1);

#ifdef NMAIN
    NSystem.initialize(NMain);
#else
    NSystem.initialize(0);
#endif

    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *jvm, void *reserved) {
    NSystem.terminate();
}