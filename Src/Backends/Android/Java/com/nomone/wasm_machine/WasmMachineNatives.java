package com.nomone.wasm_machine;

public class WasmMachineNatives {

    static {
        System.loadLibrary("WasmMachine");
    }

    public static String compileWasmToBites() {
        return nativeCompileWasmToBites();
    }

    public static native String nativeCompileWasmToBites();
}
