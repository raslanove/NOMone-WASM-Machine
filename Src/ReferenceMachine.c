
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
static boolean getToken(const char* logTag, const char* inputText, int32_t* in_out_index, const char* expectedToken, char* outputToken, boolean throw);
static boolean parseModule(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex, boolean throw);
static boolean parseType(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex, boolean throw);
static boolean parseFunctionParameters(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex, boolean throw);
static boolean parseResult(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex, boolean throw);

#define ASSERT_TOKEN(expectedToken, throw) if (!getToken(LOG_TAG, parsingStructure->watCode, in_out_watCodeByteIndex, expectedToken, token, throw)) goto failureCleanUp;

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

    if (!parseModule(parsingStructure, in_out_watCodeByteIndex, True)) return False;

    // Make sure the file ends here,
    ASSERT_TOKEN("EOF", True)

    return True;

failureCleanUp:
    *in_out_watCodeByteIndex = initialWatCodeByteIndex;
    return False;
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
static boolean getToken(const char* logTag, const char* inputText, int32_t* in_out_index, const char* expectedToken, char* outputToken, boolean throw) {

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
        if (throw) NERROR(logTag, "Expected: \"%s%s%s\", found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), expectedToken, NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), outputToken, NTCOLOR(STREAM_DEFAULT));
        shouldContinueParsing = False;
    }

    // TODO: remove...
    if (shouldContinueParsing) NLOGI(logTag, "found token: %s%s", NTCOLOR(HIGHLIGHT), outputToken);

    return shouldContinueParsing;
}

static boolean parseModule(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex, boolean throw) {

    #undef LOG_TAG
    #define LOG_TAG "ReferenceMachine.parseModule()"
    int32_t initialWatCodeByteIndex = *in_out_watCodeByteIndex;
    char token[MAX_TOKEN_LENGTH];

    // Parse,
    ASSERT_TOKEN("(", throw)
    ASSERT_TOKEN("module", throw)

    // Parse child expressions,
    // Note: this could be accelerated by using trees, but would compromise readability. Maybe
    // use Flex and Bison for the accelerated (non-reference) machine.
    do {
        // Get token,
        int32_t childExpressionIndex = *in_out_watCodeByteIndex;
        ASSERT_TOKEN(0, False)

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
            if (parseType(parsingStructure, in_out_watCodeByteIndex, False)) continue;
            if (NError.observeErrors()-errorsStart > 0) goto failureCleanUp;

            // TODO: ...Check other expressions...
            // ....xxx

        } else {
            NERROR(LOG_TAG, "Expected: \"%s)%s\" or \"%s(%s\", found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
            goto failureCleanUp;
        }
    } while (True);


failureCleanUp:
    // TODO: destroy the allocated data structure...
    *in_out_watCodeByteIndex = initialWatCodeByteIndex;
    return False;
}

static boolean parseType(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex, boolean throw) {

    #undef LOG_TAG
    #define LOG_TAG "ReferenceMachine.parseType()"
    int32_t initialWatCodeByteIndex = *in_out_watCodeByteIndex;
    char token[MAX_TOKEN_LENGTH];

    // Parse,
    // Example: (type (func (param i32 i32 i32 i32) (result i32)))
    // Note: currently, we can only define function types, so we don't have a separate function
    // to parse functions.
    ASSERT_TOKEN("(", throw)
    ASSERT_TOKEN("type", throw)
    ASSERT_TOKEN("(", True)
    ASSERT_TOKEN("func", True)

    // Parse child expressions,
    boolean parametersParsed = False;
    boolean resultParsed = False;
    do {
        // Get token,
        int32_t childExpressionIndex = *in_out_watCodeByteIndex;
        ASSERT_TOKEN(0, False)

        // ")",
        if (TEST_TOKEN(")")) {
            // Consume another closing parenthesis,
            ASSERT_TOKEN(")", True)
            // TODO: finalize and save the function type structure...
            return True;
        } else if (TEST_TOKEN("(")) {

            // Check child expressions,
            *in_out_watCodeByteIndex = childExpressionIndex; // Return to the child expression beginning.
            int32_t errorsStart = NError.observeErrors();

            // Check for type expression,
            // TODO: turn into a macro...
            if ((!parametersParsed) && parseFunctionParameters(parsingStructure, in_out_watCodeByteIndex, False)) {
                parametersParsed = True;
                continue;
            }
            if (NError.observeErrors()-errorsStart > 0) goto failureCleanUp;

            // Check for result expression,
            if (!resultParsed && parseResult(parsingStructure, in_out_watCodeByteIndex, False)) {
                resultParsed = True;
                continue;
            }
            if (NError.observeErrors()-errorsStart > 0) goto failureCleanUp;

            // This not be reached if the expression is correct,
            ASSERT_TOKEN(0, False)
            ASSERT_TOKEN(0, False)
            if (!parametersParsed && !resultParsed) {
                NERROR(LOG_TAG, "Expected: \"%s(param%s\" or \"%s(result%s or \"%s)%s\", found: \"%s(%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
                goto failureCleanUp;
            } else if (!parametersParsed) {
                NERROR(LOG_TAG, "Expected: \"%s(param%s\" or \"%s)%s\", found: \"%s(%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
                goto failureCleanUp;
            } else if (!resultParsed) {
                NERROR(LOG_TAG, "Expected: \"%s(result%s\" or \"%s)%s\", found: \"%s(%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
                goto failureCleanUp;
            } else {
                NERROR(LOG_TAG, "Expected: \"%s)%s\", found: \"%s(%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
                goto failureCleanUp;
            }

        } else {
            if (!parametersParsed || !resultParsed) {
                NERROR(LOG_TAG, "Expected: \"%s)%s\" or \"%s(%s\", found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
            } else {
                NERROR(LOG_TAG, "Expected: \"%s)%s\", found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
            }
            goto failureCleanUp;
        }
    } while (True);

failureCleanUp:
    // TODO: destroy the allocated data structure...
    *in_out_watCodeByteIndex = initialWatCodeByteIndex;
    return False;
}

static boolean parseDataType(const char* token) {

    if (TEST_TOKEN("i32")) {
        // TODO: add this i32 parameter...
    } else if (TEST_TOKEN("i64")) {
        // TODO: add this i64 parameter...
    } else if (TEST_TOKEN("f64")) {
        // TODO: add this f64 parameter...
    } else {
        return False;
    }
    return True;
}

static boolean parseFunctionParameters(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex, boolean throw) {

    #undef LOG_TAG
    #define LOG_TAG "ReferenceMachine.parseFunctionParameters()"
    int32_t initialWatCodeByteIndex = *in_out_watCodeByteIndex;
    char token[MAX_TOKEN_LENGTH];

    // Parse,
    // Example: (param i32 i32 i32 i32)
    ASSERT_TOKEN("(", throw)
    ASSERT_TOKEN("param", throw)

    // Parse arguments,
    int32_t parametersCount=0;
    do {
        // Get token,
        ASSERT_TOKEN(0, False)

        // ")",
        if (TEST_TOKEN(")")) {
            if (parametersCount > 0) {
                // TODO: finalize and save the parameters to the current function...
                return True;
            } else {
                NERROR(LOG_TAG, "Found param structure without any parameters");
                goto failureCleanUp;
            }
        } else if (parseDataType(token)) {
                parametersCount++;
                // TODO: add this parameter...
        } else {
            NERROR(LOG_TAG, "Expected: \"%s)%s\" or a %sdata-type%s, found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
            goto failureCleanUp;
        }
    } while (True);

failureCleanUp:
    *in_out_watCodeByteIndex = initialWatCodeByteIndex;
    return False;
}

static boolean parseResult(struct ParsingStructure* parsingStructure, int32_t* in_out_watCodeByteIndex, boolean throw) {

    #undef LOG_TAG
    #define LOG_TAG "ReferenceMachine.parseResult()"
    int32_t initialWatCodeByteIndex = *in_out_watCodeByteIndex;
    char token[MAX_TOKEN_LENGTH];

    // Parse,
    // Example: (result i32)
    ASSERT_TOKEN("(", throw)
    ASSERT_TOKEN("result", throw)

    // Parse result type,
    ASSERT_TOKEN(0, False)
    if (!parseDataType(token)) {
        NERROR(LOG_TAG, "Expected: a %sdata-type%s, found: \"%s%s%s\"", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), token, NTCOLOR(STREAM_DEFAULT));
        goto failureCleanUp;
    }

    ASSERT_TOKEN(")", True)
    return True;

failureCleanUp:
    *in_out_watCodeByteIndex = initialWatCodeByteIndex;
    return False;
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

/* Statements still not handled:
(func $popBack32Bit (type 3) (param i32 i32) (result i32) (local i32)
(table 62 62 funcref)
(memory 2)
(global (mut i32) (i32.const 68272))
(export "memory" (memory 0))
(export "jump" (func $jump))
(export "NWM" (global 5))
(elem (;0;) (i32.const 1) func $initialize $terminate $strlen $startsWith $equals $copy $parseInteger $NWM_createReferenceWasmMachine $NWM_createReferenceWasmMachineInHeap $getTime $initialize.1 $destroy $destroyAndFree $vAppend $append $set $get $create $replace $length $create.1 $createInHeap $destroy.1 $clear $pushBack $popBack $pushBack32Bit $popBack32Bit $get.1 $set.1 $size $terminate.1 $observeErrors $pushError $pushAndPrintError $popErrors $destroyAndFreeErrors $popDestroyAndFreeErrors $logAndTerminate $create.2 $createInHeap.1 $destroy.2 $destroyAndFree.1 $emplaceBack $pushBack.1 $popBack.1 $get.2 $size.1 $parseWatCode $NWM_destroyReferenceMachine $NMain $nMalloc $nFree $nMemset $nMemcpy $nLogI $nLogW $nLogE $getTime.1 $isNaN $isInf)
(data (;0;) (i32.const 1024) "\01\00\00\00\02\00\00\00\03\00\00\00\04\00\00\00\05\00\00\00\06\00\00\00\07\00\00\00NCString.parseInteger()\00Integer length can't exceed 10 digits and a sign. Found: %s%s\00Only digits from 0 to 9 are allowed. Found: %s%s\00Value too small to fit in a 32 bit integer: %s%s\00\08\00\00\00\09\00\00\00\0a\00\00\00\0b\00\00\00\0c\00\00\00\0d\00\00\00\0e\00\00\00\0f\00\00\00\10\00\00\00\11\00\00\00\12\00\00\00\13\00\00\00\14\00\00\00NString.vAppend\00Expected \22%sd%s\22 after \22%s%%l%s\22 in format string: %s%s\00Unexpected sequence: \22%s%%%c%s\22 in format string: %s%s\00\00%s\00%c\00\00\00\15\00\00\00\16\00\00\00\17\00\00\00\18\00\00\00\19\00\00\00\1a\00\00\00\1b\00\00\00\1c\00\00\00\1d\00\00\00\1e\00\00\00\1f\00\00\00NByteVector.get()\00Index out of bound: %d\00\00\00\00 \00\00\00!\00\00\00\22\00\00\00#\00\00\00$\00\00\00%\00\00\00&\00\00\00'\00\00\00\00NError\00Error tag exceeded maximum length\00Error message exceeded maximum length\00%s\00Unhandled errors\00%sUnhandled errors count: %d\00  %s: %s\00  %s\00\00(\00\00\00)\00\00\00*\00\00\00+\00\00\00,\00\00\00-\00\00\00.\00\00\00/\00\00\000\00\00\00ReferenceMachine.parseModule()\00(\00module\00)\00Expected: \22%s)%s\22 or \22%s(%s\22, found: \22%s%s%s\22\00EOF\00TOKEN exceeded maximum length: %s\00Expected: \22%s%s%s\22, found: \22%s%s%s\22\00found token: %s%s\00ReferenceMachine.parseType()\00type\00;\004\00\00\005\00\00\006\00\00\007\00\00\008\00\00\009\00\00\00:\00\00\00;\00\00\00<\00\00\00=\00\00\00\1b[0m\00\1b[0;30m\00\1b[0;31m\00\1b[0;32m\00\1b[0;33m\00\1b[0;34m\00\1b[0;35m\00\1b[0;36m\00\1b[0;37m\00\1b[1;30m\00\1b[1;31m\00\1b[1;32m\00\1b[1;33m\00\1b[1;34m\00\1b[1;35m\00\1b[1;36m\00\1b[1;37m\00\1b[4;30m\00\1b[4;31m\00\1b[4;32m\00\1b[4;33m\00\1b[4;34m\00\1b[4;35m\00\1b[4;36m\00\1b[4;37m\00\1b[40m\00\1b[41m\00\1b[42m\00\1b[43m\00\1b[44m\00\1b[45m\00\1b[46m\00\1b[47m\00\1b[0;90m\00\1b[0;91m\00\1b[0;92m\00\1b[0;93m\00\1b[0;94m\00\1b[0;95m\00\1b[0;96m\00\1b[0;97m\00\1b[1;90m\00\1b[1;91m\00\1b[1;92m\00\1b[1;93m\00\1b[1;94m\00\1b[1;95m\00\1b[1;96m\00\1b[1;97m\00\1b[0;100m\00\1b[0;101m\00\1b[0;102m\00\1b[0;103m\00\1b[0;104m\00\1b[0;105m\00\1b[0;106m\00\1b[0;107m\00\1bNOMoneSD\00\00\bc\07\00\00\c1\07\00\00\c9\07\00\00\d1\07\00\00\d9\07\00\00\e1\07\00\00\e9\07\00\00\f1\07\00\00\f9\07\00\00\01\08\00\00\09\08\00\00\11\08\00\00\19\08\00\00!\08\00\00)\08\00\001\08\00\009\08\00\00A\08\00\00I\08\00\00Q\08\00\00Y\08\00\00a\08\00\00i\08\00\00q\08\00\00y\08\00\00\81\08\00\00\87\08\00\00\8d\08\00\00\93\08\00\00\99\08\00\00\9f\08\00\00\a5\08\00\00\ab\08\00\00\b1\08\00\00\b9\08\00\00\c1\08\00\00\c9\08\00\00\d1\08\00\00\d9\08\00\00\e1\08\00\00\e9\08\00\00\f1\08\00\00\f9\08\00\00\01\09\00\00\09\09\00\00\11\09\00\00\19\09\00\00!\09\00\00)\09\00\001\09\00\00:\09\00\00C\09\00\00L\09\00\00U\09\00\00^\09\00\00g\09\00\00p\09\00\00y\09\00\00\c9\07\00\00\d9\07\00\00\09\09\00\00\f9\08\00\00(module)\00Parsing result\00%s\0a\00True\00False\00"))
*/