
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

struct LocalVariable {
    DataType type;
    int32_t offset;
    int32_t sizeBytes;
};

struct Type {
    struct NVector parameters;       // LocalVariable
    int32_t parametersSizeBytes;
    DataType resultType;
};

struct Instruction {
    InstructionType type;
    union Value argument;
};

struct Function {
    struct Module* module;
    struct NString name;
    int32_t typeIndex;
    struct Type* type;
    struct NVector localVariables;   // LocalVariable
    int32_t localVariablesSizeBytes;
    struct NVector instructions;     // Instruction
};

struct Table {
    struct NVector functions;        // Function*
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
    struct NString name;
    ExportType type;
    uint32_t index;
};

struct Stack {
    struct NByteVector data;
};

/*
struct ReturnInfo {
};

struct CallStack { // Is this necessary?
    struct NVector data;   // ReturnInfo
};
*/

struct Module {
    struct Memory* memory;        // Could be imported from another machine (or parent code).
    struct Table* functionsTable; // Could be imported from another machine (or parent code).
    boolean memoryImported, tableImported;
    struct Stack stack;

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

static int32_t getDataTypeSizeBytes(DataType type) {
    switch (type) {
        case TYPE_INT32:
        case TYPE_FLOAT32:
            return 4;
        case TYPE_INT64:
        case TYPE_FLOAT64:
            return 8;
        default:
            NERROR("ReferenceMachine.getDataTypeSizeBytes()", "Unknown datatype: %s%d%s", NTCOLOR(HIGHLIGHT), type, NTCOLOR(STREAM_DEFAULT));
            return 0;
    }
}

static int32_t pushRuleParameterTypes(struct NCC* ncc, int32_t variablesCount, struct NVector* outVector) {

    // Push variable types,
    int32_t offset = 0;
    for (int32_t i=0; i<variablesCount; i++) {
        struct NCC_Variable variable; NCC_getRuleVariable(ncc, i, &variable);
        struct LocalVariable parameter;
        parameter.type = getDataTypeFromString(NString.get(&variable.value));
        parameter.offset = offset;
        parameter.sizeBytes = getDataTypeSizeBytes(parameter.type);
        offset += parameter.sizeBytes;
        NVector.pushBack(outVector, &parameter);
    }
    return offset;
}

#define GET_CURRENT_TYPE \
    struct ParsingData* parsingData = ncc->extraData; \
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules); \
    struct Type* type = *(struct Type**) NVector.getLast(&module->types)

static void onType_Parameters(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_TYPE;
    type->parametersSizeBytes = pushRuleParameterTypes(ncc, variablesCount, &type->parameters);
}

static void onType_Result(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_TYPE;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    type->resultType = getDataTypeFromString(NString.get(&variable.value));
}

static struct Type* createType() {
    struct Type* type = NMALLOC(sizeof(struct Type), "ReferenceMachine.createType() type");
    NSystemUtils.memset(type, 0, sizeof(struct Type));

    NVector.initialize(&type->parameters, 0, sizeof(struct LocalVariable));
    return type;
}

static void destroyAndFreeType(struct Type* type) {
    NVector.destroy(&type->parameters);
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
    pushRuleParameterTypes(ncc, variablesCount, &function->type->parameters);
}

static void onFunc_Result(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_FUNCTION;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    if (!function->type) function->type = createType();
    function->type->resultType = getDataTypeFromString(NString.get(&variable.value));
}

static void onFunc_Local(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_FUNCTION;
    function->localVariablesSizeBytes = pushRuleParameterTypes(ncc, variablesCount, &function->localVariables);
}

static struct Function* createFunction(struct Module* module) {

    struct Function* function = NMALLOC(sizeof(struct Function), "ReferenceMachine.createFunction() function");
    NSystemUtils.memset(function, 0, sizeof(struct Function));

    function->module = module;
    NString.initialize(&function->name, "");
    NVector.initialize(&function->localVariables, 0, sizeof(struct LocalVariable));
    NVector.initialize(&function->instructions, 0, sizeof(struct Instruction));
    function->typeIndex = -1; // Not set. Can't use 0 because it's a valid type index.

    return function;
}

static void destroyAndFreeFunction(struct Function* function) {
    NString.destroy(&function->name);
    NVector.destroy(&function->localVariables);
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

    struct Function* newFunction = createFunction(module);
    NVector.pushBack(&module->functions, &newFunction);
}

static boolean typesEqual(struct Type* type1, struct Type* type2) {

    if (type1->resultType != type2->resultType) return False;
    if (NVector.size(&type1->parameters) != NVector.size(&type2->parameters)) return False;

    for (int32_t i=NVector.size(&type1->parameters)-1; i>=0; i--) {
        struct LocalVariable parameter1 = *(struct LocalVariable*) NVector.get(&type1->parameters, i);
        struct LocalVariable parameter2 = *(struct LocalVariable*) NVector.get(&type2->parameters, i);
        if (parameter1.type != parameter2.type) return False;
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
            for (int32_t i=NVector.size(&function->type->parameters)-1; i>=0; i--) {
                struct LocalVariable parameter = *(struct LocalVariable*) NVector.get(&function->type->parameters, i);
                NVector.pushBack(&newType->parameters, &parameter);
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

    // Adjust the offsets of the local variables,
    struct Type* type = *(struct Type**) NVector.get(&module->types, function->typeIndex);
    if (type->parametersSizeBytes) {
        int32_t localVariablesCount = NVector.size(&function->localVariables);
        for (int32_t i=0; i<localVariablesCount; i++) {
            struct LocalVariable localVariable = *(struct LocalVariable*) NVector.get(&function->localVariables, i);
            localVariable.offset += type->parametersSizeBytes;
        }
    }
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
    union Value value;
    value.int32 = 0;
    struct NCC_Variable variable;
    if (NCC_popRuleVariable(ncc, &variable)) {
        value.int32 = NCString.parseInteger(NString.get(&variable.value));
        NCC_destroyVariable(&variable);
    }
    createAndPushInstruction(ncc, instructionType, value);
}

static void onInstruction_i32_const    (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionWithInt32Argument(ncc, INST_i32_const    ); }
static void onInstruction_i32_load8_u  (struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) { createAndPushInstructionWithInt32Argument(ncc, INST_i32_load8_u  ); }
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
// Export
///////////////////////////////////////////////////////////////////////////

#define GET_CURRENT_EXPORT \
    struct ParsingData* parsingData = ncc->extraData; \
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules); \
    struct Export* export = NVector.getLast(&module->exports)

static void onExport_Name(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_EXPORT;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    NString.set(&export->name, "%s", NString.get(&variable.value));
}

static void onExport_Memory(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_EXPORT;
    export->type = TYPE_MEMORY;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    export->index = NCString.parseInteger(NString.get(&variable.value));
}

static void onExport_Global(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_EXPORT;
    export->type = TYPE_GLOBAL;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    export->index = NCString.parseInteger(NString.get(&variable.value));
}

static void onExport_Func(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    GET_CURRENT_EXPORT;
    export->type = TYPE_FUNC;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    const char* identifier = NString.get(&variable.value);

    // Find the function with this identifier,
    for (int32_t i=NVector.size(&module->functions)-1; i>-1; i--) {
        struct Function* function = *(struct Function **) NVector.get(&module->functions, i);
        if (NCString.equals(NString.get(&function->name), identifier)) {
            export->index = i;
            return ;
        }
    }

    NERROR("ReferenceMachine.onExport_Func()", "Function %s%s%s index not found.", NTCOLOR(HIGHLIGHT), NString.get(&variable.value), NTCOLOR(STREAM_DEFAULT));
}

static void destroyExport(struct Export* export) {
    NString.destroy(&export->name);
}

static void onExport_Start(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    #ifdef NWM_VERBOSE
    NLOGI("ReferenceMachine", "Export->Start");
    #endif

    struct ParsingData* parsingData = ncc->extraData;
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules);
    struct Export* export = NVector.emplaceBack(&module->exports);

    NSystemUtils.memset(export, 0, sizeof(struct Export));
    NString.initialize(&export->name, "");
}

///////////////////////////////////////////////////////////////////////////
// Element
///////////////////////////////////////////////////////////////////////////

static void onElement_End(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    #ifdef NWM_VERBOSE
    NLOGI("ReferenceMachine", "Element->End");
    #endif

    struct ParsingData* parsingData = ncc->extraData;
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules);

    // Make sure there's a function table,
    if (!module->functionsTable) module->functionsTable = createTable();

    // Check if there's an offset,
    int32_t tableSize = NVector.size(&module->functionsTable->functions);
    int32_t offset = tableSize;
    int32_t currentVariableIndex = 0;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    if (NCString.equals(variable.name, "PositiveInteger")) {
        offset = NCString.parseInteger(NString.get(&variable.value));
        currentVariableIndex = 1;
    }

    // Prepare the table,
    int32_t minNewSize = offset + variablesCount - currentVariableIndex;
    if (tableSize < minNewSize) NVector.resize(&module->functionsTable->functions, minNewSize);

    // Populate the table,
    for (; currentVariableIndex<variablesCount; currentVariableIndex++) {
        NCC_getRuleVariable(ncc, currentVariableIndex, &variable);
        const char* identifier = NString.get(&variable.value);

        // Find the function with this identifier,
        boolean found = False;
        for (int32_t i=NVector.size(&module->functions)-1; i>-1; i--) {
            struct Function* function = *(struct Function **) NVector.get(&module->functions, i);
            if (NCString.equals(NString.get(&function->name), identifier)) {
                struct Function** tableEntry = NVector.get(&module->functionsTable->functions, offset);
                *tableEntry = function;
                found = True;
                break;
            }
        }

        if (!found) NERROR("ReferenceMachine.onElement_End()", "Function %s%s%s not found.", NTCOLOR(HIGHLIGHT), NString.get(&variable.value), NTCOLOR(STREAM_DEFAULT));
        offset++;
    }
}

///////////////////////////////////////////////////////////////////////////
// Data
///////////////////////////////////////////////////////////////////////////

static void onData_End(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {

    #ifdef NWM_VERBOSE
    NLOGI("ReferenceMachine", "Data->End");
    #endif

    struct ParsingData* parsingData = ncc->extraData;
    struct Module* module = *(struct Module**) NVector.getLast(parsingData->modules);

    // Make sure there's a memory,
    if (!module->memory) module->memory = createMemory();

    // Check if there's an offset,
    uint32_t offset = 0;
    int32_t currentVariableIndex = 0;
    struct NCC_Variable variable; NCC_getRuleVariable(ncc, 0, &variable);
    if (NCString.equals(variable.name, "PositiveInteger")) {
        offset = NCString.parseInteger(NString.get(&variable.value));
        currentVariableIndex = 1;
    }

    // Unescape the data,
    NCC_getRuleVariable(ncc, currentVariableIndex, &variable);
    const char* data = NString.get(&variable.value);

    struct NByteVector* memoryData = &module->memory->data;
    uint32_t memorySize = NByteVector.size(memoryData);
    uint32_t currentByteIndex=0;
    uint8_t currentByte;
    while ((currentByte = data[currentByteIndex])) {

        // Check if this is an escape sequence,
        if (currentByte == '\\') {

            // Note: whenever the '\' character itself is required, a "\5c" is used.

            // The following two digits should be a hexadecimal value,
            char hexByte1 = data[currentByteIndex + 1];
            if (hexByte1 >= '0' && hexByte1 <= '9') {
                currentByte = (hexByte1 - '0') << 4;
            } else if (hexByte1 >= 'a' && hexByte1 <= 'f') {
                currentByte = (10 + hexByte1 - 'a') << 4;
            } else {
                NERROR("ReferenceMachine.onData_End()", "Malformed escape sequence. Expecting %s\\0-9|a-f%s, found %s\\%c%s.", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), hexByte1, NTCOLOR(STREAM_DEFAULT));
            }

            char hexByte2 = data[currentByteIndex + 2];
            if (hexByte2 >= '0' && hexByte2 <= '9') {
                currentByte += hexByte2 - '0';
            } else if (hexByte2 >= 'a' && hexByte2 <= 'f') {
                currentByte += 10 + hexByte2 - 'a';
            } else {
                NERROR("ReferenceMachine.onData_End()", "Malformed escape sequence. Expecting %s\\{0-9|a-f}{0-9|a-f}%s, found %s\\%c%c%s.", NTCOLOR(HIGHLIGHT), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), hexByte1, hexByte2, NTCOLOR(STREAM_DEFAULT));
            }

            currentByteIndex+=2;
        }

        // Set the byte,
        if (offset >= memorySize) {
            NByteVector.pushBack(memoryData, currentByte);
            memorySize = NByteVector.size(memoryData);
        } else {
            NByteVector.set(memoryData, offset, currentByte);
        }
        offset++;
        currentByteIndex++;
    }
}

///////////////////////////////////////////////////////////////////////////
// Module
///////////////////////////////////////////////////////////////////////////

static struct Module* createModule() {

    struct Module* module = NMALLOC(sizeof(struct Module), "ReferenceMachine.createModule() module");
    NSystemUtils.memset(module, 0, sizeof(struct Module));

    NByteVector.initialize(&module->stack.data, 0);

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
    if (!module->tableImported && module->functionsTable) destroyAndFreeTable(module->functionsTable);

    // Stack,
    NByteVector.destroy(&module->stack.data);

    // Types,
    struct Type* type; while (NVector.popBack(&module->types, &type)) destroyAndFreeType(type);
    NVector.destroy(&module->types);

    // Functions,
    struct Function* function; while (NVector.popBack(&module->functions, &function)) destroyAndFreeFunction(function);
    NVector.destroy(&module->functions);

    // Exports,
    for (int32_t i=NVector.size(&module->exports)-1; i>-1; i--) destroyExport(NVector.get(&module->exports, i));
    NVector.destroy(&module->exports);

    // Globals,
    NVector.destroy(&module->globals);

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
    NCC_addRule(cc, "String", "\" { ${Literal}|{\\\\${Literal}} }^* \"", 0, False, True, False);
    // Note: maybe it's better to specify the supported ranges instead of excluding the unsupported ones?
    NCC_addRule(cc, "IdentifierLiteral", "\x01-\x08 | \x0b-\x0c | \x0e-\x1f | \x21-\x27 | \x2b-\xff", 0, False, False, False); // Note, characters like '*', ' ' and '-' must be escaped before being used in a literal-range expression.
    NCC_addRule(cc, "Identifier", "\\$${IdentifierLiteral}^*", 0, False, True, False);
    NCC_addRule(cc, "DataType", "{i32}|{i64}|{f32}|{f64}", 0, False, True, False);
    NCC_addRule(cc, "Parameters", "(${} param ${} ${DataType} ${} {${DataType} ${}}^*)", 0, False, False, False);
    NCC_addRule(cc, "Result", "(${} result ${} ${DataType} ${})", 0, False, False, False);
    NCC_addRule(cc, "TypeIndex", "(${} type ${} ${PositiveInteger} ${})", 0, False, False, False);
    NCC_addRule(cc, "Offset", "offset ${} = ${} ${PositiveInteger}", 0, False, False, False);

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
    NCC_addRule(cc, "I-i32.const"    , "i32.const ${} ${Integer}"           , onInstruction_i32_const    , False, False, False);
    NCC_addRule(cc, "I-i32.load8_u"  , "i32.load8_u ${} ${Offset}|${Empty}" , onInstruction_i32_load8_u  , False, False, False);
    NCC_addRule(cc, "I-local.get"    , "local.get ${} ${PositiveInteger}"   , onInstruction_local_get    , False, False, False);
    NCC_addRule(cc, "I-local.set"    , "local.set ${} ${PositiveInteger}"   , onInstruction_local_set    , False, False, False);
    NCC_addRule(cc, "I-local.tee"    , "local.tee ${} ${PositiveInteger}"   , onInstruction_local_tee    , False, False, False); // The same as set, but doesn't consume the value from the stack.
    NCC_addRule(cc, "I-i32.add"      , "i32.add"                            , onInstruction_i32_add      , False, False, False);
    NCC_addRule(cc, "I-i32.and"      , "i32.and"                            , onInstruction_i32_and      , False, False, False);
    NCC_addRule(cc, "I-loop"         , "loop"                               , onInstruction_loop         , False, False, False);
    NCC_addRule(cc, "I-block"        , "block"                              , onInstruction_block        , False, False, False);
    NCC_addRule(cc, "I-end"          , "end"                                , onInstruction_end          , False, False, False);
    NCC_addRule(cc, "I-i32.eq"       , "i32.eq"                             , onInstruction_i32_eq       , False, False, False);
    NCC_addRule(cc, "I-i32.eqz"      , "i32.eqz"                            , onInstruction_i32_eqz      , False, False, False);
    NCC_addRule(cc, "I-br"           , "br ${} ${PositiveInteger}"          , onInstruction_br           , False, False, False);
    NCC_addRule(cc, "I-br_if"        , "br_if ${} ${PositiveInteger}"       , onInstruction_br_if        , False, False, False);
    NCC_addRule(cc, "I-call_indirect", "call_indirect ${} ${TypeIndex}"     , onInstruction_call_indirect, False, False, False); // The function type is needed for type checking only.
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
    NCC_addRule(cc, "Table", "(${} table ${} {${Table->minSize}|${Empty} ${} ${Table->maxSize}}|${Empty} ${} funcref ${})", onTable_End, False, False, True);

    // Memory,
    NCC_addRule(cc, "Memory->Start", "", onMemory_Start, False, False, False);
    NCC_addRule(cc, "Memory->PagesCount", "${PositiveInteger}", onMemory_PagesCount, False, False, True);
    NCC_addRule(cc, "Memory", "${Memory->Start} (${} memory ${} ${Memory->PagesCount} ${})", 0, False, False, False);

    // Global,
    //     Examples:
    //         (global (;0;) (mut i32) (i32.const 68272))
    //         (global (;1;) i32 (i32.const 1024))
    NCC_addRule(cc, "Global->Start", "", onGlobal_Start, False, False, False);
    NCC_addRule(cc, "Global->DataType", "${DataType}", onGlobal_DataType, False, False, True);
    NCC_addRule(cc, "Global->Mutable", "(${} mut ${} ${Global->DataType} ${})", onGlobal_Mutable, False, False, False);
    NCC_addRule(cc, "Global->Value", "(${} ${DataType}.const ${} ${Integer} ${})", onGlobal_Value, False, False, True); // TODO: support float. Add two cases, integerType+integerValue and floatType+floatValue...
    NCC_addRule(cc, "Global", "${Global->Start} (${} global ${} ${Global->DataType}|${Global->Mutable} ${} ${Global->Value} ${})", 0, False, False, False);

    // Export,
    //     Examples:
    //         (export "memory" (memory 0))
    //         (export "NSystem" (global 1))
    //         (export "__wasm_call_ctors" (func $__wasm_call_ctors))
    NCC_addRule(cc, "Export->Start", "", onExport_Start, False, False, False);
    NCC_addRule(cc, "Export->Name", "${String}", onExport_Name, False, False, True);
    NCC_addRule(cc, "Export->Memory", "(${} memory ${} ${PositiveInteger} ${})", onExport_Memory, False, False, True);
    NCC_addRule(cc, "Export->Global", "(${} global ${} ${PositiveInteger} ${})", onExport_Global, False, False, True);
    NCC_addRule(cc, "Export->Func", "(${} func ${} ${Identifier} ${})", onExport_Func, False, False, True);
    NCC_addRule(cc, "Export", "${Export->Start} (${} export ${} ${Export->Name} ${} ${Export->Memory}|${Export->Global}|${Export->Func} ${})", 0, False, False, False);

    // Element,
    //     Example:
    //         (elem (;0;) (i32.const 1) func $initialize $terminate $strlen)
    NCC_addRule(cc, "Element->TableOffset", "(${} i32.const ${} ${PositiveInteger} ${})", 0, False, False, False);
    NCC_addRule(cc, "Element", "(${} elem ${} ${Element->TableOffset}|${Empty} ${} func ${} ${Identifier} ${} {${Identifier} ${}}^*)", onElement_End, False, False, True);

    // Data,
    //     Example:
    //         (data (;0;) (i32.const 1024) "NCString.parseInteger()\00Integer length can't exceed 10 digits and a sign. Found: %s%s\00\19\09\00\00!\09\00\00\c9\07\00\00\d9\07\00\00\09\09\00\00\f9\08\00\00(module)\00Parsing result\00%s\0a\00True\00False\00"))
    NCC_addRule(cc, "Data->MemoryOffset", "(${} i32.const ${} ${PositiveInteger} ${})", 0, False, False, False);
    NCC_addRule(cc, "Data", "(${} data ${} ${Data->MemoryOffset} ${} ${String} ${})", onData_End, False, False, True);

    // Module,
    NCC_addRule(cc, "Module->Start", "", onModule_Start, False, False, False);
    NCC_addRule(cc, "ModuleElement", "${Type} | ${Func} | ${Table} | ${Memory} | ${Global} | ${Export} | ${Element} | ${Data}", 0, False, False, False);
    NCC_addRule(cc, "Module", "${Module->Start} (${} module ${} {${ModuleElement}${}}^*)", 0, False, False, False);

    // Document,
    NCC_addRule(cc, "Document", "${} ${Module} ${} {${Module} ${}}^*", 0, True, False, True);

    return cc;
}

///////////////////////////////////////////////////////////////////////////
// Function execution
///////////////////////////////////////////////////////////////////////////

static void callFunction(NWM_Function functionHandle) {

    struct Function* function = functionHandle;
    struct NByteVector* stack = &function->module->stack.data;
    struct NByteVector* memory = &function->module->memory->data;
    struct NVector* locals = &function->localVariables;

    // Compute locals pointer in the stack,
    struct Type* functionType = *(struct Type**) NVector.get(&function->module->types, function->typeIndex);
    uint32_t stackSize = NByteVector.size(stack);
    uint32_t localsStartIndex = stackSize - functionType->parametersSizeBytes;
    uint8_t* localsPointer = &stack->objects[localsStartIndex];

    // Resize the stack to accommodate the locals,
    NByteVector.resize(stack, stackSize + function->localVariablesSizeBytes);

    // Execute,
    uint32_t instructionsCount = NVector.size(&function->instructions);
    for (int32_t i=0; i<instructionsCount; i++) {

        struct Instruction* instruction = NVector.get(&function->instructions, i);
        switch(instruction->type) {
            case INST_i32_const:
                NByteVector.pushBack32Bit(stack, instruction->argument.int32);
                continue;
            case INST_i32_load8_u: {
                int32_t index;
                NByteVector.popBack32Bit(stack, &index);         // Pop the index from the stack.
                index += instruction->argument.int32;            // Add the offset.
                int32_t value = NByteVector.get(memory, index);  // Get the value from the memory.
                NByteVector.pushBack32Bit(stack, value);         // Push the value into the stack.
                continue;
            }
            case INST_local_get: {

                // TODO: locals should include parameters too...

                NLOGE("sdf", "index: %d", instruction->argument.int32);
                struct LocalVariable *localVariable = NVector.get(locals, instruction->argument.int32);
                NLOGE("sdf", "offset: %d, size: %d", localVariable->offset, localVariable->sizeBytes);
                NByteVector.pushBackBulk(stack, &localsPointer[localVariable->offset], localVariable->sizeBytes);
                continue;
            }
            case INST_local_set:
            case INST_local_tee:
            case INST_i32_add:
            case INST_i32_and:
            case INST_loop:
            case INST_block:
            case INST_end:
            case INST_i32_eq:
            case INST_i32_eqz:
            case INST_br:
            case INST_br_if:
            case INST_call_indirect:
            case INST_return:
            default: ;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
// Reference machine interface
///////////////////////////////////////////////////////////////////////////

static NWM_Function getFunction(struct NWM_WasmMachine* machine, int32_t moduleIndex, const char* functionName) {

    struct ReferenceMachineData* machineData = machine->data;
    struct Module* module = *(struct Module**) NVector.get(&machineData->modules, moduleIndex);

    // Find the function within the module,
    // TODO: should look into exports instead?
    for (int32_t i=NVector.size(&module->functions)-1; i>=0; i--) {
        struct Function* function = *(struct Function**) NVector.get(&module->functions, i);
        if (NCString.equals(NString.get(&function->name), functionName)) {
            return function;
        }
    }
    return 0;
}

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
    outputMachine->getFunction = getFunction;
    outputMachine->callFunction = callFunction;

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
