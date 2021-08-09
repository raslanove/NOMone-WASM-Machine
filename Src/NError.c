#include <NError.h>
#include <NVector.h>
#include <NSystemUtils.h>
#include <NString.h>

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

static struct NError* vPushError(const char* errorMessageFormat, va_list vaList) {
    if (!errorsStack) initialize();

    // Create a new error in the stack,
    struct NError* newError = (struct NError*) NVector.emplaceBack(errorsStack);

    // Create the error message,
    struct NString* errorMessageString = NString.create("");
    NString.vAppend(errorMessageString, errorMessageFormat, vaList);

    int32_t messageLength = NString.length(errorMessageString);
    if (messageLength >= NERROR_MAX_MESSAGE_LENGTH) {
        NSystemUtils.memcpy(newError->message, NString.get(errorMessageString), NERROR_MAX_MESSAGE_LENGTH-1);
        newError->message[NERROR_MAX_MESSAGE_LENGTH-1] = 0;
        NERROR("NError", "Error message length exceeded maximum size");
    } else {
        NSystemUtils.memcpy(newError->message, NString.get(errorMessageString), messageLength+1);
    }
    NString.destroyAndFree(errorMessageString);

    NTime.getTime(&(newError->time));

    return newError;
}

static struct NError* pushError(const char* errorMessageFormat, ...) {
    va_list vaList;
    va_start(vaList, errorMessageFormat);
    struct NError* error = vPushError(errorMessageFormat, vaList);
    va_end(vaList);
    return error;
}

static struct NError* pushAndPrintError(const char* tag, const char* errorMessageFormat, ...) {
    va_list vaList;
    va_start(vaList, errorMessageFormat);
    struct NError* error = vPushError(errorMessageFormat, vaList);
    va_end(vaList);

    NLOGE(tag, error->message);
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