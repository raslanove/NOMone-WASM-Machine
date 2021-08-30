
#include <ReferenceMachine.h>

#include <WasmMachine.h>
#include <NSystemUtils.h>
#include <NByteVector.h>
#include <NCString.h>
#include <NError.h>

#define MAX_TOKEN_LENGTH 256
#define INLINE inline

struct ParsingStructure {
    struct NWM_WasmMachine *machine;
    const char* watCode;
};

static boolean INLINE isWhiteSpace(char byte);
static boolean INLINE isOneByteToken(char byte);
static void INLINE skipWhiteSpaces(const char* string, int32_t* in_out_index);
static boolean getToken(const char* logTag, const char* inputText, int32_t* in_out_index, const char* expectedToken, char* outputToken);
static boolean parseModule(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex);

static void NWM_destroyReferenceMachine(struct NWM_WasmMachine *machine) {
    machine->alive = False;
}

static boolean parseWatCode(struct NWM_WasmMachine *machine, const char* watCode) {

    // Start parsing (recursive descent),
    struct ParsingStructure parsingStructure;
    parsingStructure.machine = machine;
    parsingStructure.watCode = watCode;

    int32_t watCodeIndex=0;
    boolean success = parseModule(&parsingStructure, &watCodeIndex);

    return success;
}

static boolean INLINE isWhiteSpace(char byte) {
    return byte==' ' || byte=='\n' || byte=='\t' || byte=='\r';
}

static boolean INLINE isOneByteToken(char byte) {
    return byte=='(' || byte==')' || byte==';';
}

static void INLINE skipWhiteSpaces(const char* string, int32_t* in_out_index) {
    char byte = string[*in_out_index];
    while (isWhiteSpace(byte)) byte = string[++(*in_out_index)];
}

// TODO: modify to accept token type instead of exact token text?
// Returns True if parsing should continue.
static boolean getToken(const char* logTag, const char* inputText, int32_t* in_out_index, const char* expectedToken, char* outputToken) {

    skipWhiteSpaces(inputText, in_out_index);
    char byte = inputText[*in_out_index];

    boolean shouldContinueParsing = True;
    if (!byte) {
        // Check for end-of-file,
        NCString.copy(outputToken, "EOF");

    } else if (isOneByteToken(byte)) {
        // One byte tokens,
        outputToken[0] = byte;
        outputToken[1] = 0;
        (*in_out_index)++;

    } else {
        // Word tokens,
        int32_t outputIndex = 0;
        while (byte && !isWhiteSpace(byte) && !isOneByteToken(byte)) {
            outputToken[outputIndex++] = byte;
            if (outputIndex == MAX_TOKEN_LENGTH) {
                NERROR(logTag, "TOKEN exceeded maximum length: %s", outputToken);
                shouldContinueParsing = False;
                break;
            }
            byte = inputText[++(*in_out_index)];
        }
        outputToken[outputIndex] = 0;
    }

    // Check if this is the expected token,
    // TODO: keep track of line number and current column to use in error reporting...
    if (expectedToken && !NCString.equals(outputToken, expectedToken)) {
        NERROR(logTag, "Expected: \"%s%s%s\", found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), expectedToken, NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), outputToken, NTCOLOR(STREAM_DEFAULT));
        shouldContinueParsing = False;
    }

    // TODO: remove...
    if (shouldContinueParsing) NLOGI(logTag, "found token: %s%s", NTCOLOR(HIGHLIGHT), outputToken);

    return shouldContinueParsing;
}

static boolean parseModule(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex) {

    int32_t initialByteCodeIndex = *in_out_watCodeByteIndex;

    #define LOG_TAG "ReferenceMachine.parseModule()"
    #define ASSERT_TOKEN(expectedToken) \
    if (!getToken(LOG_TAG, parsingStructure->watCode, in_out_watCodeByteIndex, expectedToken, token)) { \
        *in_out_watCodeByteIndex = initialByteCodeIndex; \
        return False; \
    }
    #define TEST_TOKEN(testValue) NCString.equals(token, testValue)

    // Parse,
    char token[MAX_TOKEN_LENGTH];

    // "(",
    ASSERT_TOKEN("(")

    // "module",
    ASSERT_TOKEN("module")

    // Parse child expressions,
    // Note: this could be accelerated by using trees, but would compromise readability. Maybe
    // use Flex and Bison for the accelerated (non-reference) machine.
    int32_t childExpressionIndex = *in_out_watCodeByteIndex;
    do {
        // Get token,
        ASSERT_TOKEN(0)

        // ")",
        if (TEST_TOKEN(")")) {
            // TODO: finalize and save the module structure...
            return True;
        } else if (TEST_TOKEN("(")) {
            // Check child expressions,
            *in_out_watCodeByteIndex = childExpressionIndex;
            // observe errors ...
            // ....xxx
            // return false if error occurred...
        } else {
            NERROR(LOG_TAG, "Expected: \"%s)%s\" or \"%s(%s\", found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
            return False;
        }
    } while (True);

    return True;
}

static boolean parseType(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex) {

#undef LOG_TAG
#define LOG_TAG "ReferenceMachine.parseType()"
    int32_t initialWatCodeIndex = *in_out_watCodeByteIndex;
    char token[MAX_TOKEN_LENGTH];

    return True;
}

struct NWM_WasmMachine *NWM_createReferenceWasmMachine(struct NWM_WasmMachine *outputMachine) {

    NSystemUtils.memset(outputMachine, 0, sizeof(struct NWM_WasmMachine));

    outputMachine->alive = True;
    outputMachine->destroy = NWM_destroyReferenceMachine;
    outputMachine->parseWatCode = parseWatCode;

    return outputMachine;
}

struct NWM_WasmMachine *NWM_createReferenceWasmMachineInHeap() {
    struct NWM_WasmMachine* machine = NSystemUtils.malloc(sizeof(struct NWM_WasmMachine));
    return NWM_createReferenceWasmMachine(machine);
}
