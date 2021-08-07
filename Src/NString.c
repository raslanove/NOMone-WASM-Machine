
#include <NString.h>


static struct NString* initialize(struct NString* outputString) {
    NByteVector.create(0, &(outputString->string));
}

static void destroy(struct NString* outputString) {
    NByteVector.destroy(&(outputString->string));
}

static struct NString* create(const char* format, ...) {
    // TODO: ...xxx
}

const struct NString_Interface NString = {
    .initialize = initialize,
    .destroy = destroy,
    .create = create
};