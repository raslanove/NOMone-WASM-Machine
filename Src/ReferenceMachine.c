
#include <ReferenceMachine.h>
#include <WasmMachine.h>

#include <NSystemUtils.h>
#include <NCString.h>

#include <NCC.h>

#define NWM_VERBOSE 1

///////////////////////////////////////////////////////////////////////////
// Types
///////////////////////////////////////////////////////////////////////////

typedef enum { TYPE_NONE, TYPE_INT32, TYPE_INT64, TYPE_FLOAT32, TYPE_FLOAT64 } DataType; // We can have a void return value.
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
    struct NVector parameterTypes;
    DataType resultType;
};

struct Instruction {
    InstructionType type;
    union Value argument;
};

struct Function {
    uint32_t typeIndex;
    struct NString name;
    struct NVector localVariablesTypes;
    struct NVector instructions;
};

struct Table {
    struct NVector functions;
};

struct Memory {
    struct NByteVector data;
};

struct Global {
    DataType type;
    union Value value;
};

struct Export {
    ExportType type;
    uint32_t index;
};

struct Module {
    struct Memory* memory;        // Could be imported from another machine (or parent code).
    struct Table* functionsTable; // Could be imported from another machine (or parent code).
    boolean memoryImported, tableImported;

    struct NVector types;
    struct NVector functions;
    struct NVector globals;
    struct NVector exports;
};

struct ReferenceMachineData {
    struct NCC* cc;
    struct NVector modules;
};

struct ParsingData {
    struct NVector* modules;
};

///////////////////////////////////////////////////////////////////////////
// Reference machine population (common)
///////////////////////////////////////////////////////////////////////////

static void printMatch(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    NLOGI("ReferenceMachine", "ruleName: %s, variablesCount: %d", NString.get(ruleName), variablesCount);
    struct NCC_Variable variable;
    while (NCC_popVariable(ncc, &variable)) {
        NLOGI("ReferenceMachine", "            Name: %s%s%s, Value: %s%s%s", NTCOLOR(HIGHLIGHT), NString.get(&variable.name), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NString.get(&variable.value), NTCOLOR(STREAM_DEFAULT));
        NCC_destroyVariable(&variable);
    }
    NLOGI("", "");
}

///////////////////////////////////////////////////////////////////////////
// Type
///////////////////////////////////////////////////////////////////////////

static void onType_Parameters(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    printMatch(ncc, ruleName, variablesCount);
}

static void onType_Result(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    printMatch(ncc, ruleName, variablesCount);
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
// Module
///////////////////////////////////////////////////////////////////////////

static struct Module* createModule() {

    struct Module* module = NMALLOC(sizeof(struct Module), "ReferenceMachine.createModule() module");
    NSystemUtils.memset(module, 0, sizeof(struct Module));

    NVector.initialize(&module->types    , 0, sizeof(struct Type*    ));
    NVector.initialize(&module->functions, 0, sizeof(struct Function*));
    NVector.initialize(&module->globals  , 0, sizeof(struct Global*  ));
    NVector.initialize(&module->exports  , 0, sizeof(struct Export*  ));

    return module;
}

static void destroyAndFreeModule(struct Module* module) {

    // TODO: destroy memory...
    if (!module->memoryImported) {}
    // TODO: destroy table...
    if (!module->tableImported) {}

    // Types,
    struct Type* type; while (NVector.popBack(&module->types, &type)) destroyAndFreeType(type);
    NVector.destroy(&module->types);


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

    NCC_addRule(cc, "Empty", "", 0, False, False, True);
    NCC_addRule(cc, "Literal", "\x01-\xff", 0, False, False, True);
    NCC_addRule(cc, "WhiteSpace", "{\\ |\t|\r|\n}^*", 0, False, False, True);
    NCC_addRule(cc, "NotWhiteSpaceLiteral", "\x01-\x08 | \x0b-\x0c | \x0e-\x1f | \x21-\xff", 0, False, False, True);
    NCC_addRule(cc, "LineEnd", "\n|${Empty}", 0, False, False, True);
    NCC_addRule(cc, "LineComment", ";;*${LineEnd}", 0, False, False, True);
    NCC_addRule(cc, "BlockComment", "(;*;)", 0, False, False, True);
    NCC_addRule(cc, "", "{${WhiteSpace}|${LineComment}|${BlockComment}}^*", 0, False, False, True);
    NCC_addRule(cc, "PositiveInteger", "0 | 1-9 0-9^*", 0, False, False, True);
    NCC_addRule(cc, "Integer", "0 | {\\-|${Empty} 1-9 0-9^*}", 0, False, False, True);
    // TODO: float......xxxx
    NCC_addRule(cc, "String", "\" { ${Literal}|{\\\\${Literal}} }^* \"", 0, False, False, True);
    // Note: maybe it's better to specify the supported ranges instead of excluding the unsupported ones?
    NCC_addRule(cc, "IdentifierLiteral", "\x01-\x08 | \x0b-\x0c | \x0e-\x1f | \x21-\x27 | \x2b-\xff", 0, False, False, True); // Note, characters like '*', ' ' and '-' must be escaped before being used in a literal-range expression.
    NCC_addRule(cc, "Identifier", "\\$${IdentifierLiteral}^*", 0, False, False, True);
    NCC_addRule(cc, "DataType", "{i32}|{i64}|{f32}|{f64}", 0, False, True, True);
    NCC_addRule(cc, "Parameters", "(${} param ${} ${DataType} ${} {${DataType} ${}}^*)", 0, False, False, False);
    NCC_addRule(cc, "Result", "(${} result ${} ${DataType} ${})", 0, False, False, False);
    NCC_addRule(cc, "FuncType", "(${} type ${} ${PositiveInteger} ${})", 0, False, False, True);

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

    NCC_addRule(cc, "I-i32.const"    , "i32.const ${} ${Integer}"        , 0, False, False, True);
    NCC_addRule(cc, "I-i32.load8_u"  , "i32.load8_u"                     , 0, False, False, True);
    NCC_addRule(cc, "I-local.get"    , "local.get ${} ${PositiveInteger}", 0, False, False, True);
    NCC_addRule(cc, "I-local.set"    , "local.set ${} ${PositiveInteger}", 0, False, False, True);
    NCC_addRule(cc, "I-local.tee"    , "local.tee ${} ${PositiveInteger}", 0, False, False, True); // The same as set, but doesn't consume the value from the stack.
    NCC_addRule(cc, "I-i32.add"      , "i32.add"                         , 0, False, False, True);
    NCC_addRule(cc, "I-i32.and"      , "i32.and"                         , 0, False, False, True);
    NCC_addRule(cc, "I-loop"         , "loop"                            , 0, False, False, True);
    NCC_addRule(cc, "I-block"        , "block"                           , 0, False, False, True);
    NCC_addRule(cc, "I-end"          , "end"                             , 0, False, False, True);
    NCC_addRule(cc, "I-i32.eq"       , "i32.eq"                          , 0, False, False, True);
    NCC_addRule(cc, "I-i32.eqz"      , "i32.eqz"                         , 0, False, False, True);
    NCC_addRule(cc, "I-br"           , "br ${} ${PositiveInteger}"       , 0, False, False, True);
    NCC_addRule(cc, "I-br_if"        , "br_if ${} ${PositiveInteger}"    , 0, False, False, True);
    NCC_addRule(cc, "I-call_indirect", "call_indirect ${} ${FuncType}"   , 0, False, False, True);  // The function type is needed for type checking only.
                                                                                                    // The indirect call pops the desired function index
                                                                                                    // from the stack, or from a nested instruction.
    NCC_addRule(cc, "I-return"       , "return"                          , 0, False, False, True);


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
                                   "${I-return}         ", 0, False, False, True);

    /////////////////////////////////////////////////////////////////////////////
    // Structures,
    /////////////////////////////////////////////////////////////////////////////

    // Type,
    //     Examples:
    //         (type (;0;) (func))
    //         (type (;1;) (func (param i32 i32 i64)))
    //         (type (;1;) (func (param i32 i32 i64) (result i32)))
    // ...xxx
    NCC_addRule(cc, "Type->Result", "${Result}", onType_Result, False, False, True);
    NCC_addRule(cc, "Type->Parameters", "${Parameters}", onType_Parameters, False, False, True);
    NCC_addRule(cc, "Type->Func", "(${} func ${} ${Type->Parameters}|${Empty} ${} ${Type->Result}|${Empty})", 0, False, False, True);
    NCC_addRule(cc, "Type->Start", "${Empty}", onType_Start, False, False, True);
    NCC_addRule(cc, "Type", "${Type->Start} (${} type ${} ${Type->Func} ${})", 0, False, False, True);

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
    NCC_addRule(cc, "Func->Local", "(${} local ${} ${DataType} ${} {${DataType} ${}}^*)", 0, False, False, True);
    NCC_addRule(cc, "Func", "(${} func ${} ${Identifier} ${}"
                            "${FuncType} |${Empty} ${}"
                            "${Parameters} |${Empty} ${}"
                            "${Result}     |${Empty} ${}"
                            "${Func->Local}|${Empty} ${}"
                            "{${Instruction} ${}}^*)", 0, False, True, True);

    // Table,
    //     Example:
    //         (table (;0;) 62 62 funcref)  ;; 62 is the number for functions we have. Here, it defines
    //                                      ;; the limits of the table (min and max sizes).
    //
    // See: https://developer.mozilla.org/en-US/docs/WebAssembly/Understanding_the_text_format#webassembly_tables
    NCC_addRule(cc, "Table->minSize", "${PositiveInteger}", 0, False, False, True);
    NCC_addRule(cc, "Table->maxSize", "${PositiveInteger}", 0, False, False, True);
    NCC_addRule(cc, "Table", "(${} table ${} ${Table->minSize}|${Empty} ${} ${Table->maxSize} ${} funcref ${})", 0, False, True, True);

    // Memory,
    NCC_addRule(cc, "Memory->pagesCount", "${PositiveInteger}", 0, False, False, True);
    NCC_addRule(cc, "Memory", "(${} memory ${} ${Memory->pagesCount} ${})", 0, False, True, True);

    // Global,
    //     Examples:
    //         (global (;0;) (mut i32) (i32.const 68272))
    //         (global (;1;) i32 (i32.const 1024))
    NCC_addRule(cc, "Global->mutable", "(${} mut ${} ${DataType} ${})", 0, False, False, True);
    NCC_addRule(cc, "Global->value", "(${} ${DataType}.const ${} ${Integer} ${})", 0, False, False, True); // TODO: support float. Add two cases, integerType+integerValue and floatType+floatValue...
    NCC_addRule(cc, "Global", "(${} global ${} ${DataType}|${Global->mutable} ${} ${Global->value} ${})", 0, False, True, True);

    // Export,
    //     Examples:
    //         (export "memory" (memory 0))
    //         (export "NSystem" (global 1))
    //         (export "__wasm_call_ctors" (func $__wasm_call_ctors))
    NCC_addRule(cc, "MemoryIndex", "(${} memory ${} ${PositiveInteger} ${})", 0, False, False, True);
    NCC_addRule(cc, "GlobalIndex", "(${} global ${} ${PositiveInteger} ${})", 0, False, False, True);
    NCC_addRule(cc, "FuncName", "(${} func ${} ${Identifier} ${})", 0, False, False, True);
    NCC_addRule(cc, "Export", "(${} export ${} ${String} ${} ${MemoryIndex}|${GlobalIndex}|${FuncName} ${})", 0, False, True, True);

    // Element,
    //     Example:
    //         (elem (;0;) (i32.const 1) func $initialize $terminate $strlen)
    NCC_addRule(cc, "TableOffset", "(${} i32.const ${} ${PositiveInteger} ${})", 0, False, False, True);
    NCC_addRule(cc, "Element", "(${} elem ${} ${TableOffset}|${Empty} ${} func ${} ${Identifier} ${} {${Identifier} ${}}^*)", 0, False, True, True);

    // Data,
    //     Example:
    //         (data (;0;) (i32.const 1024) "NCString.parseInteger()\00Integer length can't exceed 10 digits and a sign. Found: %s%s\00\19\09\00\00!\09\00\00\c9\07\00\00\d9\07\00\00\09\09\00\00\f9\08\00\00(module)\00Parsing result\00%s\0a\00True\00False\00"))
    NCC_addRule(cc, "MemoryOffset", "(${} i32.const ${} ${PositiveInteger} ${})", 0, False, False, True);
    NCC_addRule(cc, "Data", "(${} data ${} ${MemoryOffset} ${} ${String} ${})", 0, False, True, True);

    // Module,
    NCC_addRule(cc, "ModuleElement", "${Type} | ${Func} | ${Table} | ${Memory} | ${Global} | ${Export} | ${Element} | ${Data}", 0, False, False, True);
    NCC_addRule(cc, "Module->Start", "${Empty}", onModule_Start, False, False, True);
    NCC_addRule(cc, "Module", "${Module->Start} (${} module ${} {${ModuleElement}${}}^*)", 0, False, False, True);

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
