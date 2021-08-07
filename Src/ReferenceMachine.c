
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
static int32_t INLINE skipWhiteSpaces(const char* string, int32_t index);
static int32_t getToken(const char* string, int32_t index, char* outputToken);
static boolean parseModule(struct ParsingStructure* parsingStructure, int32_t watCodeByteIndex);
static boolean parseModuleExpression(struct ParsingStructure* parsingStructure, int32_t watCodeByteIndex);

static void NWM_destroyReferenceMachine(struct NWM_WasmMachine *machine) {
    machine->alive = False;
}

static boolean parseWatCode(struct NWM_WasmMachine *machine, const char* watCode) {

    // Start parsing (recursive descent),
    struct ParsingStructure parsingStructure;
    parsingStructure.machine = machine;
    parsingStructure.watCode = watCode;

    boolean success = parseModule(&parsingStructure, 0);

    return success;
}

static boolean INLINE isWhiteSpace(char byte) {
    return byte==' ' || byte=='\n' || byte=='\t' || byte=='\r';
}

static boolean INLINE isOneByteToken(char byte) {
    return byte=='(' || byte==')' || byte==';';
}

static int32_t INLINE skipWhiteSpaces(const char* string, int32_t index) {

    char byte = string[index];
    while (isWhiteSpace(byte)) byte = string[++index];

    return index;
}

static int32_t getToken(const char* string, int32_t index, char* outputToken) {

    index = skipWhiteSpaces(string, index);
    char byte = string[index];

    // One byte tokens,
    if (isOneByteToken(byte)) {
        outputToken[0] = byte;
        outputToken[1] = 0;
        return index+1;
    }

    // Word tokens,
    int32_t outputIndex = 0;
    while (byte && !isWhiteSpace(byte) && !isOneByteToken(byte)) {
        outputToken[outputIndex++] = byte;
        if (outputIndex == MAX_TOKEN_LENGTH) {
            // TODO: ...xxx
            //NERROR("TOKEN exceeded maximum length: %s", outputToken);
            outputToken[outputIndex] = 0;
            return index;
        }
        byte = string[++index];
    }

    outputToken[outputIndex] = 0;
    return index;
}

static boolean parseModule(struct ParsingStructure* parsingStructure, int32_t watCodeByteIndex) {

    // Parse,
    char token[MAX_TOKEN_LENGTH];
    watCodeByteIndex = getToken(parsingStructure->watCode, watCodeByteIndex, token);
    // TODO: check for open parenthesis...
    if (!NCString.startsWith(&(parsingStructure->watCode[watCodeByteIndex]), "module")) return False;

    // Advance the wat byte index,
    watCodeByteIndex += 7; // strlen("(module")
    watCodeByteIndex = skipWhiteSpaces(parsingStructure->watCode, watCodeByteIndex);

    // ...xxx

    NLOGI("Ref machine", "yay!");
    return True;
}

static boolean parseModuleExpression(struct ParsingStructure* parsingStructure, int32_t watCodeByteIndex) {

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
