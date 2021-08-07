#include <NError.h>
#include <NVector.h>
#include <NSystemUtils.h>
#include <NCString.h>

static struct NVector* errorsStack;

static void initialize() {
    errorsStack = NVector.createInHeap(0, sizeof(struct NError));
}

static void terminate() {
    NVector.destroy(errorsStack);
    NSystemUtils.free(errorsStack);
    errorsStack = 0;
}

static int32_t observeErrors() {
    if (!errorsStack) return 0;
    return NVector.size(errorsStack);
}

static struct NError* pushError(const char* errorMessage) {
    if (!errorsStack) initialize();

    // Create a new error in the stack,
    struct NError* newError = (struct NError*) NVector.emplaceBack(errorsStack);
    NCString.copy(newError->message, errorMessage);
    NTime.getTime(&(newError->time));

    return newError;
}

static struct NError* pushAndPrintError(const char* tag, const char* errorMessage) {
    struct NError *error = pushError(errorMessage);
    NSystemUtils.logE(tag, error->message);
    return error;
}

static struct NVector* popErrors(int32_t stackPosition) {
    if (!errorsStack) return 0;

    // TODO:....
    // ...xxx
}

const struct NError_Interface NError = {
    .terminate = terminate,
    .observeErrors = observeErrors,
    .pushError = pushError,
    .pushAndPrintError = pushAndPrintError,
    .popErrors = popErrors
};