
#include <ReferenceMachine.h>
#include <WasmMachine.h>

#include <NSystemUtils.h>
#include <NCString.h>

#include <NCC.h>

// TODO: have a separate cc for each machine, to allow multiple machines concurrent action...
static struct NCC* cc=0;
static int32_t machinesCount=0;

static void printMatch(struct NCC* ncc, struct NString* ruleName, int32_t variablesCount) {
    NLOGI("ReferenceMachine", "ruleName: %s, variablesCount: %d", NString.get(ruleName), variablesCount);
    struct NCC_Variable variable;
    while (NCC_popVariable(ncc, &variable)) {
        NLOGI("ReferenceMachine", "            Name: %s%s%s, Value: %s%s%s", NTCOLOR(HIGHLIGHT), NString.get(&variable.name), NTCOLOR(STREAM_DEFAULT), NTCOLOR(HIGHLIGHT), NString.get(&variable.value), NTCOLOR(STREAM_DEFAULT));
        NCC_destroyVariable(&variable);
    }
    NLOGI("", "");
}

static void prepareCC() {

    cc = NCC_createNCC();

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

    NCC_addRule(cc, "Empty", "", 0, False);
    NCC_addRule(cc, "Literal", "\x01-\xff", 0, False);
    NCC_addRule(cc, "WhiteSpace", "{\\ |\t|\r|\n}^*", 0, False);
    NCC_addRule(cc, "NotWhiteSpaceLiteral", "\x01-\x08 | \x0b-\x0c | \x0e-\x1f | \x21-\xff", 0, False);
    NCC_addRule(cc, "LineEnd", "\n|${Empty}", 0, False);
    NCC_addRule(cc, "LineComment", ";;*${LineEnd}", 0, False);
    NCC_addRule(cc, "BlockComment", "(;*;)", 0, False);
    NCC_addRule(cc, "", "{${WhiteSpace}|${LineComment}|${BlockComment}}^*", 0, False);
    NCC_addRule(cc, "PositiveInteger", "0 | 1-9 0-9^*", 0, False);
    NCC_addRule(cc, "Integer", "0 | {\\-|${Empty} 1-9 0-9^*}", 0, False);
    // TODO: float......xxxx
    NCC_addRule(cc, "String", "\" { ${Literal}|{\\\\${Literal}} }^* \"", 0, False);
    // Note: maybe it's better to specify the supported ranges instead of excluding the unsupported ones?
    NCC_addRule(cc, "IdentifierLiteral", "\x01-\x08 | \x0b-\x0c | \x0e-\x1f | \x21-\x27 | \x2b-\xff", 0, False); // Note, characters like '*', ' ' and '-' must be escaped before being used in a literal-range expression.
    NCC_addRule(cc, "Identifier", "\\$${IdentifierLiteral}^*", 0, False);
    NCC_addRule(cc, "DataType", "{i32}|{i64}|{f32}|{f64}", 0, False);
    NCC_addRule(cc, "Parameters", "(${} param ${} ${DataType} ${} {${DataType} ${}}^*)", 0, False);
    NCC_addRule(cc, "Result", "(${} result ${} ${DataType} ${})", 0, False);
    NCC_addRule(cc, "FuncType", "(${} type ${} ${PositiveInteger} ${})", 0, False);

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

    NCC_addRule(cc, "I-i32.const"    , "i32.const ${} ${Integer}"        , 0, False);
    NCC_addRule(cc, "I-i32.load8_u"  , "i32.load8_u"                     , 0, False);
    NCC_addRule(cc, "I-local.get"    , "local.get ${} ${PositiveInteger}", 0, False);
    NCC_addRule(cc, "I-local.set"    , "local.set ${} ${PositiveInteger}", 0, False);
    NCC_addRule(cc, "I-local.tee"    , "local.tee ${} ${PositiveInteger}", 0, False); // The same as set, but doesn't consume the value from the stack.
    NCC_addRule(cc, "I-i32.add"      , "i32.add"                         , 0, False);
    NCC_addRule(cc, "I-i32.and"      , "i32.and"                         , 0, False);
    NCC_addRule(cc, "I-loop"         , "loop"                            , 0, False);
    NCC_addRule(cc, "I-block"        , "block"                           , 0, False);
    NCC_addRule(cc, "I-end"          , "end"                             , 0, False);
    NCC_addRule(cc, "I-i32.eq"       , "i32.eq"                          , 0, False);
    NCC_addRule(cc, "I-i32.eqz"      , "i32.eqz"                         , 0, False);
    NCC_addRule(cc, "I-br"           , "br ${} ${PositiveInteger}"       , 0, False);
    NCC_addRule(cc, "I-br_if"        , "br_if ${} ${PositiveInteger}"    , 0, False);
    NCC_addRule(cc, "I-call_indirect", "call_indirect ${} ${FuncType}"   , 0, False);  // The function type is needed for type checking only.
                                                                                       // The indirect call pops the desired function index
                                                                                       // from the stack, or from a nested instruction.
    NCC_addRule(cc, "I-return"       , "return"                          , 0, False);


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
                                   "${I-return}         ", 0, False);

    /////////////////////////////////////////////////////////////////////////////
    // Structures,
    /////////////////////////////////////////////////////////////////////////////

    // Type,
    //     Examples:
    //         (type (;0;) (func))
    //         (type (;1;) (func (param i32 i32 i64)))
    //         (type (;1;) (func (param i32 i32 i64) (result i32)))
    NCC_addRule(cc, "Type->Func", "(${} func ${} ${Parameters}|${Empty} ${} ${Result}|${Empty})", 0, False);
    NCC_addRule(cc, "Type", "(${} type ${} ${Type->Func} ${})", 0, False);

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
    NCC_addRule(cc, "Func->Local", "(${} local ${} ${DataType} ${} {${DataType} ${}}^*)", 0, False);
    NCC_addRule(cc, "Func", "(${} func ${} ${Identifier} ${}"
                            "${FuncType} |${Empty} ${}"
                            "${Parameters} |${Empty} ${}"
                            "${Result}     |${Empty} ${}"
                            "${Func->Local}|${Empty} ${}"
                            "{${Instruction} ${}}^*)", 0, False);

    // Table,
    //     Example:
    //         (table (;0;) 62 62 funcref)  ;; 62 is the number for functions we have. Here, it defines
    //                                      ;; the limits of the table (min and max sizes).
    //
    // See: https://developer.mozilla.org/en-US/docs/WebAssembly/Understanding_the_text_format#webassembly_tables
    NCC_addRule(cc, "Table->minSize", "${PositiveInteger}", 0, False);
    NCC_addRule(cc, "Table->maxSize", "${PositiveInteger}", 0, False);
    NCC_addRule(cc, "Table", "(${} table ${} ${Table->minSize}|${Empty} ${} ${Table->maxSize} ${} funcref ${})", 0, False);

    // Memory,
    NCC_addRule(cc, "Memory->pagesCount", "${PositiveInteger}", 0, False);
    NCC_addRule(cc, "Memory", "(${} memory ${} ${Memory->pagesCount} ${})", 0, False);

    // Global,
    //     Examples:
    //         (global (;0;) (mut i32) (i32.const 68272))
    //         (global (;1;) i32 (i32.const 1024))
    NCC_addRule(cc, "Global->mutable", "(${} mut ${} ${DataType} ${})", 0, False);
    NCC_addRule(cc, "Global->value", "(${} ${DataType}.const ${} ${Integer} ${})", 0, False); // TODO: support float. Add two cases, integerType+integerValue and floatType+floatValue...
    NCC_addRule(cc, "Global", "(${} global ${} ${DataType}|${Global->mutable} ${} ${Global->value} ${})", 0, False);

    // Export,
    //     Examples:
    //         (export "memory" (memory 0))
    //         (export "NSystem" (global 1))
    //         (export "__wasm_call_ctors" (func $__wasm_call_ctors))
    NCC_addRule(cc, "MemoryIndex", "(${} memory ${} ${PositiveInteger} ${})", 0, False);
    NCC_addRule(cc, "GlobalIndex", "(${} global ${} ${PositiveInteger} ${})", 0, False);
    NCC_addRule(cc, "FuncName", "(${} func ${} ${Identifier} ${})", 0, False);
    NCC_addRule(cc, "Export", "(${} export ${} ${String} ${} ${MemoryIndex}|${GlobalIndex}|${FuncName} ${})", 0, False);

    // Element,
    //     Example:
    //         (elem (;0;) (i32.const 1) func $initialize $terminate $strlen)
    // TODO:...xxx

    // Data,
    // TODO:...xxx

    // Module,
    NCC_addRule(cc, "ModuleStart", "${Empty}", printMatch, False);
    NCC_addRule(cc, "ModuleElement", "${Type} | ${Func} | ${Table} | ${Memory} | ${Global} | ${Export}", printMatch, False);
    NCC_addRule(cc, "Module", "(${} module ${} ${ModuleStart} {${ModuleElement}${}}^*)", 0, False);

    // Document,
    //NCC_addRule(ncc, "Document", "{${WhiteSpace} | ${LineComment} | ${Label} | ${Instruction}}^*", 0, True);
    NCC_addRule(cc, "Document", "${} ${Module} ${} {${Module} ${}}^*", 0, True);
}

static void destroyCC() {
    NCC_destroyAndFreeNCC(cc);
    cc = 0;
}


static void destroyReferenceMachine(struct NWM_WasmMachine *machine) {
    machine->alive = False;
    machinesCount--;
    if (!machinesCount) destroyCC();
}

static void destroyAndFreeReferenceMachine(struct NWM_WasmMachine *machine) {
    destroyReferenceMachine(machine);
    NFREE(machine, "ReferenceMachine.destroyAndFreeReferenceMachine() machine");
}

static boolean parseWatCode(struct NWM_WasmMachine *machine, const char* watCode) {

    int32_t matchLength = NCC_match(cc, watCode);
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

    // Prepare CC,
    if (!cc) prepareCC();
    machinesCount++;

    return outputMachine;
}

struct NWM_WasmMachine *NWM_createReferenceMachine() {
    struct NWM_WasmMachine* machine = NMALLOC(sizeof(struct NWM_WasmMachine), "ReferenceMachine.NWM_createReferenceMachine() machine");
    return NWM_initializeReferenceMachine(machine);
}