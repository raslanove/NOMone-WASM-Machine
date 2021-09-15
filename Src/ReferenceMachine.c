
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
static boolean parseType(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex);
static boolean parseFunctionParameters(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex);

#define ASSERT_TOKEN(expectedToken) \
if (!getToken(LOG_TAG, parsingStructure->watCode, in_out_watCodeByteIndex, expectedToken, token)) { \
    *in_out_watCodeByteIndex = initialWatCodeByteIndex; \
    return False; \
}
#define TEST_TOKEN(testValue) NCString.equals(token, testValue)

static void NWM_destroyReferenceMachine(struct NWM_WasmMachine *machine) {
    machine->alive = False;
}

static boolean parseWatCode(struct NWM_WasmMachine *machine, const char* watCode) {

    // Provide storage for the data stucture and pointers,
    struct ParsingStructure parsingStructureInstance;
    parsingStructureInstance.machine = machine;
    parsingStructureInstance.watCode = watCode;
    int32_t watCodeIndex=0;

    // Parse,
    struct ParsingStructure* parsingStructure = &parsingStructureInstance;
    int32_t* in_out_watCodeByteIndex = &watCodeIndex;

    #undef LOG_TAG
    #define LOG_TAG "ReferenceMachine.parseWatCode()"
    int32_t initialWatCodeByteIndex = *in_out_watCodeByteIndex;
    char token[MAX_TOKEN_LENGTH];

    if (!parseModule(parsingStructure, in_out_watCodeByteIndex)) return False;

    // Make sure the file ends here,
    ASSERT_TOKEN("EOF")

    return True;
}

static boolean INLINE isWhiteSpace(char byte) {
    return byte==' ' || byte=='\n' || byte=='\t' || byte=='\r';
}

static boolean INLINE isOneByteToken(char byte) {
    return byte=='(' || byte==')' || byte==';';
}

static void INLINE skipDelimitedComment(const char* string, int32_t* in_out_index) {

    int32_t index = *in_out_index;
    if ((string[index]!='(') || (string[index+1]!=';')) return;

    index+=2;
    do {
        *in_out_index = index;

        char byte = string[index];
        while (byte && (byte!='(') && (byte!=';')) byte = string[++index];

        if (byte=='(') {
            if (string[index+1]==';') {
                skipDelimitedComment(string, &index);
            } else {
                index++;
            }
        } else if (byte==';') {
            if (string[index+1]==')') {
                // Found the end of the comment,
                *in_out_index = index+2;
                return ;
            } else {
                index++;
            }
        } else {
            // String ended before the terminating delimeter,
            // TODO: add line number and column of the comment beginning...
            NERROR("ReferenceMachine.skipDelimitedComment()", "Delimited comment without terminating delimeter");
            *in_out_index = index;
            return;
        }

    } while (*in_out_index != index); // Continue as long as some progress took place within this iteration.
}

static void INLINE skipWhiteSpaces(const char* string, int32_t* in_out_index) {

    int32_t index = *in_out_index;
    do {
        *in_out_index = index;

        // Attempt skipping,
        char byte = string[index];

        // Actual whitespaces,
        while (isWhiteSpace(byte)) byte = string[++index];

        // Line comments (line comments start with ;; and continue till the end of the line),
        if (byte==';' && string[index+1]==';') {
            // Skip to the line end,
            index++;
            do { byte = string[++index]; } while (byte && (byte!='\n'));
        }

        // Delimited comments (delimited comments start with (; and end with ;). They can also be nested),
        skipDelimitedComment(string, &index);
    } while (index > *in_out_index); // Continue as long as some progress took place within this iteration.
}

// TODO: modify to accept token type instead of exact token text?
// Returns True if parsing should continue.
static boolean getToken(const char* logTag, const char* inputText, int32_t* in_out_index, const char* expectedToken, char* outputToken) {

    // Skip whitespaces (which also include comments),
    int32_t errorsBeginning = NError.observeErrors();
    skipWhiteSpaces(inputText, in_out_index);
    if (NError.observeErrors()>errorsBeginning) return False;

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

    #undef LOG_TAG
    #define LOG_TAG "ReferenceMachine.parseModule()"
    int32_t initialWatCodeByteIndex = *in_out_watCodeByteIndex;
    char token[MAX_TOKEN_LENGTH];

    // Parse,
    ASSERT_TOKEN("(")
    ASSERT_TOKEN("module")

    // Parse child expressions,
    // Note: this could be accelerated by using trees, but would compromise readability. Maybe
    // use Flex and Bison for the accelerated (non-reference) machine.
    do {
        // Get token,
        int32_t childExpressionIndex = *in_out_watCodeByteIndex;
        ASSERT_TOKEN(0)

        // ")",
        if (TEST_TOKEN(")")) {
            // TODO: finalize and save the module structure...
            return True;
        } else if (TEST_TOKEN("(")) {

            // Check child expressions,
            int32_t errorsStart = NError.observeErrors();
            *in_out_watCodeByteIndex = childExpressionIndex; // Return to the child expression beginning.

            // Check for type expression,
            // TODO: turn into a macro...
            if (parseType(parsingStructure, in_out_watCodeByteIndex)) continue;
            if (NError.observeErrors()-errorsStart > 0) {
                // TODO: destroy the allocated data structure...
                return False;
            }

            // TODO: ...Check other expressions...
            // ....xxx

        } else {
            NERROR(LOG_TAG, "Expected: \"%s)%s\" or \"%s(%s\", found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
            return False;
        }
    } while (True);
}

static boolean parseType(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex) {

    #undef LOG_TAG
    #define LOG_TAG "ReferenceMachine.parseType()"
    int32_t initialWatCodeByteIndex = *in_out_watCodeByteIndex;
    char token[MAX_TOKEN_LENGTH];

    // Parse,
    // Example: (type (func (param i32 i32 i32 i32) (result i32)))
    // Note: currently, we can only define function types, so we don't have a separate function
    // to parse functions.
    ASSERT_TOKEN("(")
    ASSERT_TOKEN("type")
    ASSERT_TOKEN("(")
    ASSERT_TOKEN("func")

    // Parse child expressions,
    do {
        // Get token,
        int32_t childExpressionIndex = *in_out_watCodeByteIndex;
        ASSERT_TOKEN(0)

        // ")",
        if (TEST_TOKEN(")")) {
            ASSERT_TOKEN(")")
            // TODO: finalize and save the function type structure...
            return True;
        } else if (TEST_TOKEN("(")) {

            // Check child expressions,
            int32_t errorsStart = NError.observeErrors();
            *in_out_watCodeByteIndex = childExpressionIndex; // Return to the child expression beginning.

            // Check for type expression,
            // TODO: turn into a macro...
            if (parseFunctionParameters(parsingStructure, in_out_watCodeByteIndex)) continue;
            if (NError.observeErrors()-errorsStart > 0) {
                // TODO: destroy the allocated data structure...
                return False;
            }

            // TODO: ...Check other expressions...
            // ....xxx

        } else {
            NERROR(LOG_TAG, "Expected: \"%s)%s\" or \"%s(%s\", found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
            return False;
        }
    } while (True);
}

static boolean parseFunctionParameters(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex) {

    #undef LOG_TAG
    #define LOG_TAG "ReferenceMachine.parseFunctionParameters()"
    int32_t initialWatCodeByteIndex = *in_out_watCodeByteIndex;
    char token[MAX_TOKEN_LENGTH];

    // Parse,
    // Example: (param i32 i32 i32 i32)
    ASSERT_TOKEN("(")
    ASSERT_TOKEN("param")

    // Parse arguments,
    int32_t parametersCount=0;
    do {
        // Get token,
        ASSERT_TOKEN(0)

        // ")",
        if (TEST_TOKEN(")")) {
            if (parametersCount > 0) {
                // TODO: finalize and save the parameters to the current function...
                return True;
            } else {
                NERROR(LOG_TAG, "Found param structure without any parameters");
                return False;
            }
        } else if (TEST_TOKEN("i32")) {
            parametersCount++;
            // TODO: add this i32 parameter...
        } else if (TEST_TOKEN("f64")) {
            parametersCount++;
            // TODO: add this f64 parameter...
        } else {
            NERROR(LOG_TAG, "Expected: \"%s)%s\" or \"%si32%s\" or \"%sf64%s\", found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
            return False;
        }
    } while (True);
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
