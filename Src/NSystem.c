
#include <NSystem.h>

static void initialize(void (*nMain)()) {
    if (nMain) nMain();
}

static void terminate() {
}

const struct NSystem_Interface NSystem = {
    .initialize = initialize,
    .terminate = terminate
};