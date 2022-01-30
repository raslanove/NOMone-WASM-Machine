
#include <ReferenceMachine.h>
#include <WasmMachine.h>

#include <NSystemUtils.h>
#include <NCString.h>
#include <NError.h>

#include <NCC.h>

#define NWM_VERBOSE 1

///////////////////////////////////////////////////////////////////////////
// Types
///////////////////////////////////////////////////////////////////////////

typedef enum { TYPE_NONE=0, TYPE_INT32, TYPE_INT64, TYPE_FLOAT32, TYPE_FLOAT64 } DataType; // We can have a void return value.
typedef enum { TYPE_MEMORY, TYPE_GLOBAL, TYPE_FUNC } ExportType;
typedef enum {
    INST_i32_const,
    INST_i32_load8_u,
    INST_local_get,
    INST_local_set,
    INST_local_tee,
    INST_i32_add,
    INST_i32_and,
    INST_loop,
    INST_block,
    INST_end,
    INST_i32_eq,
    INST_i32_eqz,
    INST_br,
    INST_br_if,
    INST_call_indirect,
    INST_return
} InstructionType;

union Value {
    int32_t int32;
    int64_t int64;
    float float32;
    double float64;
};

struct Type {
    struct NVector parameterTypes;   // DataType
    DataType resultType;
};

struct Instruction {
    InstructionType type;
    union Value argument;
};

struct Function {
    struct NString name;
    int32_t typeIndex;
    struct Type* type;
    struct NVector localVariablesTypes;  // DataType
    struct NVector instructions;         // Instruction
};

struct Table {
    struct NVector functions;            // Function*
};

struct Memory {
    struct NByteVector data;
};

struct Global {
    DataType type;
    union Value value;
    boolean mutable;
};

struct Export {
    ExportType type;
    uint32_t index;
};

struct Module {
    struct Memory* memory;        // Could be imported from another machine (or parent code).
    struct Table* functionsTable; // Could be imported from another machine (or parent code).
    boolean memoryImported, tableImported;

    struct NVector types;         // Type*
    struct NVector functions;     // Function*
    struct NVector globals;       // Global
    struct NVector exports;       // Export
};

struct ReferenceMachineData {
    struct NCC* cc;
    struct NVector modules;       // Module*
};

struct ParsingData {
    struct NVector* modules;
};

///////////////////////////////////////////////////////////////////////////
// Reference machine population (common)
///////////////////////////////////////////////////////////////////////////

static void printMatch(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    NLOGI("ReferenceMachine", "ruleName: %s, variablesCount: %d", NString.get(ruleName), variablesCount);
    for (int32_t i=0; i<variablesCount; i++) {
        struct NCC_Variable variable;
        NCC_getRuleVariable(ncc, i, &variable);
        NLOGI("ReferenceMachine", "            Name: %s%s%s, Value: %s%s%s", NTCOLOR(HIGHLIGHT), variable.name, NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NString.get(&variable.value), NTCOLOR(STREAM_DEFAULT));
    }
    NLOGI("", "");
}

///////////////////////////////////////////////////////////////////////////
// Type
///////////////////////////////////////////////////////////////////////////

static DataType getDataTypeFromString(const char* valueString) {
    if (NCString.equals(valueString, "i32")) {
        return TYPE_INT32  ;
    } else if (NCString.equals(valueString, "i64")) {
        return TYPE_INT64  ;
    } else if (NCString.equals(valueString, "f32")) {
        return TYPE_FLOAT32;
    } else if (NCString.equals(valueString, "f64")) {
        return TYPE_FLOAT64;
    }
    return 0;
}

static void pushRuleParameterTypes(struct NCC* ncc, int32_t variablesCount, struct NVector* outVector) {

    // Push variable types,
    for (int32_t i=0; i<variablesCount; i++) {
        struct NCC_Variable variable; NCC_getRuleVariable(ncc, i, &variable);
        DataType parameterType = getDataTypeFromString(NString.get(&variable.value));
        NVector.pushBack(outVector, &parameterType);
    }
}

#define GET_CURRENT_TYPE \
    struct ParsingData* parsingData = ncc->extraData; \
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules); \
    struct Type* type = *(struct Type**) NVector.getLast(&module->types)

static void onType_Parameters(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_TYPE;
    pushRuleParameterTypes(ncc, variablesCount, &type->parameterTypes);
}

static void onType_Result(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_TYPE;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    type->resultType = getDataTypeFromString(NString.get(&variable.value));
}

static struct Type* createType() {
    struct Type* type = NMALLOC(sizeof(struct Type), "ReferenceMachine.createType() type");
    NSystemUtils.memset(type, 0, sizeof(struct Type));

    NVector.initialize(&type->parameterTypes, 0, sizeof(DataType));
    return type;
}

static void destroyAndFreeType(struct Type* type) {
    NVector.destroy(&type->parameterTypes);
    NFREE(type, "ReferenceMachine.destroyAndFreeType() type");
}

static void onType_Start(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    #ifdef NWM_VERBOSE
    NLOGI("ReferenceMachine", "Type->Start");
    #endif
    struct ParsingData* parsingData = ncc->extraData;

    // Create a new type in the current module,
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules);
    struct Type* newType = createType();
    NVector.pushBack(&module->types, &newType);
}

///////////////////////////////////////////////////////////////////////////
// Func
///////////////////////////////////////////////////////////////////////////

#define GET_CURRENT_FUNCTION \
    struct ParsingData* parsingData = ncc->extraData; \
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules); \
    struct Function* function = *(struct Function**) NVector.getLast(&module->functions)

static void onFunc_Identifier(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_FUNCTION;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    NString.set(&function->name, "%s", NString.get(&variable.value));
}

static void onFunc_TypeIndex(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_FUNCTION;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    function->typeIndex = NCString.parseInteger(NString.get(&variable.value));
}

static void onFunc_Parameters(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_FUNCTION;
    if (!function->type) function->type = createType();
    pushRuleParameterTypes(ncc, variablesCount, &function->type->parameterTypes);
}

static void onFunc_Result(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_FUNCTION;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    if (!function->type) function->type = createType();
    function->type->resultType = getDataTypeFromString(NString.get(&variable.value));
}

static void onFunc_Local(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_FUNCTION;
    pushRuleParameterTypes(ncc, variablesCount, &function->localVariablesTypes);
}

static struct Function* createFunction() {

    struct Function* function = NMALLOC(sizeof(struct Function), "ReferenceMachine.createFunction() function");
    NSystemUtils.memset(function, 0, sizeof(struct Function));

    NString.initialize(&function->name, "");
    NVector.initialize(&function->localVariablesTypes, 0, sizeof(DataType));
    NVector.initialize(&function->instructions, 0, sizeof(struct Instruction));
    function->typeIndex = -1; // Not set. Can't use 0 because it's a valid type index.

    return function;
}

static void destroyAndFreeFunction(struct Function* function) {
    NString.destroy(&function->name);
    NVector.destroy(&function->localVariablesTypes);
    NVector.destroy(&function->instructions);
    if (function->type) destroyAndFreeType(function->type);
    NFREE(function, "ReferenceMachine.destroyAndFreeFunction() function");
}

static void onFunc_Start(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    #ifdef NWM_VERBOSE
    NLOGI("ReferenceMachine", "Func->Start");
    #endif
    struct ParsingData* parsingData = ncc->extraData;

    // Create a new function in the current module,
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules);

    struct Function* newFunction = createFunction();
    NVector.pushBack(&module->functions, &newFunction);
}

static boolean typesEqual(struct Type* type1, struct Type* type2) {

    if (type1->resultType != type2->resultType) return False;
    if (NVector.size(&type1->parameterTypes) != NVector.size(&type2->parameterTypes)) return False;

    for (int32_t i=NVector.size(&type1->parameterTypes)-1; i>=0; i--) {
        DataType dataType1 = *(DataType*) NVector.get(&type1->parameterTypes, i);
        DataType dataType2 = *(DataType*) NVector.get(&type2->parameterTypes, i);
        if (dataType1 != dataType2) return False;
    }
    return True;
}

static void onFunc_End(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    GET_CURRENT_FUNCTION;

    // Validate type and type index if both provided. Find type index if not provided. Add a new one if needed,

    if (function->typeIndex==-1 && !function->type) {
        // A function with no parameters, result or type index. Treat it as one with an empty type specified,
        function->type = createType();
    }
    
    int32_t typesCount = NVector.size(&module->types);
    if (function->typeIndex!=-1 && function->type) {    
        // Function type inlined and type index also specified (why though?),
        if (!typesEqual(function->type, *(struct Type**) NVector.get(&module->types, function->typeIndex))) {
            NERROR("ReferenceMachine.onFunc_End()", "Function inlined type and indexed type don't match.");
        }
    } else if (function->type) {    
        // Function type inlined, but no type index specified,
        // Find if a matching type exists,        
        for (int32_t i=typesCount-1; i>=0; i--) {
            if (typesEqual(function->type, *(struct Type**) NVector.get(&module->types, i))) {
                function->typeIndex = i;
                break;
            }
        }
        
        if (function->typeIndex==-1) {
            // No matching type found, add a new one,        
            struct Type* newType = createType();
            NVector.pushBack(&module->types, &newType);
            newType->resultType = function->type->resultType;
            for (int32_t i=NVector.size(&function->type->parameterTypes)-1; i>=0; i--) {
                DataType dataType = *(DataType*) NVector.get(&function->type->parameterTypes, i);
                NVector.pushBack(&newType->parameterTypes, &dataType);
            }
            function->typeIndex = typesCount;
        }

    } else {
        // Function type index specified,
        if (function->typeIndex>=typesCount) {
            NERROR("ReferenceMachine.onFunc_End()", "Function indexed type doesn't exist.");
        }
    }

    // Dispose of the function type. It was there only because the specification allowed it, but after validation
    // it's quite useless for us,
    if (function->type) destroyAndFreeType(function->type);
    function->type = 0;    
}

///////////////////////////////////////////////////////////////////////////
// Instructions
///////////////////////////////////////////////////////////////////////////

static void createAndPushInstruction(struct NCC* ncc, InstructionType instructionType, union Value argument) {
    GET_CURRENT_FUNCTION;
    struct Instruction* instruction = NVector.emplaceBack(&function->instructions);
    instruction->type = instructionType;
    instruction->argument = argument;
}

static void createAndPushInstructionNoArgument(struct NCC* ncc, InstructionType instructionType) {
    union Value value;
    value.int32 = 0;
    createAndPushInstruction(ncc, instructionType, value);
}

static void createAndPushInstructionWithInt32Argument(struct NCC* ncc, InstructionType instructionType) {
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    union Value value;
    value.int32 = NCString.parseInteger(NString.get(&variable.value));
    createAndPushInstruction(ncc, instructionType, value);
}

static void onInstruction_i32_const    (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionWithInt32Argument(ncc, INST_i32_const    ); }
static void onInstruction_i32_load8_u  (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionNoArgument       (ncc, INST_i32_load8_u  ); }
static void onInstruction_local_get    (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionWithInt32Argument(ncc, INST_local_get    ); }
static void onInstruction_local_set    (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionWithInt32Argument(ncc, INST_local_set    ); }
static void onInstruction_local_tee    (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionWithInt32Argument(ncc, INST_local_tee    ); }
static void onInstruction_i32_add      (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionNoArgument       (ncc, INST_i32_add      ); }
static void onInstruction_i32_and      (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionNoArgument       (ncc, INST_i32_and      ); }
static void onInstruction_loop         (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionNoArgument       (ncc, INST_loop         ); }
static void onInstruction_block        (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionNoArgument       (ncc, INST_block        ); }
static void onInstruction_end          (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionNoArgument       (ncc, INST_end          ); }
static void onInstruction_i32_eq       (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionNoArgument       (ncc, INST_i32_eq       ); }
static void onInstruction_i32_eqz      (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionNoArgument       (ncc, INST_i32_eqz      ); }
static void onInstruction_br           (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionWithInt32Argument(ncc, INST_br           ); }
static void onInstruction_br_if        (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionWithInt32Argument(ncc, INST_br_if        ); }
static void onInstruction_call_indirect(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionWithInt32Argument(ncc, INST_call_indirect); }
static void onInstruction_return       (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionNoArgument       (ncc, INST_return       ); }

///////////////////////////////////////////////////////////////////////////
// Table
///////////////////////////////////////////////////////////////////////////

static struct Table* createTable() {
    struct Table* table = NMALLOC(sizeof(struct Table), "ReferenceMachine.createTable() table");
    NSystemUtils.memset(table, 0, sizeof(struct Table));
    NVector.initialize(&table->functions, 0, sizeof(struct Function*));
    return table;
}

static void destroyAndFreeTable(struct Table* table) {
    NVector.destroy(&table->functions);
    NFREE(table, "ReferenceMachine.destroyAndFreeTable() table");
}

static void onTable_End(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    #ifdef NWM_VERBOSE
    NLOGI("ReferenceMachine", "Table->End");
    #endif

    struct ParsingData* parsingData = ncc->extraData;
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules);

    if (module->functionsTable) {
        NERROR("ReferenceMachine.onTable_End()", "Table defined more than once.");
        return;
    }

    module->functionsTable = createTable();
}

///////////////////////////////////////////////////////////////////////////
// Memory
///////////////////////////////////////////////////////////////////////////

#define GET_CURRENT_MEMORY \
    struct ParsingData* parsingData = ncc->extraData; \
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules); \
    struct Memory* memory = module->memory

static void onMemory_PagesCount(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_MEMORY;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    NByteVector.resize(&memory->data, 64*1024*NCString.parseInteger(NString.get(&variable.value)));
}

static struct Memory* createMemory() {
    struct Memory* memory = NMALLOC(sizeof(struct Memory), "ReferenceMachine.createMemory() memory");
    NSystemUtils.memset(memory, 0, sizeof(struct Memory));
    NByteVector.initialize(&memory->data, 0);
    return memory;
}

static void destroyAndFreeMemory(struct Memory* memory) {
    NByteVector.destroy(&memory->data);
    NFREE(memory, "ReferenceMachine.destroyAndFreeMemory() memory");
}

static void onMemory_Start(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    #ifdef NWM_VERBOSE
    NLOGI("ReferenceMachine", "Memory->Start");
    #endif

    GET_CURRENT_MEMORY;
    if (memory) {
        NERROR("ReferenceMachine.onMemory_Start()", "Memory defined more than once.");
        return;
    }

    module->memory = createMemory();
}

///////////////////////////////////////////////////////////////////////////
// Global
///////////////////////////////////////////////////////////////////////////

#define GET_CURRENT_GLOBAL \
    struct ParsingData* parsingData = ncc->extraData; \
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules); \
    struct Global* global = NVector.getLast(&module->globals)

static void onGlobal_DataType(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_GLOBAL;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    global->type = getDataTypeFromString(NString.get(&variable.value));
}

static void onGlobal_Mutable(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_GLOBAL;
    global->mutable = True;
}

static void onGlobal_Value(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_GLOBAL;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 1, &variable);
    global->value.int64 = NCString.parse64BitInteger(NString.get(&variable.value));
}

static void onGlobal_Start(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    #ifdef NWM_VERBOSE
    NLOGI("ReferenceMachine", "Global->Start");
    #endif

    struct ParsingData* parsingData = ncc->extraData;
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules);
    struct Global* global = NVector.emplaceBack(&module->globals);

    NSystemUtils.memset(global, 0, sizeof(struct Global));
}

///////////////////////////////////////////////////////////////////////////
// Module
///////////////////////////////////////////////////////////////////////////

static struct Module* createModule() {

    struct Module* module = NMALLOC(sizeof(struct Module), "ReferenceMachine.createModule() module");
    NSystemUtils.memset(module, 0, sizeof(struct Module));

    NVector.initialize(&module->types    , 0, sizeof(struct Type*    ));
    NVector.initialize(&module->functions, 0, sizeof(struct Function*));
    NVector.initialize(&module->globals  , 0, sizeof(struct Global   ));
    NVector.initialize(&module->exports  , 0, sizeof(struct Export   ));

    return module;
}

static void destroyAndFreeModule(struct Module* module) {

    // Memory,
    if (!module->memoryImported) destroyAndFreeMemory(module->memory);

    // Table,
    if (!module->tableImported) destroyAndFreeTable(module->functionsTable);

    // Types,
    struct Type* type; while (NVector.popBack(&module->types, &type)) destroyAndFreeType(type);
    NVector.destroy(&module->types);

    // Functions,
    struct Function* function; while (NVector.popBack(&module->functions, &function)) destroyAndFreeFunction(function);
    NVector.destroy(&module->functions);

    NVector.destroy(&module->globals  );
    NVector.destroy(&module->exports  );

    NFREE(module, "ReferenceMachine.destroyAndFreeModule() module");
}

static void onModule_Start(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    #ifdef NWM_VERBOSE
    NLOGI("ReferenceMachine", "Module->Start");
    #endif
    struct ParsingData* parsingData = ncc->extraData;

    // Create a new module,
    struct Module* module = createModule();
    NVector.pushBack(parsingData->modules, &module);
}

///////////////////////////////////////////////////////////////////////////
// Language definition
///////////////////////////////////////////////////////////////////////////

static struct NCC* prepareCC() {

    struct NCC* cc = NCC_createNCC();

    // Not all WAT syntactic sugar features are implement. Maybe we can implement them at some point
    // in the future. Note that the text form is only implemented for convenience in learning and
    // debugging. The binary format should be much easier to parse and actually use, and should be
    // the form to be actually shipped in non-reference machines.
    //
    // Omitted features:
    //   - Implicit imports and exports.
    //   - Named parameters/locals/labels.
    //   - Folded and nested instructions.
    //   - Folded block instructions (if/then/else).
    //   - Named function types (signatures). See: https://stackoverflow.com/a/45714992
    //
    // For extensive examples of WAT code, see: https://github.com/mdn/webassembly-examples
    // and: https://developer.mozilla.org/en-US/docs/WebAssembly/Understanding_the_text_format
    //
    // For more information on how WASM works in Firefox, see: https://hacks.mozilla.org/category/webassembly/

    /////////////////////////////////////////////////////////////////////////////
    // Basic elements
    /////////////////////////////////////////////////////////////////////////////

    NCC_addRule(cc, "Empty", "", 0, False, False, False);
    NCC_addRule(cc, "Literal", "\x01-\xff", 0, False, False, False);
    NCC_addRule(cc, "WhiteSpace", "{\\ |\t|\r|\n}^*", 0, False, False, False);
    NCC_addRule(cc, "NotWhiteSpaceLiteral", "\x01-\x08 | \x0b-\x0c | \x0e-\x1f | \x21-\xff", 0, False, False, False);
    NCC_addRule(cc, "LineEnd", "\n|${Empty}", 0, False, False, False);
    NCC_addRule(cc, "LineComment", ";;*${LineEnd}", 0, False, False, False);
    NCC_addRule(cc, "BlockComment", "(;*;)", 0, False, False, False);
    NCC_addRule(cc, "", "{${WhiteSpace}|${LineComment}|${BlockComment}}^*", 0, False, False, False);
    NCC_addRule(cc, "PositiveInteger", "0 | {1-9 0-9^*}", 0, False, True, False);
    NCC_addRule(cc, "Integer", "0 | {\\-|${Empty} 1-9 0-9^*}", 0, False, True, False);
    // TODO: float......xxxx
    NCC_addRule(cc, "String", "\" { ${Literal}|{\\\\${Literal}} }^* \"", 0, False, False, False);
    // Note: maybe it's better to specify the supported ranges instead of excluding the unsupported ones?
    NCC_addRule(cc, "IdentifierLiteral", "\x01-\x08 | \x0b-\x0c | \x0e-\x1f | \x21-\x27 | \x2b-\xff", 0, False, False, False); // Note, characters like '*', ' ' and '-' must be escaped before being used in a literal-range expression.
    NCC_addRule(cc, "Identifier", "\\$${IdentifierLiteral}^*", 0, False, True, False);
    NCC_addRule(cc, "DataType", "{i32}|{i64}|{f32}|{f64}", 0, False, True, False);
    NCC_addRule(cc, "Parameters", "(${} param ${} ${DataType} ${} {${DataType} ${}}^*)", 0, False, False, False);
    NCC_addRule(cc, "Result", "(${} result ${} ${DataType} ${})", 0, False, False, False);
    NCC_addRule(cc, "TypeIndex", "(${} type ${} ${PositiveInteger} ${})", 0, False, False, False);

    /////////////////////////////////////////////////////////////////////////////
    // Instructions
    /////////////////////////////////////////////////////////////////////////////

    // Examples:
    //     i32.const 0
    //     i32.add
    //     block  ;; label = @1
    //     local.get 0
    //     i32.eqz
    //     br_if 0 (;@1;)
    //     call_indirect (type 0)
    //     end
    //
    // See: https://github.com/sunfishcode/wasm-reference-manual/blob/master/WebAssembly.md
    NCC_addRule(cc, "I-i32.const"    , "i32.const ${} ${Integer}"        , onInstruction_i32_const    , False, False, False);
    NCC_addRule(cc, "I-i32.load8_u"  , "i32.load8_u"                     , onInstruction_i32_load8_u  , False, False, False);
    NCC_addRule(cc, "I-local.get"    , "local.get ${} ${PositiveInteger}", onInstruction_local_get    , False, False, False);
    NCC_addRule(cc, "I-local.set"    , "local.set ${} ${PositiveInteger}", onInstruction_local_set    , False, False, False);
    NCC_addRule(cc, "I-local.tee"    , "local.tee ${} ${PositiveInteger}", onInstruction_local_tee    , False, False, False); // The same as set, but doesn't consume the value from the stack.
    NCC_addRule(cc, "I-i32.add"      , "i32.add"                         , onInstruction_i32_add      , False, False, False);
    NCC_addRule(cc, "I-i32.and"      , "i32.and"                         , onInstruction_i32_and      , False, False, False);
    NCC_addRule(cc, "I-loop"         , "loop"                            , onInstruction_loop         , False, False, False);
    NCC_addRule(cc, "I-block"        , "block"                           , onInstruction_block        , False, False, False);
    NCC_addRule(cc, "I-end"          , "end"                             , onInstruction_end          , False, False, False);
    NCC_addRule(cc, "I-i32.eq"       , "i32.eq"                          , onInstruction_i32_eq       , False, False, False);
    NCC_addRule(cc, "I-i32.eqz"      , "i32.eqz"                         , onInstruction_i32_eqz      , False, False, False);
    NCC_addRule(cc, "I-br"           , "br ${} ${PositiveInteger}"       , onInstruction_br           , False, False, False);
    NCC_addRule(cc, "I-br_if"        , "br_if ${} ${PositiveInteger}"    , onInstruction_br_if        , False, False, False);
    NCC_addRule(cc, "I-call_indirect", "call_indirect ${} ${TypeIndex}"  , onInstruction_call_indirect, False, False, False); // The function type is needed for type checking only.
                                                                                                                              // The indirect call pops the desired function index
                                                                                                                              // from the stack, or from a nested instruction.
    NCC_addRule(cc, "I-return"       , "return"                          , onInstruction_return       , False, False, False);


    NCC_addRule(cc, "Instruction", "${I-i32.const}     |"
                                   "${I-i32.load8_u}   |"
                                   "${I-local.get}     |"
                                   "${I-local.set}     |"
                                   "${I-local.tee}     |"
                                   "${I-i32.add}       |"
                                   "${I-i32.and}       |"
                                   "${I-loop}          |"
                                   "${I-block}         |"
                                   "${I-end}           |"
                                   "${I-i32.eq}        |"
                                   "${I-i32.eqz}       |"
                                   "${I-br}            |"
                                   "${I-br_if}         |"
                                   "${I-call_indirect} |"
                                   "${I-return}         ", 0, False, False, False);

    /////////////////////////////////////////////////////////////////////////////
    // Structures,
    /////////////////////////////////////////////////////////////////////////////

    // Type,
    //     Examples:
    //         (type (;0;) (func))
    //         (type (;1;) (func (param i32 i32 i64)))
    //         (type (;1;) (func (param i32 i32 i64) (result i32)))
    NCC_addRule(cc, "Type->Start", "", onType_Start, False, False, False);
    NCC_addRule(cc, "Type->Parameters", "${Parameters}", onType_Parameters, False, False, True);
    NCC_addRule(cc, "Type->Result", "${Result}", onType_Result, False, False, True);
    NCC_addRule(cc, "Type->Func", "(${} func ${} ${Type->Parameters}|${Empty} ${} ${Type->Result}|${Empty})", 0, False, False, False);
    NCC_addRule(cc, "Type", "${Type->Start} (${} type ${} ${Type->Func} ${})", 0, False, False, False);

    // Function,
    //     Example 1:
    //         (func $__wasm_call_ctors (type 0))
    //
    //     Example 2:
    //         (func $initialize (type 4) (param i32)
    //           block  ;; label = @1
    //             local.get 0
    //             i32.eqz
    //             br_if 0 (;@1;)
    //             local.get 0
    //             call_indirect (type 0)
    //           end)
    //
    //     Example 3:
    //         (func $strlen (type 6) (param i32) (result i32)
    //           (local i32 i32 i32)
    //           i32.const 0
    //           local.set 1
    //           loop  ;; label = @1
    //             local.get 0
    //             local.get 1
    //             i32.add
    //             local.set 2
    //             local.get 1
    //             i32.const 1
    //             i32.add
    //             local.tee 3
    //             local.set 1
    //             local.get 2
    //             i32.load8_u
    //             br_if 0 (;@1;)
    //           end
    //           local.get 3
    //           i32.const -1
    //           i32.add)
    //
    //     Example 4 (implicit import, not implemented):
    //         (func $i (import "imports" "imported_func") (param i32))
    //
    //     Example 5 (implicit export, not implemented):
    //         (func (export "exported_func")
    //           i32.const 42
    //           call $i))
    //
    //     Example 6 (named parameters/locals/labels, folded and nested instructions implemented):
    //         (func (export "accumulate") (param $ptr i32) (param $len i32) (result i32)
    //           (local $end i32)
    //           (local $sum i32)
    //           (local.set $end (i32.add (local.get $ptr) (i32.mul (local.get $len) (i32.const 4))))
    //           (block $break (loop $top
    //             (br_if $break (i32.eq (local.get $ptr) (local.get $end)))
    //             (local.set $sum (i32.add (local.get $sum)
    //                                      (i32.load (local.get $ptr))))
    //               (local.set $ptr (i32.add (local.get $ptr) (i32.const 4)))
    //               (br $top)
    //           ))
    //           (local.get $sum))
    NCC_addRule(cc, "Func->Start", "", onFunc_Start, False, False, False);
    NCC_addRule(cc, "Func->Identifier", "${Identifier}", onFunc_Identifier, False, False, True);
    NCC_addRule(cc, "Func->TypeIndex", "${TypeIndex}", onFunc_TypeIndex, False, False, True);
    NCC_addRule(cc, "Func->Parameters", "${Parameters}", onFunc_Parameters, False, False, True);
    NCC_addRule(cc, "Func->Result", "${Result}", onFunc_Result, False, False, True);
    NCC_addRule(cc, "Func->Local", "(${} local ${} ${DataType} ${} {${DataType} ${}}^*)", onFunc_Local, False, False, True);
    NCC_addRule(cc, "Func", "${Func->Start} (${} func ${} ${Func->Identifier} ${}"
                            "${Func->TypeIndex}  |${Empty} ${}"
                            "${Func->Parameters} |${Empty} ${}"
                            "${Func->Result}     |${Empty} ${}"
                            "${Func->Local}      |${Empty} ${}"
                            "{${Instruction} ${}}^*)", onFunc_End, False, False, True);

    // Table,
    //     Example:
    //         (table (;0;) 62 62 funcref)  ;; 62 is the number for functions we have. Here, it defines
    //                                      ;; the limits of the table (min and max sizes).
    //
    // See: https://developer.mozilla.org/en-US/docs/WebAssembly/Understanding_the_text_format#webassembly_tables
    NCC_addRule(cc, "Table->minSize", "${PositiveInteger}", 0, False, False, False);
    NCC_addRule(cc, "Table->maxSize", "${PositiveInteger}", 0, False, False, False);
    NCC_addRule(cc, "Table", "(${} table ${} {${Table->minSize}|${Empty} ${} ${Table->maxSize}}|${Empty} ${} funcref ${})", onTable_End, False, True, False);

    // Memory,
    NCC_addRule(cc, "Memory->Start", "", onMemory_Start, False, False, False);
    NCC_addRule(cc, "Memory->PagesCount", "${PositiveInteger}", onMemory_PagesCount, False, False, True);
    NCC_addRule(cc, "Memory", "${Memory->Start} (${} memory ${} ${Memory->PagesCount} ${})", 0, False, True, False);

    // Global,
    //     Examples:
    //         (global (;0;) (mut i32) (i32.const 68272))
    //         (global (;1;) i32 (i32.const 1024))
    NCC_addRule(cc, "Global->Start", "", onGlobal_Start, False, False, False);
    NCC_addRule(cc, "Global->DataType", "${DataType}", onGlobal_DataType, False, False, True);
    NCC_addRule(cc, "Global->Mutable", "(${} mut ${} ${Global->DataType} ${})", onGlobal_Mutable, False, False, False);
    NCC_addRule(cc, "Global->Value", "(${} ${DataType}.const ${} ${Integer} ${})", onGlobal_Value, False, False, True); // TODO: support float. Add two cases, integerType+integerValue and floatType+floatValue...
    NCC_addRule(cc, "Global", "${Global->Start} (${} global ${} ${Global->DataType}|${Global->Mutable} ${} ${Global->Value} ${})", 0, False, True, False);

    // Export,
    //     Examples:
    //         (export "memory" (memory 0))
    //         (export "NSystem" (global 1))
    //         (export "__wasm_call_ctors" (func $__wasm_call_ctors))
    NCC_addRule(cc, "MemoryIndex", "(${} memory ${} ${PositiveInteger} ${})", 0, False, False, False);
    NCC_addRule(cc, "GlobalIndex", "(${} global ${} ${PositiveInteger} ${})", 0, False, False, False);
    NCC_addRule(cc, "FuncName", "(${} func ${} ${Identifier} ${})", 0, False, False, False);
    NCC_addRule(cc, "Export", "(${} export ${} ${String} ${} ${MemoryIndex}|${GlobalIndex}|${FuncName} ${})", 0, False, True, False);

    // Element,
    //     Example:
    //         (elem (;0;) (i32.const 1) func $initialize $terminate $strlen)
    NCC_addRule(cc, "TableOffset", "(${} i32.const ${} ${PositiveInteger} ${})", 0, False, False, False);
    NCC_addRule(cc, "Element", "(${} elem ${} ${TableOffset}|${Empty} ${} func ${} ${Identifier} ${} {${Identifier} ${}}^*)", 0, False, True, False);

    // Data,
    //     Example:
    //         (data (;0;) (i32.const 1024) "NCString.parseInteger()\00Integer length can't exceed 10 digits and a sign. Found: %s%s\00\19\09\00\00!\09\00\00\c9\07\00\00\d9\07\00\00\09\09\00\00\f9\08\00\00(module)\00Parsing result\00%s\0a\00True\00False\00"))
    NCC_addRule(cc, "MemoryOffset", "(${} i32.const ${} ${PositiveInteger} ${})", 0, False, False, False);
    NCC_addRule(cc, "Data", "(${} data ${} ${MemoryOffset} ${} ${String} ${})", 0, False, True, False);

    // Module,
    NCC_addRule(cc, "Module->Start", "", onModule_Start, False, False, False);
    NCC_addRule(cc, "ModuleElement", "${Type} | ${Func} | ${Table} | ${Memory} | ${Global} | ${Export} | ${Element} | ${Data}", 0, False, False, False);
    NCC_addRule(cc, "Module", "${Module->Start} (${} module ${} {${ModuleElement}${}}^*)", 0, False, False, False);

    // Document,
    NCC_addRule(cc, "Document", "${} ${Module} ${} {${Module} ${}}^*", 0, True, False, True);

    return cc;
}

///////////////////////////////////////////////////////////////////////////
// Reference machine interface
///////////////////////////////////////////////////////////////////////////

static void destroyReferenceMachine(struct NWM_WasmMachine *machine) {
    machine->alive = False;

    struct ReferenceMachineData* machineData = machine->data;

    // Modules,
    struct Module* module;
    while (NVector.popBack(&machineData->modules, &module)) destroyAndFreeModule(module);
    NVector.destroy(&machineData->modules);

    // CC,
    struct ParsingData* parsingData = machineData->cc->extraData;
    NFREE(parsingData, "ReferenceMachine.destroyReferenceMachine() parsingData");
    NCC_destroyAndFreeNCC(machineData->cc);

    // The machine data itself,
    NFREE(machineData, "ReferenceMachine.destroyReferenceMachine() machineData");
}

static void destroyAndFreeReferenceMachine(struct NWM_WasmMachine *machine) {
    destroyReferenceMachine(machine);
    NFREE(machine, "ReferenceMachine.destroyAndFreeReferenceMachine() machine");
}

static boolean parseWatCode(struct NWM_WasmMachine *machine, const char* watCode) {

    struct ReferenceMachineData* machineData = machine->data;
    int32_t matchLength = NCC_match(machineData->cc, watCode);
    NLOGW("", "Match length: %d", matchLength);

    boolean success = (matchLength == NCString.length(watCode));
    if (!success) {
        // TODO: Note the matched length before failing to match (only take into account substitute nodes...
        // NLOGE("ReferenceMachine", "parseWatCode(): couldn't parse: %s%s%s", NTCOLOR(HIGHLIGHT), &watCode[maxMatchLength], NTCOLOR(STREAM_DEFAULT));
        return False;
    }

    return True;
}

struct NWM_WasmMachine *NWM_initializeReferenceMachine(struct NWM_WasmMachine *outputMachine) {

    NSystemUtils.memset(outputMachine, 0, sizeof(struct NWM_WasmMachine));

    outputMachine->alive = True;
    outputMachine->destroy = destroyReferenceMachine;
    outputMachine->destroyAndFree = destroyAndFreeReferenceMachine;
    outputMachine->parseWatCode = parseWatCode;

    // Initialize machine data,
    struct ReferenceMachineData* machineData = NMALLOC(sizeof(struct ReferenceMachineData), "ReferenceMachine.NWM_initializeReferenceMachine() machineData");
    NVector.initialize(&machineData->modules, 0, sizeof(struct Module*));

    // CC,
    struct NCC* ncc = prepareCC();
    struct ParsingData* parsingData = NMALLOC(sizeof(struct ParsingData), "ReferenceMachine.NWM_initializeReferenceMachine() parsingData");
    parsingData->modules = &machineData->modules;
    ncc->extraData = parsingData;
    machineData->cc = ncc;

    outputMachine->data = machineData;

    return outputMachine;
}

struct NWM_WasmMachine *NWM_createReferenceMachine() {
    struct NWM_WasmMachine* machine = NMALLOC(sizeof(struct NWM_WasmMachine), "ReferenceMachine.NWM_createReferenceMachine() machine");
    return NWM_initializeReferenceMachine(machine);
}
