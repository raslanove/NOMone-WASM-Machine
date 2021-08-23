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

    // Create the error message,
    struct NString* errorMessageString = NString.create("");
    NString.vAppend(errorMessageString, errorMessageFormat, vaList);

    // Create a new error in the stack,
    struct NError* newError = (struct NError*) NVector.emplaceBack(errorsStack);
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

    NLOGE(tag, "%s", error->message);
    return error;
}

static struct NVector* popErrors(int32_t stackPosition) {
    if (!errorsStack) return 0;

    // If no errors, return immediately,
    int32_t errorsCount = NVector.size(errorsStack) - stackPosition;
    if (errorsCount <= 0) return 0;

    // Pop the errors and return them,
    struct NVector* errors = NVector.createInHeap(errorsCount, sizeof(struct NError));
    for (int32_t i=0; i<errorsCount; i++) {
        NVector.emplaceBack(errors);
        NVector.popBack(errorsStack, NVector.get(errors, i));
    }

    return errors;
}

static void destroyAndFreeErrors(struct NVector *errors) {
    NVector.destroy(errors);
    NSystemUtils.free(errors);
}

static int32_t popDestroyAndFreeErrors(int32_t stackPosition) {
    struct NVector* errors = popErrors(stackPosition);
    if (!errors) return 0;

    int32_t errorsCount = NVector.size(errors);
    destroyAndFreeErrors(errors);
    return errorsCount;
}

const struct NError_Interface NError = {
    .terminate = terminate,
    .observeErrors = observeErrors,
    .pushError = pushError,
    .pushAndPrintError = pushAndPrintError,
    .popErrors = popErrors,
    .destroyAndFreeErrors = destroyAndFreeErrors,
    .popDestroyAndFreeErrors = popDestroyAndFreeErrors
};
