package com.nomone.wasm_machine;

public class WasmMachineNatives {

    static {
        System.loadLibrary("WasmMachine");
    }

    public static String compileWasmToBites(String watCode) {
        //return nativeCompileWasmToBites(watCode);
        return "besm Allah";
    }

    //public static native String nativeCompileWasmToBites(String watCode);
    public static native int nativeCreateReferenceMachine();
}
