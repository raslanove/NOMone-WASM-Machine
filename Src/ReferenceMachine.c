
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
    //
    // For extensive examples of WAT code, see: https://github.com/mdn/webassembly-examples

    // Basic elements,
    NCC_addRule(cc, "Empty", "", 0, False);
    NCC_addRule(cc, "WhiteSpace", "{\\ |\t|\r|\n}^*", 0, False);
    NCC_addRule(cc, "NotWhiteSpaceLiteral", "\x01-\x08 | \x0b-\x0c | \x0e-\x1f | \x21-\xff", 0, False);
    NCC_addRule(cc, "LineEnd", "\n|${Empty}", 0, False);
    NCC_addRule(cc, "LineComment", ";;*${LineEnd}", 0, False);
    NCC_addRule(cc, "BlockComment", "(;*;)", 0, False);
    NCC_addRule(cc, "", "{${WhiteSpace}|${BlockComment}}^*", 0, False);
    NCC_addRule(cc, "Integer", "0-9 | 1-9 0-9^*", 0, False);
    NCC_addRule(cc, "Identifier", "\\$${NotWhiteSpaceLiteral}^*", 0, False);
    NCC_addRule(cc, "DataType", "{i32}|{i64}|{f32}|{f64}", 0, False);
    NCC_addRule(cc, "Parameters", "(${} param ${} ${DataType} ${} {${DataType} ${}}^*)", 0, False);
    NCC_addRule(cc, "Result", "(${} result ${} ${DataType} ${})", 0, False);

    /*
    // Instructions,
    NCC_addRule(ncc, "Push", "push ${WhiteSpace} ${StackModifier} ${WhiteSpace} ${Integer}", pushListener, False);
    NCC_addRule(ncc, "Pop" , "pop  ${WhiteSpace} ${StackModifier} ${WhiteSpace} ${Integer}",  popListener, False);
    NCC_addRule(ncc, "Add", "add", addListener, False);
    NCC_addRule(ncc, "Sub", "sub", subListener, False);
    NCC_addRule(ncc, "And", "and", andListener, False);
    NCC_addRule(ncc, "Or" , "or" ,  orListener, False);
    NCC_addRule(ncc, "Eq" , "eq" ,  eqListener, False);
    NCC_addRule(ncc, "LT" , "lt" ,  ltListener, False);
    NCC_addRule(ncc, "GT" , "gt" ,  gtListener, False);
    NCC_addRule(ncc, "Neg", "neg", negListener, False);
    NCC_addRule(ncc, "Not", "not", notListener, False);
    NCC_addRule(ncc, "Jmp", "goto ${WhiteSpace} ${Identifier}", jumpListener, False);
    NCC_addRule(ncc, "JNZ", "if\\-goto ${WhiteSpace} ${Identifier}", jumpNotZeroListener, False);
    NCC_addRule(ncc, "Function", "function ${WhiteSpace} ${Identifier} ${WhiteSpace} ${Integer}", functionListener, False);
    NCC_addRule(ncc, "Return", "return", returnListener, False);
    NCC_addRule(ncc, "Call", "call ${WhiteSpace} ${Identifier} ${WhiteSpace} ${Integer}", callListener, False);

    NCC_addRule(ncc, "Instruction", "${Push} | ${Pop} | ${Add} | ${Sub} | ${And} | ${Or} | ${Eq} | ${LT} | ${GT} | ${Neg} | ${Not} | ${Jmp} | ${JNZ} | ${Function} | ${Return} | ${Call}", 0, False);
    */

    // Structures,

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
    NCC_addRule(cc, "Func->Type", "(${} type ${} ${Integer} ${})", 0, False);
    NCC_addRule(cc, "Func->Local", "(${} local ${} ${DataType} ${} {${DataType} ${}}^*)", 0, False);
    NCC_addRule(cc, "Func", "(${} func ${} ${Identifier} ${}"
                            "${Func->Type} |${Empty} ${}"
                            "${Parameters} |${Empty} ${}"
                            "${Result}     |${Empty} ${}"
                            "${Func->Local}|${Empty} ${})", printMatch, False);

    // ...xxxx

    // Module,
    NCC_addRule(cc, "ModuleElement", "${Type} | ${Func}", printMatch, False);
    NCC_addRule(cc, "Module", "(${} module ${} {${ModuleElement}${}}^*)", 0, False);

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

    return matchLength == NCString.length(watCode);
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